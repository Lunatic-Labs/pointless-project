# Pointless Project
A website to generate and distribute puzzles to prospective Lipscomb recruits

# Requirements

## Debian/Ubuntu based distros

```
sudo apt install g++
sudo apt-get install libboost-all-dev
sudo apt-get install libzip-dev
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

*see how to install the bost library below*

*Using Brew and Rosetta Intel emulator*
Before installing Homebrew you will need to install Rosetta2 emulator for the new ARM silicon (M1 chip). Install Rosetta2 via terminal using:
`/usr/sbin/softwareupdate --install-rosetta --agree-to-license`

After installing Rosetta2 above you can then use the Homebrew cmd and install Homebrew for ARM M1 chip.
`arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`

Once Homebrew for M1 ARM is installed use this Homebrew command to install packages:

Boost library:
`arch -x86_64 brew install boost`

*MacOS*
```arch -x86_64 brew install libzip```

# Building
```
cd ./src/
./build <optional flag>
```
where
* `c` - clean
* `r` - run
* `a` - clean, build, and run

## Structure of Output

Upon running `./main`, it will populate the `zipfiles` directory.
The zipfiles in here will be the generated output from the program.

Each zip file is put into this directory, but if you want to view
the nested zips, just open `puzzle1.zip` and follow the instructions.

# Development

## New Puzzle

When adding a new puzzle, create a new files-`puzzle name` directory. Inside of here,
put any files that it needs (ie configuration files, premade files) and prefix
the name of the file with `.`. The `walkdir()` function will ignore any
dotfiles/hidden directories. It will also explore any subdirectories that is inside
of the puzzle directory that you have created. If any of the files are not prefixed with `.`,
then that file will be added to the final zipped version of it.

