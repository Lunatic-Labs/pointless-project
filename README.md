# Pointless Project
A website to generate and distribute puzzles to prospective Lipscomb recruits

# Requirements

## Debian/Ubuntu based distros

```
sudo apt install g++ libboost-all-dev libzip-dev
```

## Arch based distros

```
sudo pacman -S gcc
sudo pacman -S boost
sudo pacman -S libzip
```

## MacOS

```
brew install gcc
arch -x86_64 brew install zlib
arch -x86_64 brew install libzip
```

**See how to install the boost library below**

*Using Brew and Rosetta Intel emulator*

Before installing Homebrew you will need to install Rosetta2 emulator for the new ARM silicon (M1 chip). Install Rosetta2 via terminal using:
`/usr/sbin/softwareupdate --install-rosetta --agree-to-license`

After installing Rosetta2 above you can then use the Homebrew cmd and install Homebrew for ARM M1 chip.
`arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`

Once Homebrew for M1 ARM is installed use this Homebrew command to install packages:

Boost library:
`arch -x86_64 brew install boost`

# Building
```
cd ./src/
make
./main
```

## Structure of Output

Upon running `./main`, it will populate the `zipfiles` directory.
The zipfiles in here will be the generated output from the program.

Each zip file is put into this directory, but if you want to view
the nested zips, just open `puzzle1.zip` and follow the instructions.

# Development

*Note*:

When adding a new puzzle, create a new files-`puzzle name` directory. Inside of here,
put any files that it needs (ie configuration files, premade files) and prefix
the name of the file with `.`. The `walkdir()` function will ignore any
dotfiles/hidden directories. It will also explore any subdirectories that is inside
of the puzzle directory that you have created. If any of the files are not prefixed with `.`,
then that file will be added to the final zipped version of it.

## Creating a New Puzzle

### Puzzle Requirements Directory

Start by creating a new directory in ./src/ and name it `files-<new puzzle name>`. Then inside of there,
create a new file called `.desc.txt`. This is where the instructions, hints, and other info about the puzzle is stored.
Everything in this file will be put into an HTML file, so make sure that it adheres to HTML rules.

In order to transfer information easily from C++ to HTML/Javascript, we use a special delimiter in this file, namely `%DELIM`.
For example, if I need to pass a password and an array that is generated in C++ into this `.desc.txt` file, I would do something like:

```html
<script>
  let password = %DELIM;
  let array = [%DELIM];
</script>
```

It is then the job of C++ to "stringify" the required information to pass to `.desc.txt`.

In order to make the explanation more easy to follow, I will make a new puzzle called "fib", where the point(less) of it is to
have the user find the *n*th number in the fibonacci sequence.

```
cd ./src/
mkdir files-fib && cd files-fib
echo "What is the <b>%DELIM</b>th number in the fibonacci sequence?" > .desc.txt
```

### Puzzle Implementation

Once the above step is done, create a new file called `<puzzle name>-puzzle.cpp` in `./src/` and put the entrypoint of the
function that will create the puzzle.

```cpp
Puzzle fib_puzzle_create(long seed)
{
  return {"files-fib", "changeme", {}}; // NOTE: "files-fib" is the puzzle requirements directory from the previous step.
}
```

The return value of this function is a `Puzzle` object. It is defined as:

```cpp
struct Puzzle {
  // The filepath to the appropriate directory
  // that contains all of the information needed
  // for the puzzle.
  std::string contents_fp;

  std::string password;

  std::optional<std::string> extra_info;
};
```

So we are returning a new puzzle where
- requirements = "files-fib"
- password = "changeme"
- extra_info = None

The `password` field is what will set the password for the next zipfile layer. The `extra_info` field is purely for debugging
and is strictly used for the side effect of printing when running the application. Nothing that gets put into it will be
displayed/used in the final puzzle in the zipfiles.

Now it's time to actually create the solver for the puzzle. I will create a fib function that will find the number.

```cpp
#include "./include/puzzle.h"
#include "./include/utils.h" // utils_rng_roll, utils_html_printf, utils_generate_file

static int fib(int n)
{
  if (n < 2) return n;
  return fib(n-1) + fib(n-2);
}

Puzzle fib_puzzle_create(long seed)
{
  int fibnum = utils_rng_roll(3, 10, seed); // get a random number in the range 3..=10.
  int password = fib(fibnum);

  std::string html_content = utils_html_printf("Fibonacci Sequence", "./files-fib/.desc.txt", {std::to_string(fibnum)});
  utils_generate_file("./files-fib/instructions.html", html_content);
  return {"files-fib", std::to_string(password), {}};
}
```

Now that the implementation is done, add the signature of `fib_puzzle_create` to ./src/include/puzzle.h.

### Using The Puzzle

In the `main` function in `main.cpp`, there is some code that looks like:

```cpp
std::vector<Puzzle> puzzles = {
  math_puzzle_create(seed),
  color_puzzle_create(seed),
  pixel_puzzle_create(seed),
  maze_puzzle_create(seed),
  encrypt_puzzle_create(seed),
  based_puzzle_create(seed),
  ast_puzzle_create(seed),
  fin_puzzle_create(seed),
};
```

Put the new puzzle that was created in the spot that you want it to appear in the nested zipfiles.
For example, if I want it to be the third puzzle that the user solves, I would do:

```cpp
std::vector<Puzzle> puzzles = {
  math_puzzle_create(seed),
  color_puzzle_create(seed),
  fib_puzzle_create(seed), // Added it here
  pixel_puzzle_create(seed),
  maze_puzzle_create(seed),
  encrypt_puzzle_create(seed),
  based_puzzle_create(seed),
  ast_puzzle_create(seed),
  fin_puzzle_create(seed),
};
```

Now run `make all` and these things will happen:
1. The files-\<puzzle name\> directories will all generate a file called "instructions.html".
2. `./src/zipfiles/` will be populated with zipfiles.

The entire project will be located in `./src/zipfiles/puzzle1.zip`, as it contains the zipped files puzzle1..puzzleN.
The reason why we also generate the other puzzles outside of `puzzle1.zip`, is because it allows for easy testing
without having to go through the entire zipfile structure.

## Other Notes

`./include/utils.h` and `utils.cpp` contains helpful utility functions. Feel free to add more to here if you would like.

`./include/graphics.h` and `graphics.cpp` contains functions for working with graphics, namely SVG and PPM. It also has a
struct for a pixel and an image.

# Future Plans

TODO


