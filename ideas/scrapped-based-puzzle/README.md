# Scrapped Based Rematch (not in the game)

This puzzle was removed from the game. The call to it in `puzzle-code/src/main.cpp` had been commented out, and it has no test. It was moved here on 2026-09-14 so that it no longer compiles with the generator.

- `scrapped-based-puzzle.cpp`: defines `Puzzle based_puzzle_create(long seed)` (the declaration was removed from `puzzle.h`)
- `files-scrapped-based/`: its resource directory (`.desc.txt`)

## Description

*Adjustable Variables*:
- `NUM_VALUES` (**must modify html elements to match**)
- `VAL_SZ`

This is a harder version of the *Based* puzzle that also involves sorting. The user sees an instructions page outlining the challenge. Three hidden buttons on the page, when clicked, list the made-up bases they will encounter in the actual puzzle.

The bases:
- *Symbolic 8*: `!, @, #, $, %, ^, &, *`
- *Glyphic 16*: ``{, }, [, ], (, ), `, >, a, b, c, d, e, f, g, h``
- *Mystic 36*: `|, ~, ,, /, ?, *, +, =, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z`

Clicking the "Enter" button takes them to the actual puzzle. There are `NUM_VALUES` values, each `VAL_SZ` characters long. Clicking a value places it into the container in the center of the page. The user must fill the container with the values sorted from smallest to largest. Doing this correctly shows a modal containing the key.

*RNG*:
- The values

## Restoring it

1. Move `scrapped-based-puzzle.cpp` back to `puzzle-code/src/` and `files-scrapped-based/` back to `puzzle-code/resources/`. The code uses the hardcoded path `../resources/files-scrapped-based/`.
2. Declare `Puzzle based_puzzle_create(long seed);` in `puzzle-code/src/include/puzzle.h`.
3. Add `based_puzzle_create(seed)` to the puzzle list in `puzzle-code/src/main.cpp`. It used to sit between `encrypt_puzzle_create` and `rematch_puzzle_create`.
4. Add a test (see "Implementing New Tests" in the main README).
