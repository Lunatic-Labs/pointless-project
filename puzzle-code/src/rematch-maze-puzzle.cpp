/*
 * File: rematch-maze-puzzle.cpp
 * Description:
 *   A harder maze. The page has three mazes, and the player moves by calling JavaScript functions
 *   in the browser console. They pick up a key in the second maze and a battery in the third, then
 *   go to the gold exit in the first maze and call exit(), which shows the password.
 */

#include <string>
#include <vector>

#include "./include/graphics.h"
#include "./include/maze.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define MAZE_SIZE 9 // Must be odd. Also hardcoded into files-rematch-maze/.desc.html

static constexpr Pixel TILE_START{255, 0, 255};  // Purple: where the player starts
static constexpr Pixel TILE_EXIT{255, 255, 0};   // Gold
static constexpr Pixel TILE_DOWN{200, 150, 0};   // Brown: stairs down
static constexpr Pixel TILE_UP{0, 200, 0};       // Green: stairs up
static constexpr Pixel TILE_ITEM{250, 150, 150}; // Pink

// The numbers in the mazes' JavaScript arrays. Must match files-rematch-maze/.desc.html.
enum Cell {
  CELL_WALL = 0,
  CELL_EMPTY = 1,
  CELL_START = 2,
  CELL_END = 3,
  CELL_DESCEND = 4,
  CELL_ASCEND = 5,
  CELL_PICKUP_KEY = 6,
  CELL_PICKUP_BATTERY = 7,
};

// Colors the tile at (row, col) of `maze` and records it in `cells`.
static void set_tile(Image &maze, std::vector<int> &cells, int row, int col, Pixel color, Cell cell)
{
  maze(row, col) = color;
  cells[row*MAZE_SIZE + col] = cell;
}

// Returns `cells` as the rows of a JavaScript array.
static std::string cells_to_js(const std::vector<int> &cells)
{
  std::string res = "\n";
  for (int i = 0; i < MAZE_SIZE; i++) {
    res += "    [";
    for (int j = 0; j < MAZE_SIZE; j++) {
      res += std::to_string(cells[i*MAZE_SIZE + j]) + ",";
    }
    res += "],\n";
  }
  return res + "  ";
}

Puzzle rematch_maze_puzzle_create(seed_t seed)
{
  const int password = utils_rng_roll(1000000, 9999999, seed);
  const int password_key = utils_rng_roll(1000000, 9999999, seed);

  const int last = MAZE_SIZE - 1;
  const int middle = MAZE_SIZE / 2;
  strvec_t svgs;
  strvec_t arrays;
  for (int i = 0; i < 3; ++i) {
    Image maze = maze_generate(MAZE_SIZE, seed);
    std::vector<int> cells(maze.pixels.size());
    for (size_t p = 0; p < cells.size(); p++) {
      cells[p] = maze_is_wall(maze.pixels[p]) ? CELL_WALL : CELL_EMPTY;
    }

    switch (i) {
      case 0: // The player starts at the bottom left and exits at the top right.
        set_tile(maze, cells, last, 0, TILE_START, CELL_START);
        set_tile(maze, cells, 0, last, TILE_EXIT, CELL_END);
        set_tile(maze, cells, 0, 0, TILE_DOWN, CELL_DESCEND);
        break;
      case 1:
        set_tile(maze, cells, 0, 0, TILE_UP, CELL_ASCEND);
        set_tile(maze, cells, last, last, TILE_DOWN, CELL_DESCEND);
        set_tile(maze, cells, middle, middle, TILE_ITEM, CELL_PICKUP_KEY);
        break;
      case 2:
        set_tile(maze, cells, last, last, TILE_UP, CELL_ASCEND);
        set_tile(maze, cells, middle, middle, TILE_ITEM, CELL_PICKUP_BATTERY);
        break;
    }

    Svg svg = graphics_gen_svg_from_image(maze, 40, {}); // Also hardcoded into the HTML
    svg.add_shape(Svg::Circle(-200, -200, 15, "#F4AA00", "#000000", {}, "player"));
    svgs.push_back(svg.build(false));
    arrays.push_back(cells_to_js(cells));
  }

  // The page shows (password ^ password_key) ^ password_key when the player escapes.
  std::string html_body = utils_html_printf("../resources/files-rematch-maze/.desc.html",
                                            {svgs[0], svgs[1], svgs[2], arrays[0], arrays[1], arrays[2],
                                             std::to_string(password ^ password_key), std::to_string(password_key)});
  utils_generate_file("../resources/files-rematch-maze/instructions.html", html_body);
  return {"../resources/files-rematch-maze", html_body, std::to_string(password), "", {}};
}
