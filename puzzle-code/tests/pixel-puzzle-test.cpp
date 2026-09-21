#include <map>
#include <regex>

#include "./include/test.h"

void pixel_puzzle_test()
{
  CHECK_PUZZLE(pixel_puzzle_create,
               {{1, "3960"}, {5, "9900"}, {10, "9900"}, {15, "1800"}, {test_big_seed(), "3960"}},
               {"color wheel", "Lou seems a little off today… or maybe it’s just you?"});

  // The pixel counts must match the bison in the header, and the password must be the last row's product.
  std::map<std::string, int> counts;
  const std::string header = utils_file_to_str("../resources/templates/header.html");
  const std::regex fill("fill=\"(#[0-9A-Fa-f]{6})\"");
  for (auto it = std::sregex_iterator(header.begin(), header.end(), fill); it != std::sregex_iterator(); ++it) {
    std::string hex = (*it)[1];
    for (char &c : hex) {
      c = (char)toupper((unsigned char)c);
    }
    counts[hex]++;
  }

  const std::regex row("<li>((?:#[0-9A-F]{6}(?: × )?)+) = ([0-9?]+)</li>");
  const std::regex color("#[0-9A-F]{6}");
  for (seed_t seed = 1; seed <= 50; seed++) {
    const Puzzle puzzle = pixel_puzzle_create(seed);
    int rows = 0;
    for (auto it = std::sregex_iterator(puzzle.contents_html.begin(), puzzle.contents_html.end(), row);
         it != std::sregex_iterator(); ++it, ++rows) {
      const std::string colors = (*it)[1];
      const std::string result = (*it)[2];
      long product = 1;
      for (auto c = std::sregex_iterator(colors.begin(), colors.end(), color); c != std::sregex_iterator(); ++c) {
        product *= counts[c->str()];
      }
      const std::string what = "row " + std::to_string(rows + 1) + " for seed " + std::to_string(seed);
      test_check_eq(std::to_string(product), result == "?" ? puzzle.password : result, what, __FILE__, __LINE__);
    }
    test_check(rows == 3, "the page for seed " + std::to_string(seed) + " has 3 rows", __FILE__, __LINE__);
  }
}
