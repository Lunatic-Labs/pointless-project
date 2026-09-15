/*
 * File: rematch-puzzle.cpp
 * Description:
 *   The rematch: harder versions of the maze, encrypt, and based puzzles. Each is its own zip,
 *   rematchN.zip, in this puzzle's directory. Rematch N's password unlocks passwordN.txt in its
 *   zip, which holds a 3-digit number. The three numbers in order are this puzzle's password.
 */

#include <cstdio>
#include <string>
#include <vector>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define REMATCH_DIR "../resources/files-rematch"

Puzzle rematch_puzzle_create(seed_t seed)
{
  const std::vector<Puzzle> rematches = {
    rematch_maze_puzzle_create(utils_derive_seed(seed, "rematch-maze")),
    rematch_encrypt_puzzle_create(utils_derive_seed(seed, "rematch-encrypt")),
    rematch_based_puzzle_create(utils_derive_seed(seed, "rematch-based")),
  };
  const std::string count = std::to_string(rematches.size());

  const std::string number_dir = REMATCH_DIR "/.passwords";
  utils_mkdir(number_dir);

  std::string password;
  std::string extra_info;
  for (size_t i = 0; i < rematches.size(); i++) {
    const std::string n = std::to_string(i + 1);

    char number[16];
    std::snprintf(number, sizeof number, "%03d", utils_rng_roll(0, 999, seed));
    password += number;

    const std::string number_file = number_dir + "/password" + n + ".txt";
    utils_generate_file(number_file, "You solved rematch " + n + " of " + count + "! Your number is " + number + ".\n");
    if (!(FLAGS & ANS_ONLY)) {
      std::vector<ZipEntry> entries = utils_zip_entries(rematches[i].contents_fp);
      entries.insert(entries.begin(), ZipEntry{number_file, "password" + n + ".txt", true});
      utils_zip_files(REMATCH_DIR "/rematch" + n + ".zip", entries, rematches[i].password);
    }

    extra_info += (i == 0 ? "(" : ", ") + std::string("rematch") + n + ": " + rematches[i].password;
  }
  extra_info += ")";

  std::string html = utils_html_printf("Rematch Instructions", REMATCH_DIR "/.desc.txt", {});
  utils_generate_file(REMATCH_DIR "/instructions.html", html);
  return {REMATCH_DIR, html, password, extra_info};
}
