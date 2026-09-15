#include "./include/test.h"

void based_intro_puzzle_test()
{
  CHECK_PUZZLE(based_intro_puzzle_create,
               {{1, "17EB9054"}, {5, "A8755411"}, {10, "A6D88A56"}, {15, "50372837"}, {test_email_seed(), "8D7AAA79"}},
               {"speedy maze", "To represent this structure in text form, we can simply pretend it is a number"});
}
