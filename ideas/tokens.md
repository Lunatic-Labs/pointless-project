# Tokens (plan, not implemented)

## The idea

Key component 4 of the project (see the README introduction) is giving players "multiple opportunities along the way to be recognized for their problem solving skills." The plan is:

1. Each puzzle page shows a **token**, a short phrase, on its `instructions.html`.
2. The player logs in on the website and submits tokens as they find them.
3. The site records each submission and shows the player their **level**, so the School of Computing can see how far each player got and follow up.

[tokens-webpage-uml.jpg](tokens-webpage-uml.jpg) shows the planned user flow (formerly `imgs/Pointless_UMLs-Webpage.jpg` in the README).

## Design (decided 2026-09-19)

### Tokens don't need hiding: the zip nesting is the proof

`puzzleN.zip` holds puzzle N's files unencrypted, plus `puzzleN+1.zip` encrypted with puzzle N's
password (see `game_write_zipfiles` in `puzzle-code/src/game.cpp`). So a token printed plainly on
puzzle N's page is already unforgeable evidence: to read it the player had to extract `puzzleN.zip`,
which took puzzle N-1's answer.

That means no hidden assets and no "reveal after solving" mechanism. The token goes in the generated
HTML, visibly, with a line telling the player to submit it on the website.

Consequences:

- **Token 1 proves nothing.** It ships inside the download. Tokens 2-11 are the meaningful ones.
- **Token N proves puzzles 1 through N-1 were solved**, not N.
- **Token 11 is the completion proof.** The last layer, `files-fin`, is a congratulations page rather
  than a puzzle: `fin_puzzle_create()` returns an empty password, because nothing follows it to
  encrypt. Reaching that layer means solving `bst`, the tenth and last real puzzle. So no twelfth
  layer is needed, and `fin`'s empty password is never used as a token.

This makes the arithmetic clean: there are 11 layers but 10 puzzles with passwords, so

> **level = the number of puzzles solved = (highest accepted token index) - 1**, from 0 to 10.

### Tokens are derived per puzzle, without disturbing the RNG

`utils_derive_seed(seed, name)` takes the seed **by value** and returns a new one
(`puzzle-code/src/utils.cpp`), so deriving a token inside a puzzle does not advance that puzzle's
seed. Passwords do not change, and the pinned expectations in `tests/` and `seeds-test.cpp` stay
valid.

Add a helper to `utils.{h,cpp}`:

```cpp
// Returns a random phrase for a puzzle page, rolled from resources/templates/words.txt.
std::string utils_token(seed_t seed);
```

Each puzzle calls it once, on a derived seed:

```cpp
const std::string token = utils_token(utils_derive_seed(seed, "token"));
```

The word list is a new resource, `resources/templates/words.txt`. `templates/` is not inside any
`files-<name>/` directory, so it is never zipped into the game.

### Tokens reach the page through `utils_html_printf`

Preferred: give `utils_html_printf` a new defaulted parameter for the token, and have it render a
standard block (its own class, styled once in `resources/templates/header.txt`) just before the
footer. Then:

- No `.desc.txt` changes at all, for any of the 11 puzzles.
- Every page presents the token the same way, styled in one place.
- Each `<name>-puzzle.cpp` grows by two lines: derive it, pass it.
- `<name>_puzzle_create(seed_t)` keeps its signature, so `CHECK_PUZZLE` and
  `tests/include/test.h` need no changes.

Alternative, if a puzzle ever wants the token woven into its prose: a `%DELIM` in that puzzle's
`.desc.txt` and an extra arg. `utils_html_printf` already throws unless there is exactly one arg per
`%DELIM`, so a mismatch fails loudly. Not needed for the first version.

### Tokens must be recorded at zip creation, never recomputed

Tempting: since the seed determines the tokens, validate a submission by re-running
`main -a -s <seed>`. **This is wrong and would break the site.**

Stored zips are permanent, so rebuilding the generator "changes only the games of players who
haven't downloaded yet" (README, Web Server). After any change to a puzzle's rolls, a recomputed
token would disagree with the token in the zip the player already holds, and every existing
player's submissions would start failing.

