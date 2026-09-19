# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

The Pointless Project is a puzzle challenge for pre-college students, inspired by the Synacor Challenge. A C++ generator builds a personalized, offline game: a chain of nested, password-protected zip files. Each layer holds one puzzle's `instructions.html`, and that puzzle's answer is the password to the next zip.

## Commands

The C++ code uses g++ with C++17 and needs `libzip` (`sudo apt install g++ libzip-dev php-cli`).

There is a single `puzzle-code/Makefile`; **run make from `puzzle-code/`**. Objects go in `puzzle-code/build/` (with `-MMD` header dependencies). **The binaries must run from their own directory** (`src/` or `tests/`), because all resource paths are relative to it, for example `../resources/files-math/.desc.txt`.

Puzzle generator:
- `make` / `make build`: builds `src/main` (debug, `-O0 --coverage`) and creates `src/zipfiles/`
- `make run`: builds, runs `cleanzip`, then runs `./main` in `src/`
- `./main -s <seed>` (in `src/`): uses a fixed seed (otherwise a random seed, which is printed)
- `./main -e <email>` (in `src/`): uses the player's seed from `utils_seed_from_email()`; this is what the website runs
- `./main -a` (in `src/`): answers only; prints each puzzle's password (and extra info, such as the rematch passwords) without writing zips or generated files
- `make cleanzip`: removes generated zips and generated files under `resources/` and `tests/zipfiles/`; `make clean` also removes `build/` and both binaries (not `production/`)
- `make coverage`: runs gcov on `src/*.cpp` (requires `make run` or `make test` to have run first)
- `make production`: builds an optimized generator without coverage (objects in `build/production/`) and replaces `puzzle-code/production/` with `src/main`, an empty `src/zipfiles/`, and a clean copy of `resources/`. Only a person runs this; don't run it as a side effect of other work, since it changes what the live site serves.

Tests:
- `make test`: builds `tests/main` from `tests/*.cpp` plus every `src/` object except `src/main.o`, then runs it in `tests/`. Exits nonzero if any test fails.
- `make test T=bst` (or `./main bst` in `tests/`): runs only the tests whose names contain `bst`.
- Tests are functions listed in the `tests` array in `tests/main.cpp` and declared in `tests/include/test.h`. They use `CHECK`, `CHECK_EQ`, `CHECK_THROWS`, and `CHECK_PUZZLE` (from `test.h`), which record a failure and keep going; don't use `assert`.
- Tests run with `FLAGS |= ANS_ONLY`, so they don't write files, except `game_zipfiles_test`, which clears it to write a real game (into `../resources/` and `tests/zipfiles/`) and then unlocks every layer, following the BST signs like a player.
- `seeds_test` checks that 200 players all get answers and different games. Pinned values (`utils_test`, and the expected passwords) must be updated whenever the RNG or a puzzle's rolls change.

PHP site (`web-server/`):
- `php -S localhost:8000`. Downloads need `make production` to have been run. The site never runs make. Set `POINTLESS_GENERATOR_DIR` to use a production tree elsewhere, and `POINTLESS_PLAYERS_FILE` to move the player data.
- Tests: `php web-server/tests/run.php [name-substring]` (runs from any directory). It starts its own `php -S` on a free port with a temp players file, a fake generator (`fake_generator($mode)` in `tests/lib.php`), temp sessions, and `TMPDIR`, resets that state before each test, and exits 1 on failure. It never touches `data/` or `production/`. Tests are `test_*` functions in `tests/*-test.php` using `check()` (not `assert()`, which php-cli disables), listed in `$tests` in `tests/run.php`. `test_generate_real_generator` runs `production/src/main` (or `POINTLESS_TEST_GENERATOR_DIR`): skipped without `src/main`, fails if that tree is broken.

CI (`.github/workflows/test.yml`) runs `make test`, `make production` (in the CI checkout), and the PHP tests.

## Architecture

**Puzzle pipeline.** `src/main.cpp` parses options and calls `src/game.cpp`: `game_create_puzzles(seed)` builds a `std::vector<Puzzle>` by calling each `<name>_puzzle_create(utils_derive_seed(seed, "<name>"))` in play order, `game_print_passwords` prints them, and `game_write_zipfiles` walks the list in reverse. `zipfiles/puzzleN.zip` contains puzzle N's files (unencrypted) and, except for the last puzzle, `puzzle{N+1}.zip`, encrypted with puzzle N's password. `puzzle1.zip` is the full game; the inner zips are also left on disk for easier testing. Errors are `std::runtime_error`s, caught in `main`.

