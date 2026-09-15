#include "./include/test.h"

void color_puzzle_test()
{
  CHECK_PUZZLE(color_puzzle_create,
               {{1, "331E54"}, {5, "F4AA00"}, {10, "F4AA00"}, {15, "331E54"}, {test_email_seed(), "F4AA00"}},
               {"brute force", "When defining a color in hexadecimal, red is the first two numbers, "});
}
