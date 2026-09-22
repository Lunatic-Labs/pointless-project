#include <cstdio>

#include "./include/game.h"

std::vector<Puzzle> game_create_puzzles(seed_t seed)
{
  // Each puzzle gets its own seed, so that no two puzzles share random numbers.
  // Braced lists are evaluated in order, so the puzzles are created in play order.
  return {
    math_puzzle_create(utils_derive_seed(seed, "math")),
    color_puzzle_create(utils_derive_seed(seed, "color")),
    pixel_puzzle_create(utils_derive_seed(seed, "pixel")),
    maze_puzzle_create(utils_derive_seed(seed, "maze")),
    based_intro_puzzle_create(utils_derive_seed(seed, "based-intro")),
    encrypt_puzzle_create(utils_derive_seed(seed, "encrypt")),
    rematch_puzzle_create(utils_derive_seed(seed, "rematch")),
    binary_addition_puzzle_create(utils_derive_seed(seed, "binary-addition")),
    logicgate_puzzle_create(utils_derive_seed(seed, "logicgate")),
    bst_puzzle_create(utils_derive_seed(seed, "bst")),
    fin_puzzle_create(utils_derive_seed(seed, "fin")),
  };
}

std::string game_puzzle_name(const Puzzle &puzzle)
{
  const std::string prefix = "files-";
  size_t found = puzzle.contents_fp.rfind(prefix);
  return found == std::string::npos ? puzzle.contents_fp : puzzle.contents_fp.substr(found + prefix.size());
}

void game_print_passwords(const std::vector<Puzzle> &puzzles)
{
  for (const Puzzle &puzzle : puzzles) {
    std::printf("%-15s Token: %-10s Password: %s", game_puzzle_name(puzzle).c_str(), puzzle.token.c_str(),
                puzzle.password.c_str());
    if (puzzle.extra_info) {
      std::printf("  %s", puzzle.extra_info->c_str());
    }
    std::printf("\n");
  }
}

// Returns `s` as the inside of a JSON string. Nothing generated today needs escaping, but a
// future puzzle whose answer holds a quote or a backslash must not produce invalid JSON.
static std::string json_escape(const std::string &s)
{
  std::string out;
  for (char c : s) {
    switch (c) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n";  break;
      case '\r': out += "\\r";  break;
      case '\t': out += "\\t";  break;
      default:
        if ((unsigned char)c < 0x20) {
          char buf[7];
          std::snprintf(buf, sizeof buf, "\\u%04x", c);
          out += buf;
        } else {
          out += c;
        }
    }
  }
  return out;
}

void game_print_json(seed_t seed, const std::vector<Puzzle> &puzzles)
{
  // Flat enough to write by hand, so the generator keeps libzip as its only dependency.
  std::printf("{\"seed\":\"%llu\",\"puzzles\":[", (unsigned long long)seed);
  for (size_t n = 1; n <= puzzles.size(); n++) {
    const Puzzle &puzzle = puzzles[n - 1];
    std::printf("%s{\"n\":%zu,\"name\":\"%s\",\"password\":\"%s\",\"token\":\"%s\",\"extra\":",
                n == 1 ? "" : ",", n, json_escape(game_puzzle_name(puzzle)).c_str(),
                json_escape(puzzle.password).c_str(), json_escape(puzzle.token).c_str());
    if (puzzle.extra_info) {
      std::printf("\"%s\"", json_escape(*puzzle.extra_info).c_str());
    } else {
      std::printf("null");
    }
    std::printf("}");
  }
  std::printf("]}\n");
}

void game_write_zipfiles(const std::vector<Puzzle> &puzzles, const std::string &zipdir)
{
  utils_mkdir(zipdir);

  // Innermost first, since each zip goes inside the one before it. The password locks the whole
  // zip, not the next zip inside it, so every zip extracts with one password (puzzle1.zip with none)
  // and leaves the next zip, still locked, beside the puzzle's files.
  for (size_t n = puzzles.size(); n >= 1; --n) {
    std::vector<ZipEntry> entries = utils_zip_entries(puzzles[n - 1].contents_fp);
    if (n < puzzles.size()) {
      const std::string next = "puzzle" + std::to_string(n + 1) + ".zip";
      entries.push_back(ZipEntry{zipdir + "/" + next, next});
    }
    const std::string password = n == 1 ? "" : puzzles[n - 2].password;
    utils_zip_files(zipdir + "/puzzle" + std::to_string(n) + ".zip", entries, password);
  }
}
