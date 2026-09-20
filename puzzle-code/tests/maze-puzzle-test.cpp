#include "./include/test.h"

void maze_puzzle_test()
{
  CHECK_PUZZLE(maze_puzzle_create,
               {
                 {1, "r2ululu2rdrdrurdrdru4l2dl2ul2ur6"},
                 {5, "r2ul2ur3d2rurdru2lurulu2r"},
                 {10, "ru2rd2rurdr2ulurul2dlul3ur2u2rdrurd2ru2"},
                 {15, "r3u2rdr2ulu2ru2"},
                 {test_big_seed(), "u2r3dldr2u2r2u4"},
               },
               {"Lou's pixel art", "For every sequence of the same consecutive letter, compress it by using the letter "});
}