So the tokens are frozen when the player's zip is generated, in `pointless_player_zip()`
(`web-server/includes/generate.php`), and read back from there forever.

### The answer key becomes one pretty-printed JSON file

Today `pointless_player_zip()` writes `games/<seed>.txt`: name, email, date, and the generator's raw
output. Replace it with `games/<seed>.json`, pretty-printed (`JSON_PRETTY_PRINT`), holding the same
facts in a parseable shape plus the tokens:

```json
{
  "seed": "12345678901234567890",
  "fname": "Ann",
  "lname": "Lee",
  "email": "ann@b.com",
  "generated": "2026-09-19 14:03:11 CDT",
  "puzzles": [
    { "n": 1, "name": "math", "password": "99", "token": "amber kettle drift stone" }
  ]
}
```

Pretty-printed so it stays readable for tech support, which is what the `.txt` was for. One file,
not two: the site needs the tokens parseable, and there is no reason to keep a prose copy beside it.

To build it, the generator should emit JSON on request rather than have PHP parse
`name Password: xxx` lines. Add a flag (say `-j`) that prints the puzzle list as JSON. Keep the
human output exactly as it is: `main` must still print `Seed: <seed>` as its first line, which the
website parses and `tests/` depends on.

#### Why not parse the text output in PHP?

Because the current format is genuinely ambiguous. `./main -a -s 12345` prints:

```
math            Password: 83
rematch         Password: 126223751  (rematch1: 7814929, rematch2: theory, rematch3: 155)
bst             Password: 2338463  Path: lrlrllrlrl
fin             Password:
```

`extra_info` is free-form and inconsistent (`rematch` wraps its sub-passwords in parentheses, `bst`
writes `Path: ...`), it is separated from the password only by a two-space convention, and `fin`'s
password is empty, so the last line ends with a bare `Password:`. A regex over that is a latent bug.

#### The C++ side is about 30 lines, with no new dependency

The structure is flat (a seed and an array of four-field objects), so it needs no JSON library --
which matters, since the project's only dependency is `libzip`. One escape helper:

```cpp
static std::string json_escape(const std::string &s)
{
  std::string out;
  for (char c : s) {
    switch (c) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n";  break;
      case '\r': out += "\\r";  break;
      case '\t': out += "\\t";  break;
      default:
        if ((unsigned char)c < 0x20) {
          char buf[7];
          std::snprintf(buf, sizeof buf, "\\u%04x", c);
          out += buf;
        } else {
          out += c;
        }
    }
  }
  return out;
}
```

and one printer alongside `game_print_passwords()` that walks the vector emitting
`{"n":1,"name":"math","password":"83","token":"...","extra":null}`.

Today every password is `[A-Za-z0-9_]` and every `extra_info` is plain punctuation, so nothing
currently needs escaping at all -- the helper is there so that a future puzzle whose answer contains
a quote or a backslash cannot silently produce invalid JSON.

**PHP does the rest.** `json_decode()` the generator's output, add `fname`, `lname`, `email` and
`generated`, then `json_encode(..., JSON_PRETTY_PRINT)` and store. All escaping and pretty-printing
happen in PHP, which already does both correctly, so the C++ side only has to emit valid compact
JSON on one line.

### Submissions and level live in the events log

Tokens are **not** stored in `data/contact-data.csv`. They are created at first download, not at
registration, so writing them to the roster would mean updating a row after the fact — the
read-modify-write path the append-only design exists to avoid. They would also add one column per
puzzle, tying the roster's shape to the puzzle count. What the School of Computing wants from the
roster is the level, one number, and that is better produced by a report than stored.

The roster keeps its four columns (`FName,LName,Email,Seed`). A second append-only file,
`data/events.csv`, records what happened:

| Column | Meaning |
|--------|---------|
| `Time` | When the event happened |
| `Event` | `register`, `download`, `token-ok`, or `token-bad` |
| `Email` | The player, normalized as in `pointless_normalize_email()` |
| `Detail` | The submitted phrase, or the file downloaded |
| `Level` | The player's level *after* this event |

