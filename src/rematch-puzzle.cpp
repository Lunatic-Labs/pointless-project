#include "./include/puzzle.h"
#include "./include/utils.h"

#include <iostream>
#include <string>
#include <cassert>

void create_rematch_zipfiles(std::vector<Puzzle> &puzzles)
{
  std::string zipdir = "files-rematch/";
  int i = 0;

  for (auto puzzle = puzzles.begin(); puzzle != puzzles.end(); ++puzzle, i++) {
    std::vector<std::string> files = utils_walkdir(puzzle->contents_fp);
    files.insert(files.begin(), zipdir + ".passwords/password" + std::to_string(i+1) + ".txt");

    utils_zip_files(std::string(zipdir + "rematch" + std::to_string(i+1) + ".zip"), files, puzzle->password);
  }
}

std::string create_rematch_password_files(std::vector<Puzzle> &puzzles, long seed)
{
  const std::string password_dir = "files-rematch/.passwords/";
  std::string password = "";
  int i = 0;

  utils_mkdir(password_dir);

  for (auto puzzle = puzzles.begin(); puzzle != puzzles.end(); ++puzzle, i++) {
    std::string password_part = std::to_string(utils_rng_roll(0, 999, seed));
    password += password_part;
    std::string password_fp = password_dir + "password" + std::to_string(i+1) + ".txt";
    utils_generate_file(password_fp, password_part);
  }

  return password;
}

Puzzle rematch_puzzle_create(long seed)
{
  std::vector<Puzzle> puzzles = {
    math_puzzle_create(seed),
    math_puzzle_create(seed),
    math_puzzle_create(seed),
  };

  std::string password = create_rematch_password_files(puzzles, seed);
  create_rematch_zipfiles(puzzles);

  return {"files-rematch", password, {}};
}

