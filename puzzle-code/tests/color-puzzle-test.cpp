#include "./include/test.h"

void color_puzzle_test()
{
  CHECK_PUZZLE(color_puzzle_create,
               {{1, "331E54"}, {5, "F4AA00"}, {10, "F4AA00"}, {15, "331E54"}, {test_big_seed(), "F4AA00"}},
               {"An old traveler hands you an abacus",
                "Web colors are defined by three hexadecimal values"});
}
