#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <set>

#include "./include/puzzle.h"
#include "./include/utils.h"
#include "./include/graphics.h"

#define MAZE_WALL {0,0,0}
#define MAZE_PATH {255,255,255}
#define MAZE_CHECKER_PATH {200,200,255};

// TODO: Implement slow spots
// #define MAZE_SLOW_SPOT {200, 100, 0}

#define MAZE_SIZE 13 // Must be an odd number
#define PIXEL_IS_BLACK(p) (p.red + p.green + p.blue == 0)

#define MAZE_END {255, 0, 255};   // Purple
#define MAZE_START {255, 255, 0}; // Gold

std::set<std::pair<int, int>> visited;

static int shortest_path(Image &maze, int x, int y, int steps, std::string &path, long &seed)
{
  if (x < 0 || y < 0 || x >= MAZE_SIZE || y >= MAZE_SIZE) {
    return INT_MAX;
  }

  if (PIXEL_IS_BLACK(maze(x, y))) {
    return INT_MAX;
  }

  if (x == 0 && y == MAZE_SIZE-1) {
    return -1;
  }

  if (visited.find({x, y}) != visited.end()) {
    return INT_MAX;
  }

  visited.insert({x, y});

  int min = INT_MAX;
  int min_dir = -1;
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  for (int i = 0; i < 4; i++) {
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    if (nx < 0 || ny < 0 || nx >= MAZE_SIZE || ny >= MAZE_SIZE) {
      continue;
    }

    if (PIXEL_IS_BLACK(maze(nx, ny))) {
      continue;
    }

    int next = shortest_path(maze, nx, ny, steps+1, path, seed);
    if (next < min) {
      min = next;
      min_dir = i;
    }
  }

  if (min_dir == -1) {
    return INT_MAX;
  }

  switch (min_dir) {
    case 0:
      path += "r";
      break;
    case 1:
      path += "l";
      break;
    case 2:
      path += "d";
      break;
    case 3:
      path += "u";
      break;
  }

  return min+1;
}

static std::string compress_path(std::string &path)
{
  std::string compressed;
  for (size_t i = 0; i < path.size(); i++) {
    int count = 1;
    while (i+1 < path.size() && path[i] == path[i+1]) {
      count++;
      i++;
    }
    compressed += path[i];
    count /= 2;
    if (count > 1) {
      compressed += std::to_string(count);
    }
  }
  return compressed;
}

static void randomized_dfs(Image &maze, int x, int y, long &seed)
{
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  std::shuffle(directions.begin(), directions.end(), std::default_random_engine(seed));
  ++seed;

  maze(x, y) = MAZE_CHECKER_PATH;
  for (auto &dir : directions) {
    int nx = x + dir.first;
    int ny = y + dir.second;
    int fx = x + dir.first*2;
    int fy = y + dir.second*2;

    // Bounds checking
    if (fx < 0 || fy < 0 || fx >= MAZE_SIZE || fy >= MAZE_SIZE) {
      continue;
    }

    if (PIXEL_IS_BLACK(maze(fx, fy))) {
      ++seed; // idk why, but this is necessary for better randomness
      maze(fx, fy) = MAZE_CHECKER_PATH;
      maze(nx, ny) = MAZE_PATH;
      randomized_dfs(maze, fx, fy, seed);
    }
  }
}

Puzzle maze_puzzle_create(long seed)
{
  Image maze(MAZE_SIZE, MAZE_SIZE);

  randomized_dfs(maze, MAZE_SIZE-1, 0, seed);

  std::string path;
  shortest_path(maze, MAZE_SIZE-1, 0, 0, path, seed);

  std::reverse(path.begin(), path.end());
  std::string password = compress_path(path);

  maze(0, MAZE_SIZE-1) = MAZE_START;
  maze(MAZE_SIZE-1, 0) = MAZE_END;

  std::string svg = graphics_gen_svg(maze, 20);
  std::string html_body = utils_html_printf("Maze Puzzle", "./files-maze/.desc.txt", {{svg}});
  utils_generate_file("./files-maze/instructions.html", html_body);

  return {"files-maze", password, {}};
}