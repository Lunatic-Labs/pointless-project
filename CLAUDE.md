# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

The Pointless Project is a puzzle challenge for pre-college students, inspired by the Synacor Challenge. A C++ generator builds a personalized, offline game: a chain of nested, password-protected zip files. Each layer holds one puzzle's `instructions.html`, and that puzzle's answer is the password to the next zip.

## Commands

The C++ code uses g++ with C++17 and needs `libzip` (`sudo apt install g++ libzip-dev php-cli`).

There is a single `puzzle-code/Makefile`; **run make from `puzzle-code/`**. Objects go in `puzzle-code/build/` (with `-MMD` header dependencies). **The binaries must run from their own directory** (`src/` or `tests/`), because all resource paths are relative to it, for example `../resources/files-math/.desc.html`.

Puzzle generator:
- `make` / `make build`: builds `src/main` (debug, `-O0 --coverage`) and creates `src/zipfiles/`
- `make run`: builds, runs `cleanzip`, then runs `./main` in `src/`
- `./main -s <seed>` (in `src/`): uses a fixed seed (otherwise a random seed, which is printed)
- The first line of output is always `Seed: <seed>` (the website parses it)
- `./main -a` (in `src/`): answers only; prints each puzzle's token and password (and extra info, such as the rematch passwords) without writing zips or generated files
- `./main -j` (in `src/`): prints the answers as one line of JSON and no `Seed:` line, so the whole of stdout parses; the website runs the generator this way
- `make cleanzip`: removes generated zips and generated files under `resources/` and `tests/zipfiles/`; `make clean` also removes `build/` and both binaries (not `production/`)
- `make coverage`: runs gcov on `src/*.cpp` (requires `make run` or `make test` to have run first)
- `make production`: builds an optimized generator without coverage (objects in `build/production/`) and replaces `puzzle-code/production/` with `src/main`, an empty `src/zipfiles/`, and a clean copy of `resources/`. Deploying does not need it: `bin/deploy pointless` runs `make production` on the server, so the live site is built there. The local tree only serves downloads from a local `php -S` and `test_generate_real_generator`, which fails when that tree is stale.

Tests:
- `make test`: builds `tests/main` from `tests/*.cpp` plus every `src/` object except `src/main.o`, then runs it in `tests/`. Exits nonzero if any test fails.
- `make test T=bst` (or `./main bst` in `tests/`): runs only the tests whose names contain `bst`.
- `make test` runs `cleanzip` first, so a test run never picks up files an earlier run left in `resources/`.
- `make test-web` runs `make production` and then the PHP tests against that fresh tree (`T=` filters them too);
  `make test-all` runs both suites in order, which is what CI does. Do not run either while a local `php -S` is
  serving downloads, since `make production` deletes and recreates `production/`.
- Tests are functions listed in the `tests` array in `tests/main.cpp` and declared in `tests/include/test.h`. They use `CHECK`, `CHECK_EQ`, `CHECK_THROWS`, and `CHECK_PUZZLE` (from `test.h`), which record a failure and keep going; don't use `assert`.
- Tests run with `FLAGS |= ANS_ONLY`, so they don't write files, except `game_zipfiles_test`, which clears it to write a real game (into `../resources/` and `tests/zipfiles/`) and then unlocks every layer, following the BST signs like a player.
- `seeds_test` checks that 200 players all get answers, different games, and no repeated token. Pinned values (`utils_test`, and the expected passwords) must be updated whenever the RNG or a puzzle's rolls change.

PHP site (`web-server/`):
- `php -S localhost:8000`. Local downloads need `make production` to have been run. The site never runs make. Set `POINTLESS_GENERATOR_DIR` to use a production tree elsewhere, and `POINTLESS_PLAYERS_FILE` to move the player data.
- Tests: `php web-server/tests/run.php [name-substring]` (runs from any directory). It starts its own `php -S` on a free port with a temp players file (and games directory next to it), a fake generator (`fake_generator($mode)` in `tests/lib.php`), temp sessions, and `TMPDIR`, resets that state before each test, and exits 1 on failure. It never touches `data/` or `production/`. Tests are `test_*` functions in `tests/*-test.php` using `check()` (not `assert()`, which php-cli disables), listed in `$tests` in `tests/run.php`. `test_generate_real_generator` runs `production/src/main` (or `POINTLESS_TEST_GENERATOR_DIR`): skipped without `src/main`, fails if that tree is broken.

CI (`.github/workflows/test.yml`) runs `make test`, `make production` (in the CI checkout), and the PHP tests.

## Architecture

