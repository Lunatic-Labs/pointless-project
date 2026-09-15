# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

The Pointless Project is a puzzle challenge for pre-college students, inspired by the Synacor Challenge. A C++ generator builds a personalized, offline game: a chain of nested, password-protected zip files. Each layer holds one puzzle's `instructions.html`, and that puzzle's answer is the password to the next zip.

## Commands

The C++ code uses g++ with C++17 and needs `libzip` (`sudo apt install g++ libboost-all-dev libzip-dev php-cli gcovr`).

There is a single `puzzle-code/Makefile`; **run make from `puzzle-code/`**. Objects go in `puzzle-code/build/` (with `-MMD` header dependencies). **The binaries must run from their own directory** (`src/` or `tests/`), because all resource paths are relative to it, for example `../resources/files-math/.desc.txt`.

Puzzle generator:
- `make` / `make build`: builds `src/main` (debug, `-O0 --coverage`) and creates `src/zipfiles/`
- `make run`: builds, runs `cleanzip`, then runs `./main` in `src/`
- `./main -s <seed>` (in `src/`): uses a fixed, nonzero seed (otherwise the seed comes from the current time)
- `./main -e <email>` (in `src/`): uses the player's seed from `utils_seed_from_email()`; this is what the website runs
- `./main -a` (in `src/`): answers only; prints each puzzle's password without writing zips or generated files
- `make cleanzip`: removes generated zips and generated files under `resources/`; `make clean` also removes `build/` and both binaries (not `production/`)
- `make coverage`: runs gcov on `src/*.cpp` (requires `make run` or `make test` to have run first)
- `make production`: builds an optimized generator without coverage (objects in `build/production/`) and replaces `puzzle-code/production/` with `src/main`, an empty `src/zipfiles/`, and a clean copy of `resources/`. Only a person runs this; don't run it as a side effect of other work, since it changes what the live site serves.

Tests:
- `make test`: builds `tests/main` from `tests/*.cpp` plus every `src/` object except `src/main.o`, then runs it in `tests/`
- There is no test runner or filter. Tests are `assert`-based functions listed in `tests/main.cpp`. To run a single test, temporarily comment out the other entries in the `tests` vector.
- Tests run with `FLAGS |= ANS_ONLY`, so they don't write zips.

PHP site (`web-server/`):
- `php -S localhost:8000`. Downloads need `make production` to have been run. The site never runs make. Set `POINTLESS_GENERATOR_DIR` to use a production tree elsewhere, and `POINTLESS_PLAYERS_FILE` to move the player data.
- Tests: `php web-server/tests/run.php [name-substring]` (runs from any directory). It starts its own `php -S` on a free port with a temp players file, a fake generator (`fake_generator($mode)` in `tests/lib.php`), temp sessions, and `TMPDIR`, resets that state before each test, and exits 1 on failure. It never touches `data/` or `production/`. Tests are `test_*` functions in `tests/*-test.php` using `check()` (not `assert()`, which php-cli disables), listed in `$tests` in `tests/run.php`. `test_generate_real_generator` runs `production/src/main` (or `POINTLESS_TEST_GENERATOR_DIR`): skipped without `src/main`, fails if that tree is broken.

## Architecture

**Puzzle pipeline.** `src/main.cpp` builds a `std::vector<Puzzle>` by calling each `<name>_puzzle_create(seed)` in play order. It prints every password, then `create_nested_zipfiles` walks the list in reverse. `zipfiles/puzzleN.zip` contains puzzle N's files, is encrypted with puzzle N's password, and (except for the last puzzle) contains `puzzle{N+1}.zip`. `puzzle1.zip` is the full game; the inner zips are also left on disk for easier testing.

