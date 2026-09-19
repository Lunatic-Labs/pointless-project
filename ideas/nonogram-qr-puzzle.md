# Nonogram / QR code puzzle (idea, not implemented)

A puzzle designed in 2022 and never built. The player solves a [nonogram](https://en.wikipedia.org/wiki/Nonogram)
whose picture is a QR code; scanning the finished code gives the answer (for example, a URL to the website).

Collected on 2026-09-19 from GitHub issues #50, #53, #54, #55, #58, and #59, which were then closed.

## Pieces

- **QR code generation** (#50). Generate the QR code in C++, or embed it as a base64 image. Must not use an online
  API: the game is played offline (see `CLAUDE.md`), so the code must be generated when the zip is built.
- **Short URL** (#59). The URL in the code should be at most 25 characters, so the QR code stays small enough to solve by hand.
- **Nonogram generation** (#53). Read the code's modules as a grid of black and white cells. Each row and column gets
  a header listing its runs: "2 1" means a run of 2 black cells, at least one white cell, then a run of 1. An HTML table may be enough to display it.
- **Data structure** (#54). Proposed interface:

  ```cpp
  Nonogram(QRCode code);                                 // Generate a nonogram from a QR code
  std::vector<std::list<int>> Nonogram::getColumnRuns(); // Runs for the column headers
  std::vector<std::list<int>> Nonogram::getRowRuns();    // Runs for the row headers
  std::map<std::pair<int, int>, bool> getHints();        // Cells revealed as hints
  ```

- **Sample puzzle** (#55). A page that explains nonograms with a small example to solve first. The issue has two photos of the intended layout:
  [1](https://user-images.githubusercontent.com/97480200/205157591-22ba7ca8-ab1f-48f8-911b-38fbe650ca8f.jpeg),
  [2](https://user-images.githubusercontent.com/97480200/205157665-a0d13473-1540-403c-85b3-7b4eea64048e.jpeg).

## Difficulty (#58)

Deciding whether a nonogram has a solution is NP-complete, and a QR code's nonogram may not be solvable by line
logic alone. The original team's plan was to solve each generated nonogram, count how many permutations were tried
before the right one was found, and reveal hint cells (`getHints()`) wherever that count got too high.

With this project's RNG rules, the hint cells must be chosen deterministically from the puzzle's seed (see `CLAUDE.md`).
