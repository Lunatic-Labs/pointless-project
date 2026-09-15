#include "./include/test.h"

void binary_addition_puzzle_test()
{
  CHECK_PUZZLE(binary_addition_puzzle_create,
               {{1, "10100101"}, {5, "01001000"}, {10, "11010100"}, {15, "10010100"}, {test_email_seed(), "10100110"}},
               {"master key", "<li><span class=\"state_red\"></span>E:↓A</li>"});
}
