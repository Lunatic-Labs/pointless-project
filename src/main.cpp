#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./include/html-generator.h"
#include "./include/utils.h"
#include "./include/puzzle.h"

int roll_seed(void)
{
  long seed = (uint)time(nullptr);
  srand(seed);
  return seed;
}

void create_nested_zipfiles(std::vector<Puzzle> &puzzles)
{
  const char *zipdir = "zipfiles/puzzle";
  int i = puzzles.size();

  // Need to go in reverse order to zip the files correctly.
  for (auto puzzle = puzzles.rbegin(); puzzle != puzzles.rend(); ++puzzle, --i) {
    std::vector<std::string> files = utils_walkdir(puzzle->contents_fp);

    // Adds the next level of zipped puzzle (technically the previous one beause reverse order).
    if(puzzle != puzzles.rbegin()) {
      files.insert(files.begin(), zipdir + std::to_string(i+1) + ".zip");
    }
    utils_zip_files(std::string(zipdir + std::to_string(i) + ".zip"), files, puzzle->password);
  }
}

int main(void)
{
  long seed = roll_seed();

  std::vector<Puzzle> puzzles = {
    puzzle_create1(seed),
    puzzle_create2(seed),
    puzzle_create3(seed),
    puzzle_create4(seed),
  };

  create_nested_zipfiles(puzzles);

  return 0;
}
