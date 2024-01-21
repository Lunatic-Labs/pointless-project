#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "./include/html-generator.h"
#include "./include/utils.h"
#include "./include/puzzle.h"

typedef std::vector<std::string> strvec_t;

static long glbl_seed;

void generate_file(std::string &output_filepath, std::string &output_body)
{
  std::ofstream outfp(output_filepath);
  outfp << output_body;
  outfp.close();
}

int roll_seed(void)
{
  long seed = (uint)time(nullptr);
  srand(seed);
  return seed;
}

void create_final_zipfile(const std::vector<Puzzle> &puzzles)
{
  const char *zipdir = "zipfiles/puzzle";
  int i = puzzles.size();

  // Need to go in reverse order to zip the files correctly.
  for (auto puzzle = puzzles.rbegin(); puzzle != puzzles.rend(); ++puzzle, --i) {
    std::vector<std::string> files = walkdir(puzzle->contents_fp);

    // Adds the next level of zipped puzzle (technically the previous one beause reverse order)
    if(puzzle != puzzles.rbegin()) {
      files.insert(files.begin(), zipdir + std::to_string(i + 1) + ".zip");
    }
    zip_files(std::string(zipdir + std::to_string(i) + ".zip"), files, puzzle->password);
  }
}

int main(void)
{
  glbl_seed = roll_seed();

  std::vector<Puzzle> puzzles;
  puzzles.push_back(construct_puzzle1(glbl_seed));
  puzzles.push_back(construct_puzzle2(glbl_seed));
  puzzles.push_back(construct_puzzle3(glbl_seed));
  puzzles.push_back(construct_puzzle4(glbl_seed));
  create_final_zipfile(puzzles);

  return 0;
}