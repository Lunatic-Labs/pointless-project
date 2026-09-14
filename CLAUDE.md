# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

The Pointless Project is a puzzle challenge for pre-college students, inspired by the Synacor Challenge. A C++ generator builds a personalized, offline game: a chain of nested, password-protected zip files. Each layer holds one puzzle's `instructions.html`, and that puzzle's answer is the password to the next zip.

## Commands

The C++ code uses g++ with C++17 and needs `libzip` (`sudo apt install g++ libboost-all-dev libzip-dev php-cli gcovr`).

**Always run make and the binaries from inside their own directory.** All resource paths are hardcoded relative to `puzzle-code/src/` or `puzzle-code/tests/`, for example `../html-txt/files-math/.desc.txt`.

Puzzle generator (`puzzle-code/src/`):
- `make`: builds `./main` (objects go in `obj/`, and a `zipfiles/` directory is created)
- `make all`: builds, runs `cleanzip`, then runs `./main`
- `./main -s <seed>`: uses a fixed, nonzero seed (otherwise the seed comes from the current time)
- `./main -a`: answers only; prints each puzzle's password without writing zips or generated files
- `make cleanzip`: removes generated zips and generated files under `html-txt/`; `make clean` also removes objects and the binary
- `make coverage`: runs gcov (requires `./main` to have run at least once)

Tests (`puzzle-code/tests/`):
- `make all`: cleans, rebuilds (compiling `../src/*.cpp` together with the tests), and runs `./main`
- There is no test runner or filter. Tests are `assert`-based functions listed in `tests/main.cpp`. To run a single test, temporarily comment out the other entries in the `tests` vector.
- Tests run with `FLAGS |= ANS_ONLY`, so they don't write zips.

PHP site (`web-server/`):
- `php -S localhost:8000`. Downloads run the generator, so build `puzzle-code/src/main` first (`make` must be installed). Set `POINTLESS_SRC_DIR` to use a generator in a different directory.
- Integration tests are bash + wget scripts. Run them from `web-server/integrated-tests/` (they `cd ..`), for example `./test-index-GET.sh`. Tests that modify `includes/contact-data.csv` must back it up and restore it.
## Architecture

**Puzzle pipeline.** `src/main.cpp` builds a `std::vector<Puzzle>` by calling each `<name>_puzzle_create(seed)` in play order. It prints every password, then `create_nested_zipfiles` walks the list in reverse. `zipfiles/puzzleN.zip` contains puzzle N's files, is encrypted with puzzle N's password, and (except for the last puzzle) contains `puzzle{N+1}.zip`. `puzzle1.zip` is the full game; the inner zips are also left on disk for easier testing.

**A puzzle** has two parts:
1. A resource directory `puzzle-code/html-txt/files-<name>/` containing `.desc.txt`, an HTML/JS body with `%DELIM` placeholders, plus any assets. Files and directories starting with `.` are excluded from the zip (see `utils_walkdir`).
2. `src/<name>-puzzle.cpp`, which rolls random values, calls `utils_html_printf(title, desc_path, {args...})` to substitute `%DELIM` in order and wrap the body with `html-txt/resources/header.txt` and `footer.txt`, writes `instructions.html` with `utils_generate_file`, and returns `Puzzle{contents_fp, contents_html, password, extra_info}`. `extra_info` is only printed for debugging.

To add a puzzle:
1. Create the resource directory and the `.cpp` file.
2. Declare `Puzzle <name>_puzzle_create(long)` in `src/include/puzzle.h` and insert the call into the vector in `main.cpp`.
3. Add `tests/<name>-puzzle-test.cpp`, declare it in `tests/include/test.h`, and list it in `tests/main.cpp`.

The Makefiles use wildcards, so no build changes are needed.

**Rematch puzzle.** `rematch-puzzle.cpp` is a composite. It creates the maze, encrypt, and based rematch sub-puzzles, zips each into `html-txt/files-rematch/rematchN.zip` along with a generated password fragment (`.passwords/passwordN.txt`), and returns the concatenated fragments as the password for the next main layer.

**Determinism and RNG.** `utils_rng_roll(min, max, long &seed)` reseeds `srand` and mutates the seed on every call. Within a puzzle, adding, removing, or reordering rolls changes every later value. Tests assert exact passwords for specific seeds (for example `math_puzzle_create(1)` → `"75"`), so RNG changes require updating the expected values. The seed a player gets is derived from their email (`seed_gen` in `tests/file.cpp`, mirrored by `pointless_seed` in `web-server/includes/generate.php`; keep them in sync).

**Global flags.** `extern uint32_t FLAGS` (defined in each `main.cpp`) takes bits from `utils.h`: `ANS_ONLY`, `SET_SEED`, `NO_HDR`, `NO_FTR`, and `BISON_GRID`. Some utils, such as `utils_mkdir`, become no-ops under `ANS_ONLY`.

**Graphics.** `src/graphics.{h,cpp}` provides `Image`/`Pixel` and an `Svg` builder with `Rect`/`Circle` shapes. Puzzles embed the SVG strings in their HTML. The project has moved from PPM output to SVG.

**Web.** `web-server/` is plain PHP with no framework or database.
- `index.php` registers a player by appending to `includes/contact-data.csv`. `login.php` checks that CSV. Both store the email in `$_SESSION["email"]`.
- `download.php` requires that session. On POST it calls `pointless_generate_zip()` in `includes/generate.php`, which takes a lock (the generator writes to shared `zipfiles/` and `html-txt/` paths), runs `make cleanzip` and `./main -s <seed>` in `puzzle-code/src`, cleans up again, and streams a temporary copy of `puzzle1.zip`.
- `token-sub.php` is only a form so far. `Tokens.txt` lists the per-puzzle tokens.

## Documentation

`README.md` is the only project documentation. When changing a puzzle's behavior or its adjustable `#define`s, update the matching "Current Puzzles" section in `README.md`.