**Puzzle pipeline.** `src/main.cpp` parses options and calls `src/game.cpp`: `game_create_puzzles(seed)` builds a `std::vector<Puzzle>` by calling each `<name>_puzzle_create(utils_derive_seed(seed, "<name>"))` in play order, `game_print_passwords` prints them, and `game_write_zipfiles` walks the list in reverse. `zipfiles/puzzleN.zip` contains puzzle N's files and, except for the last puzzle, `puzzle{N+1}.zip`, every entry encrypted with puzzle N-1's password (`puzzle1.zip` is not encrypted). A zip never mixes plain and encrypted entries, since macOS Archive Utility asks for one password per zip before extracting anything; `utils_zip_files` encrypts all entries or none. `puzzle1.zip` is the full game; the inner zips are also left on disk for easier testing. Errors are `std::runtime_error`s, caught in `main`.

**A puzzle** has two parts:
1. A resource directory `puzzle-code/resources/files-<name>/` containing `.desc.html`, whose first line is `%TITLE <page title>` and whose remaining lines are an HTML/JS body with `%PARAM` placeholders, plus any assets. The whole page, title included, is written here and not in the `.cpp`. Files and directories starting with `.` are excluded from the zip (see `utils_walkdir`), so the leading dot must stay. Resources must not load anything from the internet, because the game is played offline.
2. `src/<name>-puzzle.cpp`, which rolls random values, calls `utils_html_printf(desc_path, {args...}, token, inventory)` (which throws without the `%TITLE` line, or unless there is exactly one arg per `%PARAM`) to build the page: `resources/templates/header.html` with the title substituted into its `%TITLE` placeholder (the `<title>` element, so each page's tab names itself; escaped there, unlike the `<h2>`), a `.layout` div holding two columns — a `<section>` (not a `<div>`: several descriptions style `.container div`) with the title in an `<h2>` followed by the substituted body, and the sidebar — and `footer.html`. The header opens `<body>` and two divs and the footer closes them, so a `.desc.html`'s body must be a balanced fragment. The puzzle writes `instructions.html` with `utils_generate_file` and returns `Puzzle{contents_fp, contents_html, password, extra_info}`.
Every main puzzle page also carries a boxed sidebar on the right (`utils_html_printf` builds it, styled by `.container .sidebar`): the page's token at the top, then `Points: 0`, then an inventory of the useless items awarded so far, built by `inventory_html("<name>")` from the `ITEMS` table in `src/inventory.cpp` (in play order, kept in step with `game_create_puzzles` by hand; it throws for a puzzle that has no row). An item is awarded for *solving* its puzzle, so a page lists only the items of the puzzles before it: the first page has no inventory, and the last row's item is never listed. The rematch sub-puzzles pass no token and get no sidebar at all. `extra_info` is only printed for debugging. Keep helper functions `static`.

Pages are left aligned (so is `web-server/includes/styles.css`, which mirrors the template's `.container`): don't center text or re-center figures in a `.desc.html`. Use the template's `.callout` (dark box of facts), `.figure` (a maze, table, or SVG on its own line), and `.question` (the closing question) instead of repeating layout CSS. A `.desc.html`'s `<style>` applies to the whole page, so scope every selector to the puzzle's own classes; bare `body`, `svg`, or `table` rules leak into the header.

To add a puzzle:
1. Create the resource directory and the `.cpp` file.
2. Declare `Puzzle <name>_puzzle_create(seed_t)` in `src/include/puzzle.h` and insert the call into `game_create_puzzles` in `src/game.cpp`.
   Add a row for it in the same position in `ITEMS` in `src/inventory.cpp`.
3. Add `tests/<name>-puzzle-test.cpp` (usually one `CHECK_PUZZLE`), declare it in `tests/include/test.h`, list it in `tests/main.cpp`, and add the puzzle to `min_different` in `tests/seeds-test.cpp`.

The Makefile uses wildcards, so no build changes are needed. If the puzzle writes new generated files into `resources/`, add them to `clean-generated` in the Makefile and to `.gitignore`.

**Rematch puzzle.** `rematch-puzzle.cpp` is a composite. It creates the maze, encrypt, and based rematch sub-puzzles (each with its own derived seed), zips each into `resources/files-rematch/rematchN.zip` (not encrypted) along with `passwordN.zip`, which holds `passwordN.txt` (a 3-digit number) encrypted with that sub-puzzle's password, and returns the numbers concatenated in order as the password for the next main layer. Its `extra_info` lists the sub-puzzles' passwords.

**Shared code.** `src/maze.{h,cpp}` generates the mazes for both maze puzzles. `src/graphics.{h,cpp}` provides `Image`/`Pixel` and an `Svg` builder with `Rect`/`Circle` shapes; puzzles embed the SVG strings in their HTML.

**Determinism and RNG.** `seed_t` is `uint64_t`. `utils_rng_roll(min, max, seed_t &seed)` is splitmix64: it advances the seed and gives the same numbers on every platform (don't use `rand()`, `std::shuffle`, or `<random>` distributions; use `utils_rng_roll`, `utils_chance`, and `utils_shuffle`). Within a puzzle, adding, removing, or reordering rolls changes every later value, and tests assert exact passwords for specific seeds (for example `math_puzzle_create(1)` → `"99"`). A player's seed is random, chosen by the website at registration and stored in the players file, so an RNG change alters the puzzle of every player who hasn't downloaded yet. Downloaded games are stored, so later RNG changes don't affect them.

**Global flags.** `extern uint32_t FLAGS` (defined in each `main.cpp`) has one bit, `ANS_ONLY` (in `utils.h`), which makes `utils_generate_file`, `utils_mkdir`, and `utils_remove_all` no-ops.

**Web.** `web-server/` is plain PHP with no framework or database.
- Pages share `includes/header.php` and `includes/footer.php` (which holds the dark-mode script, also in `resources/templates/footer.html`) and `includes/styles.css`.
- `index.php` is the only way in: a known email signs that player back in, any other valid email registers a new player (through `includes/players.php`). It takes no tokens. First and last name are optional and only stored at registration; a returning player's are ignored, since the players file is append-only. Emails are normalized (trimmed, lowercased) before validation, storage, and lookup; names are stored as typed (except that `pointless_safe_name()` puts `'` before a leading `=`, `+`, `-`, `@`, tab, or CR, so spreadsheets don't run them as formulas) and escaped only on output. Players are stored in `data/contact-data.csv` at the repository root (outside the web root and ignored by git), with columns `FName,LName,Email,Seed`. `Seed` is random at registration and never changes. Signing in regenerates the session id and stores the email in `$_SESSION["email"]`. Every page starts its session through `includes/session.php` (cookie `pointless`, not `PHPSESSID`, because the site shares its host).
- `index.php` redirects to `download.php`, which requires that session. A POST with a `token` field is a token submission (`pointless_submit_token_throttled()`, throttled by `POINTLESS_TOKEN_INTERVAL` in `includes/events.php`); any other POST is a download (at most once per `POINTLESS_DOWNLOAD_INTERVAL` seconds per session) and streams the file returned by `pointless_player_zip()` in `includes/generate.php`: the player's permanent zip, `games/<seed>.zip` in the directory holding the players file (`data/games/`), with an answer key, `<seed>.json` (name, email, seed, date, and every puzzle's password and token). Only the first download generates it, through `pointless_generate_zip($args)`, which copies `puzzle-code/production/resources/` into a new temp directory, runs `production/src/main -s <seed> -j` there, moves out `puzzle1.zip`, and deletes the temp directory. It never writes to `production/`, so it needs no lock; the stored files are written by copy and rename, so simultaneous first downloads are safe. Never delete or change a stored zip in code: re-downloads must give the same file.
- By decision, signing in needs only an email and the forms have no CSRF tokens (see README "Web Server").
- Every puzzle page shows a token (`utils_token()`, eight characters from `TOKEN_ALPHABET`), rolled from a seed derived with the name `"token"` so it costs the puzzle no rolls. `utils_html_printf` renders it at the top of the sidebar; the three rematch sub-puzzles pass `""` and get none. The pages and the website call it a "proof of progress"; the code keeps the name `token`.
- Tokens are **frozen** in `games/<seed>.json` when a player's game is generated, and never recomputed: a recomputed token would disagree with the zip the player already holds. `download.php` takes submissions through `includes/events.php`, which appends `data/events.csv` (`Time,Event,Email,Detail,Level,Platform`, append-only; `Platform` is guessed from the request's headers by `pointless_platform()`) and computes the level as the highest accepted token index (so the first page's token counts).
- Progress is shown as an unannotated health bar (`includes/progress.php`): never a count of puzzles solved or a total, in the bar, the messages, or share posts. Once any token is accepted, `download.php` puts its progress section (bar, token form, and that form's messages) above the download. Share posts link to the player's public `share.php?p=<share id>` (name, health bar, `og:` tags; no session). The share id is an HMAC of the seed, since the seed itself must never be shown.
- `health.php` is for the deploy, not players: it answers `ok` (else 503 with reasons, no paths) only if `POINTLESS_PLAYERS_FILE` and `POINTLESS_GENERATOR_DIR` are set and usable.
- Deployment (README "Deployment") is `bin/deploy pointless` on `tools.lipscomb-soc.org`, defined in the `dtowell/tools` repo: it builds on the server (Ubuntu 22.04, hence the 22.04 CI job), gates on `make test` and the PHP tests, and serves the site under `/pointless/`, so page links must stay relative.

## Non-production material

`ideas/` holds ideas, scrapped puzzles, and work in progress. Nothing there is built, tested, zipped, or served. Keep new ideas and unfinished work there, not in `puzzle-code/` or `web-server/`, and list them in `ideas/README.md`. `ideas/cleanup-tasks.md` is the backlog of cleanup work.

## Documentation

`README.md` is the main project documentation. When changing a puzzle's behavior or its adjustable `#define`s, update the matching "Current Puzzles" section in `README.md`. Record suspected mistakes and leftovers under "Issues" in the README.
