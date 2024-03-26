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

#define MAZE_SIZE 9 // Must be an odd number
#define PIXEL_IS_BLACK(p) (p.red + p.green + p.blue == 0)

#define MAZE_END {255, 0, 255};   // Purple
#define MAZE_START {255, 255, 0}; // Gold
#define MAZE_DOWN {200,150,0};
#define MAZE_UP {0,200,0};
#define MAZE_ITEM {250,150,150};

/*** Binary maze macros ***/
#define AT(i, j) ((i) * MAZE_SIZE + (j))
#define CELL_WALL 0
#define CELL_EMPTY 1
#define CELL_START 2
#define CELL_END 3
#define CELL_DESCEND 4
#define CELL_ASCEND 5
#define CELL_PICKUP_KEY 6
#define CELL_PICKUP_BATTERY 7

void rematch_randomized_dfs(Image &maze, std::vector<int> &binmaze, int x, int y, long &seed)
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
      binmaze[AT(fx, fy)] = 1;
      binmaze[AT(nx, ny)] = 1;
      rematch_randomized_dfs(maze, binmaze, fx, fy, seed);
    }
  }
}

std::string vector_mat_to_str(std::vector<int> &v)
{
  std::string res = "\n";
  for (int i = 0; i < MAZE_SIZE; i += 1) {
    res += "    [";
    for (int j = 0; j < MAZE_SIZE; j += 1) {
      res += std::to_string(v[i*MAZE_SIZE+j]) + ",";
    }
    res += "],\n";
  }
  res += "  ";
  return res;
}

Puzzle rematch_maze_puzzle_create(long seed)
{
  std::vector<std::vector<int>> mazes = {};
  std::vector<std::string> svgs = {};
  std::string password = "changeme";

  for (int i = 0; i < 3; ++i) {
    Image maze(MAZE_SIZE, MAZE_SIZE);

    std::vector<int> binmaze = {};
    binmaze.resize(MAZE_SIZE*MAZE_SIZE, 0); // Set all to 0

    rematch_randomized_dfs(maze, binmaze, MAZE_SIZE-1, 0, seed);

    switch (i) {
      // Only the first maze should have start and end spots.
      case 0: {
        maze(0, MAZE_SIZE-1) = MAZE_START;
        maze(MAZE_SIZE-1, 0) = MAZE_END;
        maze(0, 0) = MAZE_DOWN;

        binmaze[AT(MAZE_SIZE-1, 0)] = CELL_START;
        binmaze[AT(0, MAZE_SIZE-1)] = CELL_END;
        binmaze[AT(0, 0)] = CELL_DESCEND;
      } break;
      case 1: {
        maze(0, 0) = MAZE_UP;
        maze(MAZE_SIZE-1, MAZE_SIZE-1) = MAZE_DOWN;
        maze(MAZE_SIZE/2, MAZE_SIZE/2) = MAZE_ITEM;

        binmaze[AT(0, 0)] = CELL_ASCEND;
        binmaze[AT(MAZE_SIZE-1, MAZE_SIZE-1)] = CELL_DESCEND;
        binmaze[AT(MAZE_SIZE/2, MAZE_SIZE/2)] = CELL_PICKUP_KEY;
      } break;
      case 2: {
        maze(MAZE_SIZE-1, MAZE_SIZE-1) = MAZE_UP;
        maze(MAZE_SIZE/2, MAZE_SIZE/2) = MAZE_ITEM;

        binmaze[AT(MAZE_SIZE-1, MAZE_SIZE-1)] = CELL_ASCEND;
        binmaze[AT(MAZE_SIZE/2, MAZE_SIZE/2)] = CELL_PICKUP_BATTERY;
      } break;
    }

    std::string svg = graphics_gen_svg(maze, 40);

    svgs.push_back(svg);
    mazes.push_back(binmaze);
  }

  std::string binmaze_str1 = vector_mat_to_str(mazes[0]);
  std::string binmaze_str2 = vector_mat_to_str(mazes[1]);
  std::string binmaze_str3 = vector_mat_to_str(mazes[2]);

  std::string html_body = utils_html_printf("Maze Rematch Puzzle",
                                            "./files-rematch-maze/.desc.txt",
                                            {svgs[0], svgs[1], svgs[2], binmaze_str1, binmaze_str2, binmaze_str3});

  utils_generate_file("./files-rematch-maze/instructions.html", html_body);

  return {"files-maze", password, {}};
}
