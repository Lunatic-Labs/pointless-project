#include "./include/test.h"

void rematch_encrypt_puzzle_test()
{
  CHECK_PUZZLE(rematch_encrypt_puzzle_create,
               {{1, "algorithm"}, {5, "binary"}, {10, "computer"}, {15, "languages"}, {test_big_seed(), "theory"}},
               {"All we have are the magic machines that encrypted the password."});
}
