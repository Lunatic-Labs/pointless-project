#include "./include/test.h"

void based_intro_puzzle_test()
{
  CHECK_PUZZLE(based_intro_puzzle_create,
               {{1, "87523"}, {5, "86987"}, {10, "35148"}, {15, "83177"}, {test_big_seed(), "35589"}},
               {"Good job encoding those instructions!", "the artifact below means 5 + 0 + 3&times;49 = 152"});
}
