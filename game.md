# The story layer

How the game's prose fits together, and the rules to keep it consistent when adding or editing a
puzzle. Everything described here is implemented unless it sits under "Still open."

The puzzles themselves are documented in [README.md](README.md) ("Current Puzzles"); this file
covers only the narrative that wraps them.

## The page shape

All eleven main pages follow the same four beats, in this order:

1. **A short congratulation** — two to four words. "Nicely done!", "Keen eyes!", "The tower stands!"
   Not a sentence about the previous puzzle's mechanics.
2. **A character hands over the previous puzzle's item**, in one or two sentences of prose. The
   item must be the row *above* this puzzle's in `ITEMS` (see "The items" below).
3. **The puzzle's rules**, using the template's `.callout`, `.figure`, and `.question` classes.
4. **The question**, in `<p class="question">`.

Points are *not* mentioned in the prose. The sidebar that `utils_html_printf()` builds carries the
token, `Points: 0`, and the running inventory, so a page that also writes "you have been awarded 0
points" says it twice. [files-maze](puzzle-code/resources/files-maze/.desc.html) is the one
deliberate exception: the robot "idly observes that you have not yet managed to score any points."

The three rematch sub-pages get no sidebar and no token, so they follow the beats loosely and each
carries one line acknowledging it: *"Rematches are unscored. So is everything else."*

## The four running gags

