/*
 * File: based-intro-puzzle.cpp
 * Author: Jordan Hasulube
 * Date: 10/2/2024
 * Description:
 *   The user is shown a "light box": a number in base 19 with one column per digit (least
 *   significant on the left) and one lit light per column. The password is that number written
 *   in decimal, for example 14 for the light box whose columns show 2, 3 and 0 in base 4.
 */

#include <sstream>
#include <string>
#include <vector>

#include "./include/inventory.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define BASE 19  // Must match files-based-intro/.desc.html
#define LENGTH 4 // Must match files-based-intro/.desc.html

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
  std::vector<int> digits;
  int value = 0;
  int place = 1;
  for (int i = 0; i < LENGTH; i++) {
    // The last (most significant) digit isn't 0, so every column is needed.
    int digit = utils_rng_roll(i == LENGTH - 1 ? 1 : 0, BASE - 1, seed);
    digits.push_back(digit);
    value += digit * place;
    place *= BASE;
  }
  const std::string password = std::to_string(value);

  const std::string token = utils_token(utils_derive_seed(seed, "token"));
  std::string html_content = utils_html_printf("../resources/files-based-intro/.desc.html",
                                               {create_table(digits, BASE)}, token, inventory_html("based-intro"));
  utils_generate_file("../resources/files-based-intro/instructions.html", html_content);
  return {"../resources/files-based-intro", html_content, password, token, {}};
}