**A puzzle** has two parts:
1. A resource directory `puzzle-code/resources/files-<name>/` containing `.desc.txt`, an HTML/JS body with `%DELIM` placeholders, plus any assets. Files and directories starting with `.` are excluded from the zip (see `utils_walkdir`). Resources must not load anything from the internet, because the game is played offline.
2. `src/<name>-puzzle.cpp`, which rolls random values, calls `utils_html_printf(title, desc_path, {args...})` (which throws unless there is exactly one arg per `%DELIM`) to build the page: `resources/templates/header.txt`, an optional extra head string, the title, the substituted body in a `<section>` (not `<div>`: several descriptions style `.container div`), and `footer.txt`. The header opens `<body>` and two divs and the footer closes them, so a `.desc.txt` must be a balanced fragment. The puzzle writes `instructions.html` with `utils_generate_file` and returns `Puzzle{contents_fp, contents_html, password, extra_info}`. `extra_info` is only printed for debugging. Keep helper functions `static`.

To add a puzzle:
1. Create the resource directory and the `.cpp` file.
2. Declare `Puzzle <name>_puzzle_create(seed_t)` in `src/include/puzzle.h` and insert the call into `game_create_puzzles` in `src/game.cpp`.
3. Add `tests/<name>-puzzle-test.cpp` (usually one `CHECK_PUZZLE`), declare it in `tests/include/test.h`, list it in `tests/main.cpp`, and add the puzzle to `min_different` in `tests/seeds-test.cpp`.

The Makefile uses wildcards, so no build changes are needed. If the puzzle writes new generated files into `resources/`, add them to `clean-generated` in the Makefile and to `.gitignore`.

**Rematch puzzle.** `rematch-puzzle.cpp` is a composite. It creates the maze, encrypt, and based rematch sub-puzzles (each with its own derived seed), zips each into `resources/files-rematch/rematchN.zip` along with an encrypted `passwordN.txt` holding a 3-digit number, and returns the numbers concatenated in order as the password for the next main layer. Its `extra_info` lists the sub-puzzles' passwords.

**Shared code.** `src/maze.{h,cpp}` generates the mazes for both maze puzzles. `src/graphics.{h,cpp}` provides `Image`/`Pixel` and an `Svg` builder with `Rect`/`Circle` shapes; puzzles embed the SVG strings in their HTML.

**Determinism and RNG.** `seed_t` is `uint64_t`. `utils_rng_roll(min, max, seed_t &seed)` is splitmix64: it advances the seed and gives the same numbers on every platform (don't use `rand()`, `std::shuffle`, or `<random>` distributions; use `utils_rng_roll`, `utils_chance`, and `utils_shuffle`). Within a puzzle, adding, removing, or reordering rolls changes every later value, and tests assert exact passwords for specific seeds (for example `math_puzzle_create(1)` → `"99"`). A player's seed comes from their email via `utils_seed_from_email()` in `src/utils.cpp` (FNV-1a of the trimmed, lowercased email), the only copy of the formula; the website normalizes emails the same way (`pointless_normalize_email()`). Changing either changes every existing player's puzzle.

**Global flags.** `extern uint32_t FLAGS` (defined in each `main.cpp`) has one bit, `ANS_ONLY` (in `utils.h`), which makes `utils_generate_file`, `utils_mkdir`, and `utils_remove_all` no-ops.

**Web.** `web-server/` is plain PHP with no framework or database.
- Pages share `includes/header.php` and `includes/footer.php` (which holds the dark-mode script, also in `resources/templates/footer.txt`) and `includes/styles.css`.
- `index.php` registers a player and `login.php` checks registration, both through `includes/players.php`. Emails are normalized (trimmed, lowercased) before validation, storage, and lookup; names are stored as typed (except that `pointless_safe_name()` puts `'` before a leading `=`, `+`, `-`, `@`, tab, or CR, so spreadsheets don't run them as formulas) and escaped only on output. Players are stored in `data/contact-data.csv` at the repository root (outside the web root and ignored by git), with columns `FName,LName,Email`. Both pages regenerate the session id and store the email in `$_SESSION["email"]`.
- `index.php` and `login.php` redirect to `download.php`, which requires that session. On POST (at most once per `POINTLESS_DOWNLOAD_INTERVAL` seconds per session) it calls `pointless_generate_zip()` in `includes/generate.php`. That function copies `puzzle-code/production/resources/` into a new temp directory, runs `production/src/main -e <email>` there, moves out `puzzle1.zip`, and deletes the temp directory. It never writes to `production/`, so it needs no lock. `download.php` opens and deletes that zip before streaming it, so a canceled download leaves nothing in `TMPDIR`.
- By decision, login needs only an email and the forms have no CSRF tokens (see README "Web Server").
- Token submission is only an idea (`ideas/tokens.md`).

## Non-production material

`ideas/` holds ideas, scrapped puzzles, and work in progress. Nothing there is built, tested, zipped, or served. Keep new ideas and unfinished work there, not in `puzzle-code/` or `web-server/`, and list them in `ideas/README.md`. `ideas/cleanup-tasks.md` is the backlog of cleanup work.

## Documentation

`README.md` is the main project documentation. When changing a puzzle's behavior or its adjustable `#define`s, update the matching "Current Puzzles" section in `README.md`. Record suspected mistakes and leftovers under "Issues" in the README.
