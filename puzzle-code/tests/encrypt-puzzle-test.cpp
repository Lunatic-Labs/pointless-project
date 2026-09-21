#include "./include/test.h"

void encrypt_puzzle_test()
{
  CHECK_PUZZLE(encrypt_puzzle_create,
               {
                 {1, "breadth_first_search"},
                 {5, "big_o_notation"},
                 {10, "dynamic_programming"},
                 {15, "greedy_algorithms"},
                 {test_big_seed(), "breadth_first_search"},
               },
               {"a single green bulb rolls loose", "Here are the steps he used:"});
}
