#include "./include/maze-puzzle.h"
#include "./include/puzzle.h"
#include "./include/utils.h"
#include "./include/graphics.h"

// void force_unique_shortest_path(Image &img)
// {
//   (void)img;
//   assert(false && "unimplemented");
// }

Puzzle maze_puzzle_create(long seed)
{
  std::string html_content = utils_html_printf("Maze Puzzle", "./files-maze/.desc.txt", {});
  utils_generate_file("./files-maze/instructions.html", html_content);

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

  graphics_create_ppm(img, "./files-maze/maze.ppm");

  return Puzzle{"files-maze", "uru6r3u3ld"};
}