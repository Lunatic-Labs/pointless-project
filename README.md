# Pointless Project

A website to generate and distribute puzzles to prospective Lipscomb recruits.

## Introduction

> The point(less) of the *Pointless Project* is to create a compelling, puzzle-filled
> challenge inspired by the [Synacor Challenge](https://challenge.synacor.com), but targeted at a pre-college audience.
> The goal is to allow the School of Computing (SoC) to engage potential
> students during the time they are selecting a college by providing opportunities to remain in
> contact, encourage curiosity, and engage in creative problem solving while learning about
> computing and the SoC.
>
> Playing through the Synacor Challenge will provide the clearest understanding, however reading
> someone else's "[travel log](https://github.com/KanegaeGabriel/synacor-challenge/blob/master/writeup.md)" is also insightful. The key components to success include:
> 1. a personalized download containing the complete game
> 2. the ability to play completely offline
> 3. a series of different but related logic puzzles and riddles
> 4. multiple opportunities "along the way" to be recognized for their problem solving skills.

## Project Setup

### Installation

For Debian based distributions, run:

```bash
sudo apt update
sudo apt install g++ libzip-dev php-cli
```

### Building

First, clone the repository if not done already:

```bash
git clone https://www.github.com/Lunatic-Labs/pointless-project.git
```

There is one `Makefile`, in `puzzle-code/`. Run `make` from there:

```bash
cd ./puzzle-code/
make <opt>
```

where `opt` is one of:
- *(none)* or `build`: build the generator, `src/main`
- `run`: build, remove old zipfiles, and run the generator from `src/`
- `test`: build and run the automated tests; `make test T=<name>` runs only tests whose names contain `<name>` (see [Automated Tests](#automated-tests))
- `cleanzip`: remove all generated zipfiles and generated puzzle files
- `clean`: also remove the `build/` directory and both binaries (does not touch `production/`)
- `coverage`: see [Code Coverage](#code-coverage)
- `production`: build an optimized generator (no debug info or coverage) into `puzzle-code/production/` for the web server (see [How to Start](#how-to-start))

Object files go in `puzzle-code/build/`. Zipfiles are generated in `puzzle-code/src/zipfiles/`.
You can now inspect and solve the puzzle(s) by choosing the appropriate zipfile.

**NOTE**: The generator must be run from `puzzle-code/src/` (and the tests from `puzzle-code/tests/`),
because all resource paths are relative to it, e.g. `../resources/files-math/.desc.txt`. `make run` and `make test` do this for you.

`./main` accepts the following flags:
- `-s <seed>`: use the given seed (any number) instead of a random one. Without `-s` or `-e`, the random seed is printed so the run can be repeated.
- `-e <email>`: use the seed for the player with this email (the website does this; see `utils_seed_from_email()`)
- `-a`: only print the answers (including the rematch puzzles' passwords); do not generate any files

### Structure of Output

Upon running `./main`, the password for each puzzle is printed and the `zipfiles` directory is populated.
The entire game is `zipfiles/puzzle1.zip`, which contains the nested zipfiles `puzzle2..puzzleN`.
The other puzzles are also generated outside of `puzzle1.zip` to allow easy testing
without having to go through the entire zipfile structure.

Each `puzzleN.zip` holds puzzle N's files, unencrypted, and `puzzle{N+1}.zip`, encrypted (AES-256) with puzzle N's password.

### Repository Layout

| Path | Contents |
|------|----------|
| `puzzle-code/src/` | The C++ puzzle generator |
| `puzzle-code/tests/` | Automated tests for the generator |
| `puzzle-code/resources/` | Per-puzzle resource directories (`files-<name>/`) and the shared HTML header/footer (`templates/`) |
| `puzzle-code/production/` | Output of `make production`: the generator the web server runs (not in git) |
| `web-server/` | The PHP website that registers players and serves downloads |
| `web-server/tests/` | Automated tests for the website |
| `data/` | Player data written by the web server (not in git) |
| `.github/workflows/` | CI: runs both test suites on every push and pull request |
| `ideas/` | **Not production.** Ideas and unfinished or scrapped work, kept for reference (see [ideas/README.md](ideas/README.md)) |

## Puzzle Creation Framework

There are a lot of directories in this project, and not all
of them contain code. In fact, most of these directories contain the necessary resources
for their respective puzzle.

Any directory in `puzzle-code/resources/` that starts with `files-` is a resource directory. It contains (at least) the `.desc.txt`
for its puzzle, as well as any supporting files that the puzzle needs (other HTML files, images, etc).

> **Any support files that are prefixed with `.` will not be included in the zipped-up
> puzzle. Files that do not have this prefix will be included in the zipfile.** Subdirectories are
> included recursively, unless their name also starts with `.`.

Each puzzle's generated HTML page (`instructions.html`) has a specific structure:

| instructions.html  |
|--------------------|
| Header (generated) |
| Body (.desc.txt)   |
| Footer (generated) |

The HTML *Header* and *Footer* are located in `puzzle-code/resources/templates/` and do
not need to be touched (unless changes are needed). The header opens the page, `<body>`, and the `content` and `container` divs;
the footer closes them. `utils_html_printf()` puts the puzzle title in an `<h2>` and wraps the body in a `<section>`.

A `.desc.txt` must therefore be an HTML *fragment* with balanced tags: no `<html>`, `<head>`, or `<body>`, and no unclosed or
extra closing tags, or it will break the page layout around it. `<style>` and `<script>` blocks may appear anywhere in it.

The game is played offline, so a `.desc.txt` must not *load* anything from the internet (images, scripts, fonts).
Ordinary links for further reading are fine.

In short, a puzzle is created like this:
1. `game_create_puzzles()` in `src/game.cpp` calls `<name>_puzzle_create(seed)` with a seed derived from the player's seed and the puzzle's name.
2. The puzzle rolls random values with `utils_rng_roll()`, works out the password, and builds its page with `utils_html_printf()`.
3. It writes `instructions.html` into its resource directory and returns a `Puzzle`.
4. `game_write_zipfiles()` zips every resource directory, innermost puzzle first.

## Implementing a New Puzzle

Implementing a new puzzle is incredibly easy. Here are the steps
to take when doing so.

### Puzzle Resources Directory

Start by creating a new directory in `puzzle-code/resources/` and name it `files-<new puzzle name>`.
Then inside of there, create a new file called `.desc.txt`. This is where the instructions,
hints, and other info about the puzzle are stored. Everything in this file will be put into an HTML file,
so make sure that it follows the HTML rules.

In order to transfer information easily from C++ to HTML/JavaScript, we use a special delimiter in this file,
namely `%DELIM`. For example, if I need to pass a password and an array that is generated in C++
into this `.desc.txt` file, I would do something like:

```html
<script>
  let password = %DELIM;
  let array = [%DELIM];
</script>
```

It is then the job of C++ to "stringify" the required information to pass to `.desc.txt`.
There must be exactly one argument for each `%DELIM`; otherwise `utils_html_printf()` throws an error.
To make the explanation easier to follow, I will make a new puzzle called "fib",
where the point(less) of it is to have the user find the *n*th number of the Fibonacci sequence.

```bash
cd ./puzzle-code/resources/
mkdir files-fib && cd files-fib
echo "What is the <b>%DELIM</b>th number in the fibonacci sequence?" > .desc.txt
```

### Puzzle Implementation

Once the above step is done, create a new file called `<puzzle name>-puzzle.cpp`
in `puzzle-code/src/`. It includes `./include/puzzle.h` for the `Puzzle` struct and `./include/utils.h` for the utilities.

The function returns a `Puzzle`, defined in `src/include/puzzle.h`:

```cpp
struct Puzzle {
  std::string contents_fp;               // The resource directory, "../resources/files-<name>"
  std::string contents_html;             // The puzzle's instructions.html
  std::string password;                  // The answer, which unlocks the next puzzle's zip
  std::optional<std::string> extra_info; // Printed after the password for debugging; never shown to players
};
```

Here is the complete puzzle:

```cpp
// puzzle-code/src/fib-puzzle.cpp
#include "./include/puzzle.h"
#include "./include/utils.h"

static int fib(int n)
{
  return n < 2 ? n : fib(n-1) + fib(n-2);
}

Puzzle fib_puzzle_create(seed_t seed)
{
  // Get a random number from 3 to 10.
  int n = utils_rng_roll(3, 10, seed);

  // Generate the HTML content to be displayed to the user.
  std::string html_content = utils_html_printf("Fibonacci Sequence", "../resources/files-fib/.desc.txt",
                                               {std::to_string(n)});

  // Create the instructions.html.
  utils_generate_file("../resources/files-fib/instructions.html", html_content);

  return {"../resources/files-fib", html_content, std::to_string(fib(n)), {}};
}
```

Keep any helper functions `static`, and get every random value from `utils_rng_roll()`, `utils_chance()`, or `utils_shuffle()`
(not `rand()` or `<random>`), so that every platform generates the same puzzles.

Now add the signature `Puzzle fib_puzzle_create(seed_t seed);` to `puzzle-code/src/include/puzzle.h`.
No `Makefile` changes are needed; it builds every `.cpp` file in `src/`.

### Using the Puzzle

`game_create_puzzles()` in `puzzle-code/src/game.cpp` lists the puzzles in play order:

```cpp
return {
  math_puzzle_create(utils_derive_seed(seed, "math")),
  color_puzzle_create(utils_derive_seed(seed, "color")),
  pixel_puzzle_create(utils_derive_seed(seed, "pixel")),
  // ...
  fin_puzzle_create(utils_derive_seed(seed, "fin")),
};
```

Put the new puzzle where you want it to appear in the nested zipfiles. For example, to make it the third puzzle:

```cpp
return {
  math_puzzle_create(utils_derive_seed(seed, "math")),
  color_puzzle_create(utils_derive_seed(seed, "color")),
  fib_puzzle_create(utils_derive_seed(seed, "fib")), // Added it here
  pixel_puzzle_create(utils_derive_seed(seed, "pixel")),
  // ...
};
```

Now run `make run` in `puzzle-code/` and these things will happen:
- The `files-<puzzle name>` directories will all generate a file called `instructions.html`.
- `puzzle-code/src/zipfiles/` will be populated with zipfiles.

Finally, add an automated test for the puzzle (see [Implementing New Tests](#implementing-new-tests)).

## Current Puzzles

These are the currently implemented puzzles. Each one has three entries.
1. *Adjustable Variables*: Variables can be adjusted to make the puzzle harder/easier
2. *Description*: What the puzzle does
3. *RNG*: What exactly in the puzzle is random

### Math

*Adjustable Variables*:
- `MATH_MIN1`
- `MATH_MAX1`
- `MATH_MIN2`
- `MATH_MAX2`

*Description*:

This puzzle serves as the *Hello, World!* puzzle. It introduces the user
to reading the description, solving the puzzle, and inputting the password.

We prompt the user to solve a very simple math question in the form of $a + b$.

*RNG*:
- $a$, $b$

### Color

*Adjustable Variables*: *None*

*Description*:

This puzzle asks the user to enter the hex value of one of Lipscomb's colors (purple or gold).
The goal of this puzzle is to teach the user about hex values. A player who is online can search for the colors
(for example on [this website](https://teamcolorcodes.com/lipscomb-university-bisons-color-codes/)); one who is offline
can find them in the page's source, in the bison in the header.

*RNG*:
- purple | gold

### Pixel

*Adjustable Variables*:
- `ROWS` (**must change the number of rows in `.desc.txt` to match**)
- `BISON_COLORS`: the colors that can be used and their pixel counts (**must match the bison in `templates/header.txt`**; the tests check this)

*Description*:

The goal of this puzzle is to retest the user on their knowledge of [hexadecimal color codes](https://en.wikipedia.org/wiki/Web_colors)
learned in the "Color" puzzle. The hex codes come directly from the bison SVG included in the page's header,
which is drawn with a grid on this page so its pixels can be counted.

The user is shown a hex code along with a number.

- `#331E54 = 66`

The number corresponds to the number of pixels of the given hex code found in the bison SVG.
Following the first code/number pair, there are two more rows. The first row will contain two hex codes, an "×" denoting multiplication, and a number.
The number is the product of the pixel counts related to the respective hex codes.

- `#F4AA00 × #331E54 = 396`

The final row contains three hex codes and a question mark. The user must find the product.

- `#FFFFFF × #F4AA00 × #000000 = ?`

The bison's 183 brown pixels are never used, so nobody has to count them.

*RNG*:
- The hex codes in each row

### Maze

*Adjustable Variables*:
- `MAZE_SIZE` (**must be an odd number**)

*Description*:

The goal of this puzzle is to teach the user about [run-length encoding](https://en.wikipedia.org/wiki/Run-length_encoding).
This encoding algorithm normally places numbers before the letters, while we expect the numbers to be after.
We do this because it more closely matches function calls in the form of $f\ x \rightarrow n$.

We randomly generate a maze (`src/maze.cpp`) as an SVG and have the user navigate it from the gold square to the purple square using the following rules:

| Instruction | Direction |
|-------------|-----------|
| *u*         | up        |
| *d*         | down      |
| *l*         | left      |
| *r*         | right     |

However, the answer is not just the path that you must take (*uulrrrd* for example); you must
apply the run-length encoding algorithm to it. So the answer would be: *u2lr3d*.

*RNG*:
- The maze that is presented.

### Encrypt

*Adjustable Variables*:
- `ENCR_WORDS`
- `ENCR_OPS`
- `ENCR_ROTATIONS_MIN`
- `ENCR_ROTATIONS_MAX`
- `ENCR_CHANGE_MIN`
- `ENCR_CHANGE_MAX`

*Description*:

This puzzle challenges the user to decrypt the password. We present them with
a list of steps used to encrypt it, and they must undo it to get the password.

The following things can happen:
- shift all characters by $i$ to either the right $(r)$ or left $(l)$
- swap the characters at index $j$ and $k$
- increase each **fourth** letter alphabetically by $n$.

*RNG*:
- The word, and the order of the steps
- $i$, $r$, $l$, $j$, $k$, $n$

### Intro Base

*Adjustable Variables*:
- `BASE` and `LENGTH` (**must match `.desc.txt`**)

*Description*:

The goal of this puzzle is to introduce the player to number bases and base conversion.
The puzzle contains a visual representation of numbers with different bases called a lightbox:
one column per digit, with the least significant digit on the left, and one lit light per column.
The player must write the base-16 lightbox as digits, in the same order (so the answer is the hex number written backwards).

*RNG*:
- the digits (the last one is never 0)

### Rematches

The "rematch" puzzles serve as the point in the project where there is a
noticeable difficulty spike. It interrupts the linear nested puzzle format
by having the user solve three harder versions of previous puzzles, each in its own zip (`rematch1.zip` to `rematch3.zip`).
Each rematch's password unlocks a file with a 3-digit number; the three numbers in order are the password to advance past this
rematch section.

#### Maze Rematch

*Adjustable Variables*:
- `MAZE_SIZE` (**must be an odd number**, and matches the HTML)

*Description*:

This is the harder version of the *Maze* puzzle. We provide cryptic instructions
with the goal of having them inspect the page and navigate to the console. Once they
type "instructions" a list of function calls is presented. The user then needs to navigate
the maze $(M_1)$ using these function calls and must explore two other mazes $(M_2, M_3)$ with items that need to
be picked up (a key and a battery, on pink tiles). Stairs are brown (down) and green (up). Once these items are picked up, they then need to go to $M_1$ and go
to the gold tile. The password is then presented when they type "exit()".

*RNG*:
- $M_1, M_2, M_3$, password

#### Encrypt Rematch

*Adjustable Variables*:
- `ENCR_WORDS`

*Description*:

This is a harder version of the *Encrypt* puzzle. It presents the user with an encrypted password
as well as a *not-so-helpful-at-first* order of steps that were taken. The user must interact with
a textbox on screen and try to figure out the pattern in which the buttons mess with their input.
Once they have figured it out, they must decrypt the password.

*RNG*:
- The password, the substitution table, and the XOR key

#### Based Rematch

*Adjustable Variables*:
- `ROWS` (**must modify the table in `.desc.txt` to match**)

*Description*:

The goal of this puzzle is to teach the user about [base conversion](https://en.wikipedia.org/wiki/Positional_notation#Base_conversion).
They are given a description of three Pointless-created bases. The actual instructions follow in a story-esque fashion; here they are simplified:
- Convert values to base 10
- Sum them up for each row
- Convert the sum to base 2
- Check if LSB is on or off
- Save the resulting LSB values
- Take that base 2 number and convert it back to base 10 (the rightmost column)

They are then shown a table of eight rows, with values occupying each cell except for the rightmost cell in each row. The first four rows might look like:

|    |    |    |   |
|---:|---:|---:|---|
| 24 |  F |  3 | ? |
| 11 | 41 | 41 | ? |
|  A | 18 | 27 | ? |
| 35 |  F |  E | ? |

*RNG*:
- The values in each cell

### BST

*Adjustable Variables*:
- `ROOT_MIN`
- `ROOT_MAX`
- `DEPTH`: the number of signs between the top of the tree and an ID

*Description*:

The player is given a number $x$ and a tree of directories. Each directory holds a "street sign" (a text file) with a condition on $x$:
go into `right/` if it is true and `left/` otherwise. At the bottom of the correct path, `ID.txt` holds the password.

These are the possible signs, with examples:
- comparison: $x > 57$
- addition: $12 + 38 = x$
- subtraction: $150 - 100 = x$
- multiplication: $5 \times 10 = x$
- division: $250 / 5 = x$
- square root: $\sqrt{2500} = x$
- hexadecimal: `32` is $x$ in hexadecimal
- expression: $\sqrt{((a - b) + (c \times d))} = x$ (with or without the square root, and with $+$ or $-$, and $\times$ or $/$)

Each sign uses whole numbers only, and about half of the "$= x$" signs are true.

*RNG*:
- $x$
- the signs in each directory
- the IDs, and so the path from the top to the password

### Graph Paper Robot Puzzles

The idea behind this trilogy of paper robot puzzles is that we want the player to bring out
a piece of graph paper and something to color the squares with and do this all on paper. We want to
teach the player about certain topics in the first two paper robot puzzles (state, memory, etc.) then
incorporate those ideas in the last one.

#### Binary Addition (Graph Paper Robot I)

*Adjustable Variables*:
- `TAPE_WIDTH`: the number of bits in each number
- `TAPE_HEIGHT`
- `REQ_CARRIES`: the minimum number of columns where both numbers have a 1

*Description*:

The player is presented with a grid of 3 rows and 9 columns. The first two rows each hold a 7-bit number in red $(r)$ and green $(g)$ squares
(red means 0 and green means 1), with a purple square at the left of the first row and a gold square at its right; the third row is empty.
Following the rules in the instructions, the player colors in squares. The idea of this puzzle is that the player is actually performing binary addition,
and the answer is the 8-bit sum.

*RNG*:
- $r$, $g$ squares

#### Logic Gate (Graph Paper Robot II)

*Adjustable Variables*:
- `enum class Gate`: the gates that can be rolled (`And`, `Or`, `Xor`)
- `MEMORY`: the starting queue, currently `0011011000101110`

*Description*:

The player is shown a row of colored squares $(g)$, which are logic gates (purple = OR, gold = AND, blue = XOR;
the description gives each gate's truth table), and a row of red and green circles $(c)$, which is the memory queue (red = 0, green = 1).
For each gate in order, the player takes the first two circles from the queue, evaluates the gate on them,
and puts the result at the end of the queue. This repeats until one circle is left.

The answer is every result in order (15 bits for the 16-bit input).

*RNG*:
- $g$ (the memory $c$ is fixed)

#### TODO: Graph Paper Robot III

Currently unimplemented.

## Utilities

The utilities are declared, with comments, in `puzzle-code/src/include/utils.h`. The most important:

- `seed_t`: a player's seed (`uint64_t`).
- `utils_rng_roll(min, max, seed)`, `utils_chance(percentage, seed)`, `utils_shuffle(items, seed)`: all randomness. They give the same results on every platform.
  **NOTE**: The seed is modified by every call, so adding, removing, or reordering calls
  changes every later value (and the expected passwords in the automated tests).
- `utils_derive_seed(seed, name)`: a separate seed for each puzzle, so puzzles don't share random numbers.
- `utils_seed_from_email(email)`: a player's seed. It ignores case and surrounding whitespace, like the website. Changing it changes every player's puzzle.
- `utils_html_printf(title, desc_filepath, args, extra_head)`: builds a puzzle page, replacing each `%DELIM` with the next argument.
- `utils_generate_file`, `utils_mkdir`, `utils_remove_all`: write files; they do nothing when the `ANS_ONLY` flag (`./main -a`) is set.
- `utils_walkdir`, `utils_zip_entries`, `utils_zip_files`: list a puzzle's files and zip them.

Errors are reported by throwing `std::runtime_error`.

## Graphics

`puzzle-code/src/include/graphics.h` declares `Pixel`, `Image` (a grid of pixels, indexed `img(row, column)`),
and `Svg`, which builds an SVG from `Svg::Rect` and `Svg::Circle` shapes. `graphics_gen_svg_from_image()` turns an `Image`
into an SVG with one square per pixel, each with the class `"<row>.<column>"` so that JavaScript can find it.
`puzzle-code/src/include/maze.h` generates the mazes.

## Web Server

### Description

The main goals of the webpage are puzzle download, user registration, and user tracking.
New users register on `index.php`. Registered users can log in (`login.php`) to return to the download page.
All pages share `includes/header.php`, `includes/footer.php`, and `includes/styles.css`.

Players are saved to `data/contact-data.csv` at the repository root (columns `FName,LName,Email`; see `web-server/includes/players.php`).
It is outside `web-server/`, so the web server can never serve it, and git ignores it.
Set `POINTLESS_PLAYERS_FILE` to store it somewhere else.
Emails are stored trimmed and lowercased, so `Ann@B.com` and `ann@b.com` are the same player (and get the same puzzle).
Names are stored as typed, except that a name starting with `=`, `+`, `-`, `@`, a tab, or a carriage return gets a leading `'`,
so a spreadsheet opening the file shows it as text instead of running it as a formula (`pointless_safe_name()`).

**Security decisions** (2026-09-19):
- **Logging in needs only an email.** Anyone who knows a registered email can download that player's puzzle. This is
  acceptable: the puzzle isn't secret, and the site stores nothing else about the player.
- **No CSRF tokens.** The forms have none. The worst a forged form can do is log someone into another player's download page,
  which the first decision already allows.

The user downloads the puzzle via PHP and plays offline. `download.php` builds a personalized zip on request (see `web-server/includes/generate.php`).
It copies `puzzle-code/production/resources/` into a new temporary directory, runs the production generator there with `-e <email>`
(so the seed comes from `utils_seed_from_email()`), and deletes the temporary directory. `download.php` then streams `puzzle1.zip`,
deleting it before sending it, so a canceled download leaves nothing behind.
The web server never runs `make` and never writes into `puzzle-code/`, and simultaneous downloads don't interfere with each other.
Each session can download at most once every 10 seconds (`POINTLESS_DOWNLOAD_INTERVAL`).

Tracking progress by having users submit hidden tokens is planned but not implemented (see [ideas/tokens.md](ideas/tokens.md)).

### How to Start

A person builds the production generator: once, and again whenever the puzzle code or resources change.
Run `make test` first.

```bash
cd ./pointless-project/puzzle-code
make production
```

This replaces `puzzle-code/production/` with an optimized `src/main` and a clean copy of `resources/`.
Because it is a separate copy, later `make run`, `make test`, and `make clean` don't affect the site.

Start localhost from `web-server/`:

```bash
cd ../web-server
php -S localhost:8000
```

**NOTE**: Keep the terminal running to track all requests going to the web server.

**NOTE**: To use a production generator in another directory, set `POINTLESS_GENERATOR_DIR` (e.g. `POINTLESS_GENERATOR_DIR=/srv/pointless-production php -S localhost:8000`).
That directory must contain `src/main` and `resources/`, laid out as `make production` creates them.

### Web Server Tests

The web-server tests are plain PHP (only `php-cli` is needed) and live in `web-server/tests/`. Run them from anywhere:

```bash
php web-server/tests/run.php          # all tests
php web-server/tests/run.php login    # only tests whose names contain "login"
```

`run.php` starts its own `php -S` on a free port, so no server needs to be running (and one already on port 8000 is not affected).
The server and tests use a temporary directory for the players file (`POINTLESS_PLAYERS_FILE`), a fake puzzle generator
(`POINTLESS_GENERATOR_DIR`), PHP sessions, and `TMPDIR`. The real `data/contact-data.csv` and `puzzle-code/production/` are never changed.
That state is reset before every test. Each test prints `PASS`, `FAIL`, or `SKIP`. If any test fails, the command exits with status 1
and keeps the temporary directory (including `server.log` and `php-errors.log`); otherwise it removes it.

| File | Tests |
|------|-------|
| `lib.php` | Helpers: `check()`, the test server, `Client` (a browser that keeps the session cookie), the fake generator |
| `players-test.php` | `includes/players.php`, called directly |
| `generate-test.php` | `includes/generate.php`, called directly, using the fake generator; `test_generate_real_generator` runs the real one |
| `index-test.php`, `login-test.php`, `download-test.php` | The pages, over HTTP |

The fake generator is a small script written by `fake_generator($mode)`. It records its arguments and working directory,
then (in mode `ok`) writes a `puzzle1.zip` containing `email=<email>`, so tests can check which player's zip was served.
Other modes make it fail, write no zip, or be missing.

`test_generate_real_generator` runs `puzzle-code/production/src/main` (or the tree named by `POINTLESS_TEST_GENERATOR_DIR`)
and checks that the result is a zip containing `instructions.html` and `puzzle2.zip`. It is skipped if there is no `src/main`,
and fails if the tree exists but is broken, since downloads would fail too.

To add a test, write a `test_<page>_<feature>()` function in the matching `*-test.php` file using `check($condition, 'what should be true')`,
and add its name to the `$tests` list in `run.php`. (Use `check()`, not `assert()`: PHP's `assert()` is disabled by default in `php-cli`.)

## Code Coverage

The puzzle generator and tests are compiled with `--coverage` (`make production` is not). In `puzzle-code/`, run the generator
or the tests at least once so there is coverage data, then run `make coverage`:

```bash
cd ./puzzle-code/
make run    # and/or: make test
make coverage
```

This prints the percentage of lines executed in each source file and writes the `.gcov` files to `puzzle-code/build/src/`.
Rebuilding an object deletes its old coverage data (`build/**/*.gcda`), so the counts are for the current code only.

## Automated Tests

From `puzzle-code/`, run:

```bash
make test         # all tests
make test T=bst   # only tests whose names contain "bst"
```

This compiles the tests together with the generator's code (everything in `src/` except `main.cpp`)
into `tests/main`, then runs it from `puzzle-code/tests/`. Each test prints `PASS` or `FAIL` (after the failed checks),
and the command fails if any test fails. CI runs these tests and the web server tests on every push.

Besides one test per puzzle:
- `utils` checks the utilities, including pinned values for the RNG and `utils_seed_from_email()`.
- `seeds` generates games for 200 players and checks that every puzzle has an answer and that players get different puzzles.
- `game_zipfiles` writes a real game and unlocks it layer by layer with the passwords, as a player would: it follows the BST signs
  to the ID and unlocks the rematch zips to check their numbers.

The tests run with the `ANS_ONLY` flag, so only `game_zipfiles` writes files (into `resources/` and `tests/zipfiles/`, which it removes).

### Implementing New Tests

Create `<puzzle name>-puzzle-test.cpp` in `puzzle-code/tests/`. Most puzzle tests are a single `CHECK_PUZZLE`,
which checks the password for each seed, and that the page has the header, the footer, and some text from the description:

```cpp
#include "./include/test.h"

void fib_puzzle_test()
{
  CHECK_PUZZLE(fib_puzzle_create,
               {{1, "13"}, {5, "5"}, {10, "21"}, {test_email_seed(), "8"}},
               {"th number in the fibonacci sequence?"});
}
```

The expected passwords depend on the seed; run `./main -a` in `puzzle-code/src/` to see them, or write the test with
placeholders and copy the values from the failure messages (after checking that they are right).
For other checks, use `CHECK(condition)`, `CHECK_EQ(actual, expected)` (for strings), and `CHECK_THROWS(statement)` from `tests/include/test.h`.
They record a failure and let the test continue.

Then declare `void fib_puzzle_test();` in `puzzle-code/tests/include/test.h`, add `{"fib_puzzle", fib_puzzle_test}` to the
`tests` array in `puzzle-code/tests/main.cpp`, and add the puzzle to `min_different` in `puzzle-code/tests/seeds-test.cpp`.

## Deployment

**Not deployed yet.** The site runs only from `php -S` on a developer's
machine. When it does go live it will share `tools.lipscomb-soc.org` with the
School of Computing tools site and the edna mailer. That box's arrangement —
what lives where, and what deploys own which paths — is documented in the
`dtowell/tools` repo's `readme.md`; read it before putting anything on the
server.

Three constraints that apply there, all of which this repo already satisfies:

- **The generator is built on the server, not shipped to it.** The CI here
  runs on `ubuntu-24.04` and the server is 22.04, so a binary built by CI
  would hit a glibc mismatch. `make production` runs on the server instead.
- **The player file is runtime state and must live outside the deploy tree.**
  `data/contact-data.csv` holds real names and emails, it is rewritten while
  the site runs, and a deploy must never overwrite or expose it.
  `POINTLESS_PLAYERS_FILE` already makes that a configuration choice.
- **`puzzle-code/production/` is generated, not published.**
  `POINTLESS_GENERATOR_DIR` already lets the web server read it from anywhere,
  so it can live outside the document root.

Deploying should reuse the existing mechanism on that box — `ssh
ubuntu@tools.lipscomb-soc.org bin/deploy pointless`, which fetches, checks out
a ref, builds, **runs the tests**, and installs only if they pass — rather
than inventing a fourth way of getting files onto the server.

## Issues

A backlog of cleanup and improvement tasks is in [ideas/cleanup-tasks.md](ideas/cleanup-tasks.md).

- No support for building on macOS
- Issues with accessing the zip files on macOS and Linux without file-roller. It immediately
  prompts for a password even though it should not.
- The puzzle difficulty does not scale smoothly. The earlier puzzles should be harder.
- The *Maze Rematch* puzzle needs a better description.
- Missing required "witty" quotes on all puzzles.
- The *Based Rematch* puzzle is easy to brute force: its answer is an 8-bit number.

## Future Plans

- Design Graph Paper Robot Puzzle III.
- Have an automatic emailer that sends emails to Dr. Towell.
- Have the tokens work with the website, and update the CSV file (see [ideas/tokens.md](ideas/tokens.md)).
- Put the site on `tools.lipscomb-soc.org` (see [Deployment](#deployment)).

## Contributors

Former student developers:

- Zachary Haskins - [GitHub](https://github.com/malloc-nbytes/)
- Turner Austin
- Mekeal Brown - [GitHub](https://github.com/mekealbrown)
- Steven Yassa
- Jordan Hasulube - [GitHub](https://github.com/JordanHassy)
- Michael Hernandez-Lara
- John Tabelisma - [GitHub](https://github.com/johntable)
