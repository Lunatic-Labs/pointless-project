#include "./include/test.h"

void logicgate_puzzle_test()
{
  CHECK_PUZZLE(logicgate_puzzle_create,
               {
                 {1, "010101100101111"},
                 {5, "011101101001011"},
                 {10, "000101110110011"},
                 {15, "011101000010010"},
                 {test_big_seed(), "001101010011011"},
               },
               {"ejects the battery that got it this far",
                "<li>Always take the <b>first two circles</b> from the queue to calculate the next value.</li>"});
}