**Level is a fold, not stored state**: one less than the highest token index ever accepted for that
email (see the arithmetic above), or 0 if none. Writing
it into each record as a snapshot means the newest record for an email gives the level directly
(a reverse scan, as in `pointless_find_player()`), while the events underneath still reconstruct it
if it is ever doubted.

Log rejected submissions too. They are the brute-force signal, and they are participation data for
the analytics ask below.

### Rate limiting is load-bearing

Tokens are the only proof of progress, so the submission form needs a throttle like
`POINTLESS_DOWNLOAD_INTERVAL` in `web-server/download.php`, or stricter. This is a different job
from the download throttle: that one protects the generator from repeated runs, this one protects
the integrity of the record.

Related, already-accepted risk: logging in needs only an email (README, Web Server), so anyone who
knows a player's email can submit on their behalf. They would still need that player's seed-specific
tokens, so it is not a free win. **If levels ever carry prizes, revisit that decision first.**

### Token strength

A four-word phrase from a list of about 1000 words is roughly 40 bits — far beyond guessing through
a throttled web form.

Worth knowing, though not a practical problem: `utils_derive_seed()` is FNV-1a over the name (XOR
and multiply by an odd constant), which is **invertible**. Anyone who recovered a puzzle's seed
could run it backward to the game seed and forward to every other puzzle's token. Recovering that
seed from a token means finding the 64-bit seed behind four ~10-bit splitmix64 outputs, which has no
known shortcut, so this is safe in practice. But do not shorten tokens much below four words, and
do not print a raw seed anywhere a player can see it.

## Work list

| Where | Change |
|-------|--------|
| `resources/templates/words.txt` | New word list (about 1000 words). |
| `resources/templates/header.txt` | Style the token block. |
| `src/utils.{h,cpp}` | Add `utils_token()`; add the token parameter to `utils_html_printf()`. |
| `src/*-puzzle.cpp` (11 files) | Derive the token, pass it to `utils_html_printf()`. Two lines each. |
| `src/game.cpp`, `src/main.cpp` | Print tokens in the password listing; add `-j` JSON output. |
| `tests/` | Check that a page contains its token and that passwords are unchanged. No signature changes. |
| `includes/generate.php` | Write `games/<seed>.json` instead of `<seed>.txt`; run the generator with `-j`. |
| `includes/players.php` or a new `includes/events.php` | Append to `data/events.csv`; read a player's level. |
| `download.php` | Show the level; add the token entry field and its throttle. |
| `README.md` | Update the Web Server section: the events file, the JSON answer key, the token flow. |

## Open questions

- **How does the token block read?** Wording and placement on the page are undecided. It should say
  what the phrase is for and that the player submits it on the website.
- **Migration: none needed.** No players have registered yet (2026-09-19), so there are no stored
  zips or `<seed>.txt` answer keys to carry forward, and the RNG may change freely. If that stops
  being true before this ships, revisit: a token cannot be added to a game that is already
  downloaded, and regenerating changes the player's puzzles.
- **Congratulation.** "Recognized along the way" may mean more than a level on the download page —
  an email at milestones, say. Nothing here sends mail.
- **Analytics.** GitHub issue #3 (closed 2026-09-19 and moved here) asked for participation
  statistics, so the project's upkeep can be justified. `events.csv` is designed to answer this, but
  the report itself (roster joined to level) is not specified.

## History

### What existed, and what was removed

On 2026-09-14:

- `Tokens.txt` (repository root) listed the tokens below. Nothing read it. **Removed**; the list is preserved here.
- `web-server/token-sub.php` was a form with a single "Token" field that posted to itself. It had no handling code. **Removed.**
- `web-server/login.php` redirected to `token-sub.php` after login. **Changed** to redirect to `download.php`.

On 2026-09-15:

- The player data file had a `Token` column, and `index.php` wrote `n\a` into it for every new player. **Removed**: new files have only `FName,LName,Email`. Older files that still have the column work unchanged.

### Token list (from Tokens.txt)

These were fixed phrases, shared by every player. The plan above replaces them with per-player
phrases rolled from the seed, so they are kept only as a record.

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

The list has 10 entries, but the game now has 11 puzzles (see `puzzle-code/src/game.cpp`). The numbering follows an older puzzle order, and none of these phrases appear in any puzzle today.
