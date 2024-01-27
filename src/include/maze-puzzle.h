#ifndef MAZE_PUZZLE_H
#define MAZE_PUZZLE_H

#include "./puzzle.h"

#define MAZE_WALL ({0,0,0})
#define MAZE_PATH ({255,255,255})
#define MAZE_SIZE 16

Puzzle maze_puzzle_create(long seed);

#endif // MAZE_PUZZLE_H