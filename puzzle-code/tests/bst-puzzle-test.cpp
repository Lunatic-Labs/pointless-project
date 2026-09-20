#include "./include/test.h"

// game_zipfiles_test() also follows the signs in a generated tree, as a player would.
void bst_puzzle_test()
{
  CHECK_PUZZLE(bst_puzzle_create,
               {
                 {1, "9942472", "Path: rrlrrrlrrl"},
                 {5, "5425921", "Path: rlllrlllll"},
                 {10, "4959287", "Path: rrrrrrrlll"},
                 {15, "5471974", "Path: llrrlrrrrr"},
                 {test_big_seed(), "7775386", "Path: rllllrrlll"},
               },
               {"queue of logic gates", "You will need to look at the mathematical street signs to find your way to x."});
}
