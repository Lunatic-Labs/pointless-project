# Structure

## Building

Clean the build:
```
./build.sh c
```

Build normally:
```
./build.sh
```

Build clean then build normally:
```
./build.sh a
```

## Structure of Output

Upon running `./main`, it will populate the `zipfiles` directory.
The zipfiles in here will be the generated output from the program.

Each zip file is put into this directory, but if you want to view
the nested zips, just open `puzzle1.zip` and follow the instructions.

# Development

## New Puzzle

When adding a new puzzle, create a new `puzzle`N directory. Inside of here,
put any files that it needs (ie configuration files, premade files) and prefix
the name of the file with `.`. The `walkdir()` function will ignore any
dotfiles/hidden directories. It will also explore any subdirectories that is inside
of the puzzle directory that you have created. If any of the files are not prefixed with `.`,
then that file will be added to the final zipped version of it.
