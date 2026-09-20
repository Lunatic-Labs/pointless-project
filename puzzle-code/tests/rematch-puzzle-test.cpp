#include "./include/test.h"

// game_zipfiles_test() also unlocks the rematch zips and checks their numbers.
void rematch_puzzle_test()
{
  CHECK_PUZZLE(rematch_puzzle_create,
               {
                 {1, "465519590", "(rematch1: 1800287, rematch2: lipscomb, rematch3: 216)"},
                 {5, "618344063", "(rematch1: 7262670, rematch2: theory, rematch3: 136)"},
                 {10, "466814493", "(rematch1: 6119226, rematch2: theory, rematch3: 95)"},
                 {15, "941496931", "(rematch1: 9385300, rematch2: theory, rematch3: 65)"},
                 {test_big_seed(), "104405559", "(rematch1: 8941138, rematch2: languages, rematch3: 12)"},
               },
               {"put them together in order"});
}
