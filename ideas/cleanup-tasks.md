# Cleanup and improvement tasks

What is left from a review of the whole repository on 2026-09-15. Most of that review's tasks have been done
(see git history); these are the rest. Remove an item from this file once it is done.

When doing these:
- Anything that adds, removes, or reorders an RNG roll changes the expected passwords in `puzzle-code/tests/` **and every player's puzzle**.
- When a fix changes a puzzle's behavior or `#define`s, update its "Current Puzzles" section in the top-level `README.md`.

## Do first

- [ ] **Rebuild the production generator.** `puzzle-code/production/` was built before the cleanup, so the site still serves the old puzzles
  (and the old, merging seeds). A person runs `make test`, then `make production`, in `puzzle-code/`.

## Decisions to make

- [ ] **Is an email-only login acceptable?** Anyone who knows a registered email can download that player's puzzle. That may be fine
  (the puzzle isn't secret), but record the decision in the README.
- [ ] **CSRF tokens.** The forms have none. The worst case seems to be logging someone into another player's download page; decide whether that matters.
- [ ] **Should Logic Gate's memory come from the Binary Addition answer?** It is fixed (`MEMORY` in `logicgate-puzzle.cpp`). Binary Addition's answer is 8 bits, and the queue is 16.
- [ ] **Review the contact list.** The README "Contact" section publishes personal emails of former students. Check that they still want to be listed.

## Web server

- [ ] **The download limit is per session.** A client that drops its cookie can start as many generations as it likes. If that becomes a problem, limit by IP or cache each player's zip.
- [ ] **Spreadsheet formulas in the players file.** Names are stored as typed, so a name starting with `=`, `+`, `-`, or `@` could run as a formula
  when `data/contact-data.csv` is opened in a spreadsheet. Either prefix such names with `'` or warn whoever opens the file.
- [ ] **Investigate the zip-tool problems.** Each zip is AES-256 and encrypts only the nested zip, not the puzzle's own files. This is probably why the site
  tells players to install 7-Zip, and why the README "Issues" mention unexpected password prompts. Test with the default tools on Windows, macOS, and Linux.

## Generator

- [ ] **One copy of the bison.** `web-server/includes/bison.svg` and the bison in `templates/header.txt` are separate copies. Low priority: the website and the offline game can't share a file.
- [ ] **Old coverage data.** After large changes, `make test` prints `libgcov profiling error: ... different checksum` until `make clean`. Consider deleting `build/**/*.gcda` whenever objects are rebuilt.