**A puzzle** has two parts:
1. A resource directory `puzzle-code/resources/files-<name>/` containing `.desc.txt`, an HTML/JS body with `%DELIM` placeholders, plus any assets. Files and directories starting with `.` are excluded from the zip (see `utils_walkdir`). Resources must not load anything from the internet, because the game is played offline.
2. `src/<name>-puzzle.cpp`, which rolls random values, calls `utils_html_printf(title, desc_path, {args...})` to substitute `%DELIM` in order, wrap the body in `<section>` (not `<div>`: several descriptions style `.container div`), and surround it with `resources/templates/header.txt` and `footer.txt` (the header opens `<body>` and two divs; the footer closes them, so a `.desc.txt` must be a balanced fragment), writes `instructions.html` with `utils_generate_file`, and returns `Puzzle{contents_fp, contents_html, password, extra_info}`. `extra_info` is only printed for debugging.

To add a puzzle:
1. Create the resource directory and the `.cpp` file.
2. Declare `Puzzle <name>_puzzle_create(long)` in `src/include/puzzle.h` and insert the call into the vector in `main.cpp`.
3. Add `tests/<name>-puzzle-test.cpp`, declare it in `tests/include/test.h`, and list it in `tests/main.cpp`.

The Makefile uses wildcards, so no build changes are needed. If the puzzle writes new generated files into `resources/`, add them to `clean-generated` in the Makefile.

**Rematch puzzle.** `rematch-puzzle.cpp` is a composite. It creates the maze, encrypt, and based rematch sub-puzzles, zips each into `resources/files-rematch/rematchN.zip` along with a generated password fragment (`.passwords/passwordN.txt`), and returns the concatenated fragments as the password for the next main layer.

**Determinism and RNG.** `utils_rng_roll(min, max, long &seed)` reseeds `srand` and mutates the seed on every call. Within a puzzle, adding, removing, or reordering rolls changes every later value. Tests assert exact passwords for specific seeds (for example `math_puzzle_create(1)` → `"75"`), so RNG changes require updating the expected values. A player's seed comes from their email via `utils_seed_from_email()` in `src/utils.cpp`, the only copy of the formula. Changing it changes every existing player's puzzle.

**Global flags.** `extern uint32_t FLAGS` (defined in each `main.cpp`) takes bits from `utils.h`: `ANS_ONLY`, `SET_SEED`, `NO_HDR`, `NO_FTR`, and `BISON_GRID`. Some utils, such as `utils_mkdir`, become no-ops under `ANS_ONLY`.

**Graphics.** `src/graphics.{h,cpp}` provides `Image`/`Pixel` and an `Svg` builder with `Rect`/`Circle` shapes. Puzzles embed the SVG strings in their HTML. The project has moved from PPM output to SVG.

**Web.** `web-server/` is plain PHP with no framework or database.
- Pages share `includes/header.php` and `includes/footer.php` (which holds the dark-mode script).
- `index.php` registers a player and `login.php` checks registration, both through `includes/players.php`. Players are stored in `data/contact-data.csv` at the repository root (outside the web root and ignored by git), with columns `FName,LName,Email`. Both pages store the email in `$_SESSION["email"]`.
- `index.php` and `login.php` redirect to `download.php`, which requires that session. On POST it calls `pointless_generate_zip()` in `includes/generate.php`. That function copies `puzzle-code/production/resources/` into a new temp directory, runs `production/src/main -e <email>` there, moves out `puzzle1.zip`, and deletes the temp directory. It never writes to `production/`, so it needs no lock.
- Token submission is only an idea (`ideas/tokens.md`).

## Non-production material

`ideas/` holds ideas, scrapped puzzles, and work in progress. Nothing there is built, tested, zipped, or served. Keep new ideas and unfinished work there, not in `puzzle-code/` or `web-server/`, and list them in `ideas/README.md`.

## Documentation

`README.md` is the main project documentation. When changing a puzzle's behavior or its adjustable `#define`s, update the matching "Current Puzzles" section in `README.md`. Record suspected mistakes and leftovers under "Questionable Items" in the README.
