#include <utility>
#include <vector>

#include "./include/maze.h"

bool maze_is_wall(const Pixel &p)
{
  return p.red == 0 && p.green == 0 && p.blue == 0;
}

// Marks (row, col) as a cell, then, in random order, opens a passage to each neighboring cell
// that hasn't been visited yet and visits it.
static void carve(Image &maze, int row, int col, seed_t &seed)
{
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  utils_shuffle(directions, seed);

  maze(row, col) = MAZE_CELL;
  for (const auto &[dr, dc] : directions) {
    int r = row + 2*dr;
    int c = col + 2*dc;
    if (r < 0 || c < 0 || r >= (int)maze.height || c >= (int)maze.width || !maze_is_wall(maze(r, c))) {
      continue;
    }
    maze(row + dr, col + dc) = MAZE_PASSAGE;
    carve(maze, r, c, seed);
  }
}

Image maze_generate(int size, seed_t &seed)
{
  Image maze(size, size);
  for (Pixel &p : maze.pixels) {
    p = MAZE_WALL;
  }
  carve(maze, size - 1, 0, seed);
  return maze;
}
