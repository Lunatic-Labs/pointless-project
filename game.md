## What's already working

Three running gags exist and are consistent across all 11 layers:

1. **"0 points"** — awarded on every page, and [files-math/.desc.html](puzzle-code/resources/files-math/.desc.html) sets it up with "Make sure to keep track of all the points you earn." Payoff in [files-fin](puzzle-code/resources/files-fin/.desc.html): "Points earned: 0."
2. **The MacGuffin** — named in math, encrypt, binary-addition, logicgate, bst, and revealed in fin. The word is already doing the work you're asking about.
3. **The callback chain** — every page opens by naming the previous puzzle by nickname ("speedy maze", "Lou's pixel art", "queue of logic gates", "alien lightbox", "Dr. Pepper's code"). I verified the chain matches the play order in [game.cpp:9-21](puzzle-code/src/game.cpp#L9-L21). It's unbroken.

What's missing is that gag #3 already contains a fourth gag that never got finished.

## The one structural change I'd make: finish the Plot Coupon gag

Three pages award a *useless item*, not just points:

- color → the message `"brute force"`
- pixel → a `"color wheel"`
- rematch → the `"master key"`

None of them is ever used for anything. That's a Plot Coupon, and it's the funniest thing in the game — but it fires only 3 times out of 11, so it reads as inconsistency rather than a bit. Make it fire every layer, keep a running inventory, and cash it all out in the finale.

Proposed item per layer (play order):

| # | Puzzle | Item awarded |
|---|--------|--------------|
| 1 | math | the message "brute force" *(exists)* |
| 2 | color | a color wheel *(exists)* |
| 3 | pixel | one slightly used pixel |
| 4 | maze | a map of somewhere else |
| 5 | based-intro | a spare bulb (green) |
| 6 | encrypt | Dr. Pepper's code *(exists)* |
| 7 | rematch | the master key *(exists)* |
| 8 | binary-addition | one dead battery |
| 9 | logicgate | a Tower of Wisdom, flat-pack |
| 10 | bst | a map — of the maze you already finished |
| 11 | fin | one (1) chicken |

Two of these do real narrative work for free:

- **The map.** [files-bst/.desc.html](puzzle-code/resources/files-bst/.desc.html) already says *"you lost your map!"* — with no map ever having been given. Award a (wrong) map at maze, and that line becomes a payoff instead of a non-sequitur.
- **The battery.** [files-binary-addition](puzzle-code/resources/files-binary-addition/.desc.html) opens with the robot *"shut down, its energy reserves depleted."* Award a dead battery there and the joke writes itself.

**Implementation:** hand-write a growing `<div class="callout">` inventory into each `.desc.html`, right under the opening line. Each page already hard-codes its predecessor's name, so hard-coding the list is consistent with how the pages work and needs no C++ change. (The alternative — accumulating a `strvec_t` in `game_create_puzzles` and having `utils_html_printf` inject an inventory block next to the token block, the way [utils.cpp:207-212](puzzle-code/src/utils.cpp#L207-L212) does — is cleaner but touches the signature on all 14 call sites. Only worth it if you expect to reorder puzzles.)

## Per-file suggested text

### files-math — the Call to Adventure

Replace the Fetch Quest paragraph with an explicit MacGuffin gloss plus the quest rules that set up all three gags:

```html
<p>Your mission, should you choose to accept it, is a classic <b>Fetch Quest</b>: a lot of
puzzles, a long journey, and at the end of it the coveted <b>MacGuffin</b>.</p>

<p>You may be wondering what a MacGuffin <i>is</i>. Excellent question. In the movies, nobody
ever says. It is the briefcase, the falcon, the plans, the ring &mdash; the thing everybody
wants, whose only job is to make everybody run around. Ours is no different, and we would
not dream of spoiling it.</p>

<div class="callout">
    <p><b>The rules of the quest</b></p>
    <ul>
        <li>Each puzzle's answer is the password to the next zip file.</li>
        <li>Each puzzle awards <b>points</b>. Keep careful track of them.</li>
        <li>Each puzzle awards an <b>item</b>. It may help you on your quest.</li>
    </ul>
</div>
```

Keep the sentence "The password is the way to access the next zip file." verbatim — [math-puzzle-test.cpp:7](puzzle-code/tests/math-puzzle-test.cpp#L7) asserts it.

### files-color — introduce the mentor

"It May Help You on Your Quest" works best as a *character tic*. You already have an Oracle in [files-rematch-maze](puzzle-code/resources/files-rematch-maze/.desc.html); promote it to the recurring Old Person Who Hands You Things, and let the line repeat verbatim on every award.

```html
<p>Congratulations on completing the first challenge! You have been awarded <b>0 points</b>
and the message <b>"brute force."</b></p>

<p>An old traveler presses it into your hands. "It may help you on your quest," she says, and
declines to elaborate. Wonder what that could be? Keep it. You are now ready to move on to
the first real puzzle.</p>
```

### files-maze — award the wrong map

```html
<p>So, you have beaten the "Lou's pixel art" puzzle, good job! You have been awarded
<b>0 points</b> and <b>a map</b>. The map is of somewhere else. Keep it anyway &mdash; you
will want a map later, and this will not be it. But there's no time to celebrate.</p>
```

The existing closing line, *"(Sometimes, the only way out is the fastest way. Can you compress the path to escape?)"*, is a good place for the Journey trope's first jab:

```html
<p><i>They say it's the journey that counts. This journey is scored on length, so: no.</i></p>
```

### files-rematch — the literal Plot Coupon page

This is your driest page and also the one page that is *mechanically* a plot-coupon hunt: three fragments, concatenated into one artifact.

```html
<p>With "Dr. Pepper's code" and 0 extra points, you have made it to the round of rematches!</p>

<p>Every quest has this part. Three trials, three fragments, one artifact. Scholars call the
fragments <b>Plot Coupons</b>: collect the full set, mail them in, receive one (1) plot.</p>

<p>Here you will replay three puzzles you have already solved, but a bit remixed. Each one you
finish gives you a number. When you have all three, put them together in order (rematch 1,
then 2, then 3) to forge them into the <b>master key</b>.</p>

<p><small>The master key opens exactly one thing, once. Standard for master keys in this
line of work.</small></p>
```

That last line sets up binary-addition, where you then add: *"The key has already stopped working."*

### files-bst — the third-act lostness

Keep the sentence `"You will need to look at the mathematical street signs to find your way to x."` exactly — [bst-puzzle-test.cpp:14](puzzle-code/tests/bst-puzzle-test.cpp#L14) asserts it. Change only what's around it:

```html
<p>Great job, you have completed the "queue of logic gates!" For this you get <b>0 points</b>
and <b>a map</b> &mdash; a real one this time, of the maze you finished six puzzles ago.</p>

<p>You are closing in on the MacGuffin. You are also lost, because the map you were given is of
somewhere else and the map you need is gone. This always happens in the third act. You will
need to look at the mathematical street signs to find your way to x.</p>
```

### files-fin — where "It's the Journey That Counts" belongs

This is the weakest page relative to its position: it's the punchline slot and currently delivers chicken, kaomoji, and "hope you learned something." Cash in all three gags at once:

```html
<h3>Final inventory</h3>
<div class="callout">
    <ul>
        <li>the message "brute force" &mdash; never used</li>
        <li>a color wheel &mdash; never used</li>
        <li>one slightly used pixel &mdash; never used</li>
        <li>a map of somewhere else &mdash; never used</li>
        <li>a spare bulb &mdash; never used</li>
        <li>Dr. Pepper's code &mdash; never used</li>
        <li>a master key &mdash; used once, then broke</li>
        <li>one dead battery &mdash; never used</li>
        <li>a Tower of Wisdom, flat-pack &mdash; some assembly required</li>
        <li>a map of the maze you had already finished &mdash; never used</li>
        <li>one (1) chicken</li>
    </ul>
</div>

<p>This is the part where we tell you <b>it's the journey that counts</b>. Please notice that we
waited to tell you this until <i>after</i> the journey, once there was nothing left in it
for us.</p>

<p>Still. The base conversion was real. The maze was real. The logic gates, the binary adder,
the tree of street signs &mdash; all real, all yours now, and not one of them fits in an
inventory. The chicken was just the excuse to hand them to you.</p>

<h2>Points earned: <b>0</b></h2>
<p><small>This ties the all-time record.</small></p>
```

Optional Sequel Hook as the final line: `<p><small>The chicken has been stolen. &mdash; <i>Pointless II</i>, coming whenever.</small></p>`

Keep the substring `pointless journey` intact — [fin-puzzle-test.cpp:7](puzzle-code/tests/fin-puzzle-test.cpp#L7) asserts it (and also asserts the whole `.desc.html` is embedded, so the rest of that file is self-updating).

## Page titles: free trope flavor

The `<h2>` on each page comes from the `%TITLE` first line of the puzzle's `.desc.html`, and they're currently utilitarian — "Color Puzzle", "Encrypt", "BST Puzzle", "Base Intro Puzzle". Each is one line of prose, affects no rolls, and appears in no test. Cheapest high-impact edit in the whole review:

| Description | Current | Suggested |
|---|---|---|
| [files-math](puzzle-code/resources/files-math/.desc.html) | Hello Pointless | The Call to Adventure |
| [files-color](puzzle-code/resources/files-color/.desc.html) | Color Puzzle | The Ordinary World, in Hexadecimal |
| [files-pixel](puzzle-code/resources/files-pixel/.desc.html) | Pixel Puzzle | Trust Nothing, Especially Lou |
| [files-maze](puzzle-code/resources/files-maze/.desc.html) | Maze Puzzle | The Labyrinth |
| [files-based-intro](puzzle-code/resources/files-based-intro/.desc.html) | Base Intro Puzzle | The Cryptic Artifact |
| [files-encrypt](puzzle-code/resources/files-encrypt/.desc.html) | Encrypt | The Eccentric Genius |
| [files-rematch](puzzle-code/resources/files-rematch/.desc.html) | Rematch Instructions | Gather the Plot Coupons |
| [files-binary-addition](puzzle-code/resources/files-binary-addition/.desc.html) | Graph Paper Robot | The Companion Reawakens |
| [files-logicgate](puzzle-code/resources/files-logicgate/.desc.html) | Graph Paper Robot PT II | The Tower of Wisdom |
| [files-bst](puzzle-code/resources/files-bst/.desc.html) | BST Puzzle | Lost Without a Map |
| [files-fin](puzzle-code/resources/files-fin/.desc.html) | Goodbye Pointless | The MacGuffin Revealed |

## Cast: four characters appear once each

You have Lou (pixel), Dr. Pepper (encrypt), the Oracle (rematch-maze), and the golden robot (maze + binary-addition). Only the robot recurs. Cheapest fix is to reuse the existing four rather than invent more:

- **The Oracle** → the mentor who hands over every useless item, always with the same line.
- **The golden robot** → the Loyal Sidekick. Note that [logicgate](puzzle-code/resources/files-logicgate/.desc.html) is titled "Graph Paper Robot PT II" but its text never mentions the robot at all — that's a free place to bring it back.
- **Dr. Pepper** → the Mad Scientist. [files-rematch-encrypt](puzzle-code/resources/files-rematch-encrypt/.desc.html) already has his slapstick voice ("we *accidentally* dropped the password into a goopifying machine") without attributing it to him. Attribute it.
- **Lou** → the Unreliable Narrator, already perfect in one line: *"Lou seems a little off today… or maybe it's you."*

## Continuity nits

- **"alien lightbox"** ([files-encrypt](puzzle-code/resources/files-encrypt/.desc.html)) refers back to based-intro, whose light box isn't alien — it's "a cryptic structure." The *alien* one is [files-rematch-based](puzzle-code/resources/files-rematch-based/.desc.html), which comes later. Either make based-intro's light box explicitly alien, or change the callback. Note the string is asserted at [encrypt-puzzle-test.cpp:12](puzzle-code/tests/encrypt-puzzle-test.cpp#L12), so changing it means updating that line.
- **"You seemed to struggle on the first encryption puzzle"** ([files-rematch-encrypt](puzzle-code/resources/files-rematch-encrypt/.desc.html)) presumes a failure that may not have happened. Trope-ify it instead: *"The Oracle has reviewed your performance and is concerned. It would like to give you a freebie."*
- **The rematch sub-puzzles have no "0 points" line** except rematch-based. Since they're also the only pages with no token, that's defensible — but a one-line acknowledgement ("Rematches are unscored. So is everything else.") would keep the voice unbroken.
- **The rematch-maze key and battery are the only items in the game that actually do something.** Worth a wink there: *"An item that is genuinely required. Savor it."*

## Website copy

[index.php:37-42](web-server/index.php#L37-L42) is the front door and currently reads as a mailing-list signup. Suggest:

```html
<h2>The Pointless Challenge.</h2>
<p>
    At the end of a chain of locked zip files there is a <b>MacGuffin</b>. We will not tell
    you what it is. That is what makes it a MacGuffin.
</p>
<p>
    Between you and it: eleven puzzles, a number of items you will never need, and exactly
    <b>0 points</b>. Register your email and we'll send you on your way.
    Already registered? Log in <a href="./login.php">here</a>.
</p>
```

And in [download.php:81-89](web-server/download.php#L81-L89), the progress block is the natural home for the points gag's web payoff — add a second line under "Puzzles solved" reading `Points earned: <b>0</b>`, and reframe the token as quest-proof: *"Every puzzle page shows a token: proof you were there, which is the only kind of proof this quest issues."*

## Before you apply any of this

Text-only edits don't touch `utils_rng_roll`, so no password changes and no reseeding of downloaded games. The only way these edits break `make test` is the `CHECK_PUZZLE` snippet list. The asserted phrases you must preserve (or update in lockstep) are: `speedy maze`, `brute force`, `color wheel`, `master key`, `alien lightbox`, `adding the colored squares`, `Lou's pixel art`, `queue of logic gates`, `pointless journey`, plus the five exact prose sentences in math/color/maze/bst/logicgate tests.

Per CLAUDE.md, narrative changes to a puzzle also mean updating its "Current Puzzles" section in [README.md](README.md#L290), and any new running gag is worth a line in [ideas/storyline-ideas.txt](ideas/storyline-ideas.txt) — which currently holds only an unrelated "Data Syndicate" pitch in a completely different (serious, cyber-thriller) register than the game actually shipped with.