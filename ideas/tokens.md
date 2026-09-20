# Tokens (idea, not implemented)

## The idea

Key component 4 of the project (see the README introduction) is giving players "multiple opportunities along the way to be recognized for their problem solving skills." The plan was:

1. Each puzzle hides a **token**, a short phrase the player can only find by working through it.
2. The player logs in on the website and submits tokens they have found.
3. The site records their progress (a "level") in the player data file (now `data/contact-data.csv`), which lets the School of Computing see how far each player got and follow up.

[tokens-webpage-uml.jpg](tokens-webpage-uml.jpg) shows this planned user flow (formerly `imgs/Pointless_UMLs-Webpage.jpg` in the README).

## What existed, and what was removed

On 2026-09-14:

- `Tokens.txt` (repository root) listed the tokens below. Nothing read it. **Removed**; the list is preserved here.
- `web-server/token-sub.php` was a form with a single "Token" field that posted to itself. It had no handling code. **Removed.**
- `web-server/login.php` redirected to `token-sub.php` after login. **Changed** to redirect to `download.php`.

On 2026-09-15:

- The player data file had a `Token` column, and `index.php` wrote `n\a` into it for every new player. **Removed**: new files have only `FName,LName,Email`. Older files that still have the column work unchanged.

## Token list (from Tokens.txt)

| # | Token |
|---|-------|
| 1 | brute force |
| 2 | color wheel |
| 3 | Lou's pixel art |
| 4 | speedy maze |
| 5 | alien lightbox |
| 6 | Dr. Pepper's code |
| 7 | master key |
| 8 | adding the colored squares |
| 9 | queue of logic gates |
| 10 | pointless journey |

The list has 10 entries, but the game now has 11 puzzles (see `puzzle-code/src/main.cpp`). The numbering follows an older puzzle order, and none of these phrases appear in any puzzle today.

## Open questions

- **Where do tokens appear?** In the generated `instructions.html`, in a hidden asset, or only after the puzzle is solved?
- **Fixed or per-player?** Fixed phrases like the ones above can be shared between players. Per-player tokens could be derived from the seed, as passwords are, but then the website would need to compute them (for example, by running the generator with `-a -s <seed>`).
- **What is a "level"?** The number of tokens submitted, or the highest puzzle reached?
- **Analytics.** GitHub issue #3 (closed 2026-09-19 and moved here) asked for participation statistics, so that the project's
  upkeep can be justified. Token submissions would be one source; the players file already gives registrations.
