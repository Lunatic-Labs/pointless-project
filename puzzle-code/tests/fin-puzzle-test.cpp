#include "./include/test.h"

void fin_puzzle_test()
{
  // The whole description is embedded in the page, so this test follows any rewrite of it. Only the
  // "%TITLE <title>" first line is not part of the body: utils_html_printf() renders it as the <h2>.
  const std::string desc = utils_file_to_str("../resources/files-fin/.desc.html");
  const std::string body = desc.substr(desc.find('\n') + 1);

  CHECK_PUZZLE(fin_puzzle_create,
               {{1, ""}, {5, ""}, {10, ""}, {15, ""}, {test_big_seed(), ""}},
               {"pointless journey", "<h2>Goodbye Pointless</h2>", body});
}
