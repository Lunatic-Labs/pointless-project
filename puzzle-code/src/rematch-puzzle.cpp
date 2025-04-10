#include "./include/puzzle.h"
#include "./include/utils.h"

#include <iostream>
#include <string>
#include <cassert>

static void create_rematch_zipfiles(std::vector<Puzzle> &puzzles)
{
  std::string zipdir = "../html-txt/files-rematch/";
  int i = 0;

  for (auto puzzle = puzzles.begin(); puzzle != puzzles.end(); ++puzzle, i++) {
    if (i != 0) {
      std::vector<std::string> files = utils_walkdir(puzzle->contents_fp);
      files.insert(files.begin(), zipdir + ".passwords/password" + std::to_string(i) + ".txt");

      utils_zip_files(std::string(zipdir + "rematch" + std::to_string(i) + ".zip"), files, puzzle->password);
    }
  }
}

static std::string create_rematch_password_files(std::vector<Puzzle> &puzzles, long seed)
{
  const std::string password_dir = "../html-txt/files-rematch/.passwords/";
  std::string password = "";
  int i = 0;

  utils_mkdir(password_dir);

  for (auto puzzle = puzzles.begin(); puzzle != puzzles.end(); ++puzzle, i++) {
    if (i != 0)  {
      std::string password_part = std::to_string(utils_rng_roll(0, 999, seed));
      password += password_part;
      std::string password_fp = password_dir + "password" + std::to_string(i) + ".txt";
      utils_generate_file(password_fp, password_part);
    }
  }

  return password;
}

Puzzle rematch_puzzle_inst(long seed)
{
  (void)seed;
  std::string html_content = utils_html_printf("Rematch Instructions", "../html-txt/files-rematch/.desc.txt", {});
  utils_generate_file("../html-txt/files-rematch/instructions.html", html_content);
  return {"../html-txt/files-rematch", html_content, "", {}};
}

Puzzle rematch_puzzle_create(long seed)
{
  std::vector<Puzzle> puzzles = {
    rematch_puzzle_inst(seed),
    rematch_maze_puzzle_create(seed),
    rematch_encrypt_puzzle_create(seed),
    rematch_based_puzzle_create(seed),
    // math_puzzle_create(seed),
    // math_puzzle_create(seed),
  };

  std::string password = create_rematch_password_files(puzzles, seed);
  if (!(FLAGS & ANS_ONLY)) {
    create_rematch_zipfiles(puzzles);

    std::cout << "Rematch:" << std::endl;
    for (auto &puzzle : puzzles) {
      std::string puzzle_name = puzzle.contents_fp;
      puzzle_name.erase(puzzle_name.begin(), puzzle_name.begin()+12);
      std::printf("  %-17s Password: %s", puzzle_name.substr(6).c_str(), puzzle.password.c_str());
      if (puzzle.extra_info) {
        std::printf(" %s", puzzle.extra_info->c_str());
      }
      std::cout << std::endl;
    }
  }

  return {"../html-txt/files-rematch", "", password, {}};
}
