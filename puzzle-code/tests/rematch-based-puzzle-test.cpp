#include "./include/test.h"

void rematch_based_puzzle_test()
{
  CHECK_PUZZLE(rematch_based_puzzle_create,
               {{1, "207"}, {5, "27"}, {10, "2"}, {15, "71"}, {test_big_seed(), "22"}},
               {"<li>Numbers <b>A</b> through <b>F</b> are represented as usual in <b>base-16</b>.</li>"});
}