1. **"0 points"** — every page's sidebar says `Points: 0`.
   [files-math](puzzle-code/resources/files-math/.desc.html) sets it up ("since its also a game,
   we'll keep score too") and [files-fin](puzzle-code/resources/files-fin/.desc.html) pays it off
   with "Points earned: 0" and "This ties the all-time record."
2. **The MacGuffin** — named in math, encrypt, binary-addition, logicgate, and bst, and revealed in
   fin as a chicken. Never explained anywhere, which is the joke.
3. **The Plot Coupons** — every layer awards a useless item, the sidebar keeps the running
   inventory, and fin's sidebar prints the final inventory with what each item turned out to be
   good for (mostly "never used"). [files-rematch](puzzle-code/resources/files-rematch/.desc.html)
   names the trope outright, because that layer is mechanically a plot-coupon hunt: three
   fragments, concatenated into one master key.
4. **"It may help you on your quest"** — the Oracle's line, repeated verbatim whenever an item
   changes hands.

## The items

The list lives in `ITEMS` in [inventory.cpp](puzzle-code/src/inventory.cpp), in play order, and
`inventory_html()` renders it. An item is awarded for *solving* its puzzle, so it appears in the
prose and the sidebar of the **next** page. The first page therefore shows no inventory, and the
last row (the chicken) is never listed — fin hands it over itself.

| # | Puzzle | Item | Handed over by |
|---|--------|------|----------------|
| 1 | math | an abacus | an old traveler (the Oracle) |
| 2 | color | a color wheel | a pixelated bison |
| 3 | pixel | a slightly used pixel | Eight-bit Lou |
| 4 | maze | a map of somewhere else | found |
| 5 | based-intro | a spare bulb (green) | the artifact, powering down |
| 6 | encrypt | Dr. Pepper's code | Dr. Pepper |
| 7 | rematch | a master key | forged from the three numbers |
| 8 | binary-addition | one dead battery | the golden robot |
| 9 | logicgate | a Tower of Wisdom, flat-pack | the golden robot |
| 10 | bst | a map of the maze you had already finished | the golden robot |
| 11 | fin | one (1) chicken | — |

Two of these carry real narrative weight: the maze's **wrong map** turns bst's "you lost your map"
into a payoff instead of a non-sequitur, and the **dead battery** explains why the robot in
binary-addition shut down. The **master key** is the only item that ever does anything — it opens
one zip file and then breaks, which binary-addition says out loud.

Adding a puzzle means adding its row to `ITEMS` in the same position as its call in
`game_create_puzzles`; `inventory_html()` throws for a puzzle with no row.

## The titles

The `%TITLE` first line of each `.desc.html` becomes the page's `<h2>` and its `<title>`. The
house style is a short trope name: no article, no "Puzzle", two or three words.

| Page | Title |
|---|---|
| [files-math](puzzle-code/resources/files-math/.desc.html) | Call to Adventure |
| [files-color](puzzle-code/resources/files-color/.desc.html) | Realm of Adventure |
| [files-pixel](puzzle-code/resources/files-pixel/.desc.html) | Eight-bit Lou |
| [files-maze](puzzle-code/resources/files-maze/.desc.html) | Labyrinth |
| [files-based-intro](puzzle-code/resources/files-based-intro/.desc.html) | Mysterious Artifact |
| [files-encrypt](puzzle-code/resources/files-encrypt/.desc.html) | Eccentric Genius |
| [files-rematch](puzzle-code/resources/files-rematch/.desc.html) | Plot Coupons |
| [files-rematch-maze](puzzle-code/resources/files-rematch-maze/.desc.html) | Oracle's Labyrinth |
| [files-rematch-encrypt](puzzle-code/resources/files-rematch-encrypt/.desc.html) | Dr. Pepper Apologizes |
| [files-rematch-based](puzzle-code/resources/files-rematch-based/.desc.html) | Alien Artifact |
| [files-binary-addition](puzzle-code/resources/files-binary-addition/.desc.html) | Companion Reawakens |
| [files-logicgate](puzzle-code/resources/files-logicgate/.desc.html) | Tower of Wisdom |
| [files-bst](puzzle-code/resources/files-bst/.desc.html) | Lost Without a Map |
| [files-fin](puzzle-code/resources/files-fin/.desc.html) | MacGuffin Revealed |

Note that "Graph Paper Robot I / II" survives in [README.md](README.md) and in the source comments
of `binary-addition-puzzle.cpp` and `logicgate-puzzle.cpp`. That is the *mechanic's* name, not a
page title, and it should stay.

## The cast

Four characters, each with one job:

- **The Oracle**, an old traveler — the mentor who hands over the useless items, always with the
  same line. She opens the game in [files-color](puzzle-code/resources/files-color/.desc.html) and
  turns out to be the voice in the console in
  [files-rematch-maze](puzzle-code/resources/files-rematch-maze/.desc.html).
- **The golden robot** — the loyal sidekick. Appears in maze, runs out of power in
  binary-addition, and hands over the last three items.
- **Dr. Pepper**, the eccentric genius — the mad scientist. Encrypts the phrase in
  [files-encrypt](puzzle-code/resources/files-encrypt/.desc.html), and is the one who drops the
  free password into the goopifying machine in
  [files-rematch-encrypt](puzzle-code/resources/files-rematch-encrypt/.desc.html).
- **Eight-bit Lou** — the unreliable narrator, in one line: *"Lou seems a little off today… or
  maybe it's you?"*

Reuse these four rather than adding more.

## Still open

- **Website copy.** [index.php](web-server/index.php) is the front door and still reads as a
  mailing-list signup; it is the natural place to promise a MacGuffin and refuse to say what it is.
  [download.php](web-server/download.php)'s progress block is the natural home for the points gag's
  web payoff (a `Points earned: 0` line under "Puzzles solved") and for reframing the token as the
  only proof this quest issues.
- **[ideas/storyline-ideas.txt](ideas/storyline-ideas.txt)** holds only an unrelated "Data
  Syndicate" pitch, in a serious cyber-thriller register that the shipped game never adopted.
  Either rewrite it to match the game or retire it.
- Puzzle-level prose problems that are not about the story layer — the *Logic Gate* rules that
  never say the queue starts at the right, the maze colors whose names disagree with the tiles, the
  missing "press F12" hint in *Maze Rematch*, the *Based Rematch* rules with no worked row — are
  tracked in [ideas/cleanup-tasks.md](ideas/cleanup-tasks.md), not here.

## Editing the prose safely

Text-only edits never touch `utils_rng_roll`, so passwords do not move and already-downloaded games
are unaffected. The one way to break `make test` is the `CHECK_PUZZLE` snippet list: each puzzle's
test asserts one or two exact strings from its page, to catch a `.desc.html` that stopped being
rendered at all. Currently asserted, per puzzle:

| Test | Asserted |
|---|---|
| math | `way to access the next puzzle.` |
| color | `An old traveler hands you an abacus` · `Web colors are defined by three hexadecimal values` |
| pixel | `color wheel` · `Lou seems a little off today… or maybe it's just you?` |
| maze | `Eight-bit Lou fades in` · `should be replaced by the letter followed by the number` |
| based-intro | `Good job encoding those instructions!` · `the artifact below means 5 + 0 + 3&times;49 = 152` |
| encrypt | `a single green bulb rolls loose` · `Here are the steps he used:` |
| rematch | `put them together in order` |
| rematch-maze | `You vaguely recall that typing <code>instructions</code> into the …` |
| rematch-encrypt | `All we have are the magic machines that encrypted the password.` |
| rematch-based | the `<li>` explaining base-16 |
| binary-addition | `master key` · the first robot rule `<li>` |
| logicgate | `ejects the battery that got it this far` · the first queue-rule `<li>` |
| bst | `boxes it up flat-packed` · `You will need to look at the mathematical street signs to find your way to x.` |
| fin | `pointless journey` · `<h2>MacGuffin Revealed</h2>` · the whole `.desc.html` body |

Rewriting around one of these is fine; just update the test in the same commit. `fin`'s test embeds
the whole description, so that page's body is self-updating apart from its title.

Per [CLAUDE.md](CLAUDE.md), a change to a puzzle's behavior also means updating its "Current
Puzzles" section in [README.md](README.md), and anything that looks like a leftover belongs under
the README's "Issues".
