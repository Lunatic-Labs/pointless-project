# Cleanup and improvement tasks

What is left from a review of the whole repository on 2026-09-15. Most of that review's tasks have been done
(see git history); these are the rest. Remove an item from this file once it is done.

When doing these:
- Anything that adds, removes, or reorders an RNG roll changes the expected passwords in `puzzle-code/tests/` **and every player's puzzle**.
- When a fix changes a puzzle's behavior or `#define`s, update its "Current Puzzles" section in the top-level `README.md`.

## Do first

- [ ] **Rebuild the production generator.** `puzzle-code/production/` was built before the cleanup, so the site still serves the old puzzles
  (and the old, merging seeds). A person runs `make test`, then `make production`, in `puzzle-code/`.

## Deferred decisions

- [ ] **Should Logic Gate's memory come from the Binary Addition answer?** It is fixed (`MEMORY` in `logicgate-puzzle.cpp`). Binary Addition's answer is 8 bits, and the queue is 16.
  On 2026-09-19 this was left alone for now.

## Web server

- [ ] **The download limit is per session.** Each player's zip is now generated once and stored, so a client that drops its cookie can only
  start generations by registering new emails. If that becomes a problem, limit registrations by IP.
- [ ] **Investigate the zip-tool problems.** The zips were AES-256, which Windows' built-in zip support can't open; on 2026-09-19 they were switched to
  traditional PKWARE (ZipCrypto). Each zip still encrypts only the nested zip, not the puzzle's own files, which may explain the unexpected password prompts in the
  README "Issues". Test with the default tools on Windows, macOS, and Linux, and drop the site's "install 7-Zip" notice (`download.php`) if it is no longer needed.

## Generator

- [ ] **One copy of the bison.** `web-server/includes/bison.svg` and the bison in `templates/header.txt` are separate copies. Low priority: the website and the offline game can't share a file.
