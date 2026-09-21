#include "./include/test.h"

void math_puzzle_test()
{
  CHECK_PUZZLE(math_puzzle_create,
               {{1, "99"}, {5, "127"}, {10, "74"}, {15, "83"}, {test_big_seed(), "72"}},
               {"way to access the next puzzle."});
}
