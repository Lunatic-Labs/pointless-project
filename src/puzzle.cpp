#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "./include/utils.h"
#include "./include/puzzle.h"
#include "./include/graphics.h"

#define BOLD(s) "<b>" + (s) + "</b>"

Puzzle puzzle_create1(long seed)
{
  // TODO: Change in final version to bigger numbers.
  int a = utils_rng_roll(1, 5, seed);
  int b = utils_rng_roll(6, 9, seed);
  int s = a+b;
  std::string description = utils_file_to_str("./puzzle1/.desc.txt");
  std::string question = BOLD("what is " + std::to_string(a) + "+" + std::to_string(b) + "?");
  description.append(question);
  
  std::string html_content = utils_generate_html("Hello Pointless", description, seed);

  utils_generate_file("./puzzle1/instructions.html", html_content);
  return Puzzle{"puzzle1", std::to_string(s)};
}

Puzzle puzzle_create2(long seed)
{
  std::string description = utils_file_to_str("./puzzle2/.desc.txt");
  std::string html_content = utils_generate_html("Color Puzzle", description, seed);
  utils_generate_file("./puzzle2/instructions.html", html_content);
  return Puzzle{"puzzle2", "331E54F4AA00"};
}

#define MAZE_WALL ({0,0,0})
#define MAZE_PATH ({255,255,255})
#define MAZE_SIZE 16

void force_unique_shortest_path(Image &img)
{
  (void)img;
  assert(false && "unimplemented");
}

Puzzle puzzle_create3(long seed)
{
  std::string description = utils_file_to_str("./puzzle3/.desc.txt");
  std::string html_content = utils_generate_html("Maze Puzzle", description, seed);
  utils_generate_file("./puzzle3/instructions.html", html_content);

  int s = MAZE_SIZE;
  Image img = Image {(size_t)s, (size_t)s};
  for(int i = 0; i < s; i++) {
    for(int j = 0; j < s; j++) {
      img(i, j) = i%2 ? Pixel MAZE_PATH : Pixel MAZE_WALL;
    }
  }

  for(int i = 0; i < s; i++) {
    img(i, utils_rng_roll(0,s-1,seed+i)) = Pixel MAZE_PATH;
    img(i, utils_rng_roll(0,s-1,seed+i*s)) = Pixel MAZE_PATH;
  }

  img(0, 0) = Pixel {255, 0, 255};
  img(s-1, s-1) = Pixel {255, 255, 0};

  img = graphics_scale_ppm(img, 70);

  graphics_create_ppm(img, "./puzzle3/maze.ppm");

  return Puzzle{"puzzle3", "uru6r3u3ld"};
}

Puzzle puzzle_create4(long seed)
{
  (void)seed;
  return Puzzle{"puzzle4", ""};
}
