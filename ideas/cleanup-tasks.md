# Cleanup and improvement tasks

What is left from a review of the whole repository on 2026-09-15. Most of that review's tasks have been done
(see git history); these are the rest. Remove an item from this file once it is done.

When doing these:
- Anything that adds, removes, or reorders an RNG roll changes the expected passwords in `puzzle-code/tests/` **and every player's puzzle**.
- When a fix changes a puzzle's behavior or `#define`s, update its "Current Puzzles" section in the top-level `README.md`.

## Deferred decisions

- [ ] **Should Logic Gate's memory come from the Binary Addition answer?** It is fixed (`MEMORY` in `logicgate-puzzle.cpp`). Binary Addition's answer is 8 bits, and the queue is 16.
  On 2026-09-19 this was left alone for now.

## Puzzle text

From the review of the puzzle pages on 2026-09-19. The layout and typo fixes from that review are done; these change
what a page tells the player, so they are decisions, not cleanups. Each one is described under "Issues" in the top-level
`README.md`.

- [ ] **Fix the *Logic Gate* page's rules.** Say that the answer is every result in order, that the puzzle ends when one
  circle is left, that the queue starts at the right, and that red is 0 and green is 1. Decide first how much of this
  should stay a discovery: the password's shape is already familiar from *Binary Addition*.
- [ ] **Make the maze colors and their names agree.** Either recolor the tiles or rename them in the text
  (*Maze*: "golden"/"purple" for yellow/magenta; *Maze Rematch*: "Purple" for its magenta start tile).
- [ ] **Help players find the browser console** in *Maze Rematch*, for example "press F12", without giving away that the
  console is where the puzzle lives.
- [ ] **Add a worked row to *Based Rematch*,** showing one row converted, summed, and reduced to its least significant bit.
- [ ] **Fix the banner title in dark mode.** `.title` is dark purple on the dark background, in `templates/header.html` and
  `web-server/includes/styles.css` both.

## Web server

- [ ] **The download limit is per session.** Each player's zip is now generated once and stored, so a client that drops its cookie can only
  start generations by registering new emails. If that becomes a problem, limit registrations by IP.
- [ ] **Retest the zips with the default tools.** The zips were AES-256, which Windows' built-in zip support can't open; on 2026-09-19 they were switched to
  traditional PKWARE (ZipCrypto). On 2026-09-22 each zip became locked as a whole by the previous answer (it had encrypted only the nested zip, which made
  macOS Archive Utility ask for a password before the player could read the puzzle). Test with the default tools on Windows, macOS, and Linux; on macOS, check
  what Archive Utility's "Keep expanding if possible" does with the nested zip (README "Issues").

## Generator

- [ ] **One copy of the bison.** `web-server/includes/bison.svg` and the bison in `templates/header.html` are separate copies. Low priority: the website and the offline game can't share a file.
