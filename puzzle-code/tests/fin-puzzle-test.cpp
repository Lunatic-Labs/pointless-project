#include "./include/test.h"

void fin_puzzle_test()
{
  CHECK_PUZZLE(fin_puzzle_create,
               {{1, ""}, {5, ""}, {10, ""}, {15, ""}, {test_big_seed(), ""}},
               {"pointless journey", utils_file_to_str("../resources/files-fin/.desc.txt")});
}
