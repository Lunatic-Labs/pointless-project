#include "./include/test.h"

void rematch_maze_puzzle_test()
{
  CHECK_PUZZLE(rematch_maze_puzzle_create,
               {{1, "6822465"}, {5, "5358618"}, {10, "7483466"}, {15, "4975941"}, {test_email_seed(), "4004104"}},
               {"You vaguely recall that typing <code>instructions</code> into the <i><u><b>console</b></u></i> "});
}
