/*
 * File: based-intro-puzzle.cpp
 * Author: Jordan Hasulube
 * Contact: jdhasulube@mail.lipscomb.edu
 * Date: 10/2/2024
 * Description:
 *   The user is shown a "light box": a number in base 16 with one column per digit (least
 *   significant on the left) and one lit light per column. The password is the digits written
 *   in the same order, for example 210 for the light box of 7 in base 3.
 */

#include <sstream>
#include <string>
#include <vector>

#include "./include/puzzle.h"
#include "./include/utils.h"

#define BASE 16  // Must match files-based-intro/.desc.txt
#define LENGTH 8 // Must match files-based-intro/.desc.txt

// Returns a light box table with `base` rows showing `digits` (least significant first).
static std::string create_table(const std::vector<int> &digits, int base)
{
  std::stringstream table;
  table << "    <table class='number'>\n";
  table << "        <tbody>\n";
  for (int value = base - 1; value >= 0; value--) {
    table << "            <tr>\n";
    for (int digit : digits) {
      if (digit != value) {
        table << "                <td>⦿</td>\n";
      } else if (value == 0) {
        table << "                <td style='color: rgb(255, 0, 0);'>⦿</td>\n";
      } else {
        table << "                <td style='color: rgb(141, 255, 141);'>⦿</td>\n";
      }
    }
    table << "            </tr>\n";
  }
  table << "        </tbody>\n";
  table << "    </table>";
  return table.str();
}

Puzzle based_intro_puzzle_create(seed_t seed)
{
  const char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::vector<int> digits;
  std::string password;
  for (int i = 0; i < LENGTH; i++) {
    // The last (most significant) digit isn't 0, so every column is needed.
    int digit = utils_rng_roll(i == LENGTH - 1 ? 1 : 0, BASE - 1, seed);
    digits.push_back(digit);
    password += symbols[digit];
  }

  std::string html_content = utils_html_printf("Base Intro Puzzle", "../resources/files-based-intro/.desc.txt",
                                               {create_table(digits, BASE)});
  utils_generate_file("../resources/files-based-intro/instructions.html", html_content);
  return {"../resources/files-based-intro", html_content, password, {}};
}
