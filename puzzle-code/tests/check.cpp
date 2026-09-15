#include <iostream>

#include "./include/test.h"

static int failures = 0;

void test_check(bool ok, const std::string &what, const char *file, int line)
{
  if (!ok) {
    failures++;
    std::cout << "  " << file << ":" << line << ": failed: " << what << std::endl;
  }
}

void test_check_eq(const std::string &actual, const std::string &expected, const std::string &what,
                   const char *file, int line)
{
  if (actual != expected) {
    failures++;
    std::cout << "  " << file << ":" << line << ": " << what << " is \"" << actual
              << "\", expected \"" << expected << "\"" << std::endl;
  }
}

int test_failures(void)
{
  return failures;
}

seed_t test_email_seed(void)
{
  return utils_seed_from_email("test@example.com");
}

void test_puzzle(Puzzle (*create)(seed_t), const std::vector<Expected> &expected, const strvec_t &snippets,
                 const char *file, int line)
{
  const std::string header = utils_file_to_str("../resources/templates/header.txt");
  const std::string footer = utils_file_to_str("../resources/templates/footer.txt");

  for (const Expected &e : expected) {
    const Puzzle puzzle = create(e.seed);
    const std::string &html = puzzle.contents_html;
    const std::string seed = "seed " + std::to_string(e.seed);

    test_check_eq(puzzle.password, e.password, "password for " + seed, file, line);
    if (e.extra_info) {
      test_check_eq(puzzle.extra_info.value_or("(none)"), *e.extra_info, "extra_info for " + seed, file, line);
    }
    test_check(html.compare(0, header.size(), header) == 0, "page for " + seed + " starts with the header", file, line);
    test_check(html.size() >= footer.size() && html.compare(html.size() - footer.size(), footer.size(), footer) == 0,
               "page for " + seed + " ends with the footer", file, line);
    for (const std::string &snippet : snippets) {
      test_check(html.find(snippet) != std::string::npos, "page for " + seed + " contains \"" + snippet + "\"", file, line);
    }
  }
}
