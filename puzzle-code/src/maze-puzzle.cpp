/*
 * File: maze-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Date: 2/4/2024
 * Description:
 *   The user is presented with a generated maze. The goal is to find the
 *   path from the gold spot to the purple spot. The password is the
 *   compressed path from the start to the end, for example uudlllr -> u2dl3r.
 */

#include <string>

#include "./include/graphics.h"
#include "./include/inventory.h"
#include "./include/maze.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define MAZE_SIZE 13 // Must be odd. Also hardcoded into files-maze/.desc.html

static constexpr Pixel MAZE_START{0xF4, 0xAA, 0x00}; // Gold, bottom left
static constexpr Pixel MAZE_END{0x33, 0x1E, 0x54};   // Purple, top right

// Appends one letter (u, d, l, or r) per pixel moved on the way from (row, col) to the top-right
// cell, never stepping back to (from_row, from_col). Returns whether it got there.
// A generated maze has no loops, so this finds its only path.
static bool find_path(const Image &maze, int row, int col, int from_row, int from_col, std::string &path)
{
  if (row == 0 && col == MAZE_SIZE - 1) {
    return true;
  }

  static const struct { int dr, dc; char letter; } moves[] = {
    {0, 1, 'r'}, {0, -1, 'l'}, {1, 0, 'd'}, {-1, 0, 'u'},
  };
  for (const auto &move : moves) {
    int r = row + move.dr;
    int c = col + move.dc;
    if (r < 0 || c < 0 || r >= MAZE_SIZE || c >= MAZE_SIZE || (r == from_row && c == from_col) || maze_is_wall(maze(r, c))) {
      continue;
    }
    path += move.letter;
    if (find_path(maze, r, c, row, col, path)) {
      return true;
    }
    path.pop_back();
  }
  return false;
}

// Run-length encodes `path`, halving each run because a move from one cell to the next is two
// pixels: "uuuurr" (two cells up, one right) becomes "u2r".
static std::string compress_path(const std::string &path)
{
  std::string compressed;
  for (size_t i = 0; i < path.size();) {
    size_t run = 1;
    while (i + run < path.size() && path[i + run] == path[i]) {
      run++;
    }
    compressed += path[i];
    if (run / 2 > 1) {
      compressed += std::to_string(run / 2);
    }
    i += run;
  }
  return compressed;
}

Puzzle maze_puzzle_create(seed_t seed)
{
  Image maze = maze_generate(MAZE_SIZE, seed);

  std::string path;
  find_path(maze, MAZE_SIZE - 1, 0, -1, -1, path);

  maze(MAZE_SIZE - 1, 0) = MAZE_START;
  maze(0, MAZE_SIZE - 1) = MAZE_END;

  std::string svg_html = graphics_gen_svg_from_image(maze, 20, {}).build(false);
  const std::string token = utils_token(utils_derive_seed(seed, "token"));
  std::string html_body = utils_html_printf("../resources/files-maze/.desc.html", {svg_html}, token,
                                            inventory_html("maze"));
  utils_generate_file("../resources/files-maze/instructions.html", html_body);
  return {"../resources/files-maze", html_body, compress_path(path), token, {}};
}
