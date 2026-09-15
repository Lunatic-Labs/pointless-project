#ifndef MAZE_H
#define MAZE_H

#include "./graphics.h"
#include "./utils.h"

// A maze is a square Image with an odd side. Cells are at even rows and columns; the pixels
// between cells are either walls or passages joining neighboring cells.
inline constexpr Pixel MAZE_WALL{0, 0, 0};
inline constexpr Pixel MAZE_CELL{200, 200, 255}; // Also hardcoded (#C8C8FF) in files-rematch-maze/.desc.txt
inline constexpr Pixel MAZE_PASSAGE{255, 255, 255};

// Returns a `size` by `size` maze with exactly one path between any two cells, carved by a
// randomized depth-first search from the bottom-left cell. `size` must be odd.
Image maze_generate(int size, seed_t &seed);

// Returns whether `p` is a wall (black).
bool maze_is_wall(const Pixel &p);

#endif // MAZE_H
