#include <algorithm>
#include <climits>
#include <filesystem>
#include <fstream>
#include <set>
#include <unistd.h>

#include "./include/test.h"

namespace fs = std::filesystem;

static void rng_test()
{
  // Pinned, because changing these numbers changes every player's puzzle.
  seed_t seed = 1;
  CHECK_EQ(std::to_string(utils_rng_roll(1, 1000000, seed)), "822466");
  CHECK_EQ(std::to_string(utils_rng_roll(1, 1000000, seed)), "428520");
  CHECK_EQ(std::to_string(utils_rng_roll(1, 1000000, seed)), "890591");

  seed_t a = 42;
  seed_t b = 42;
  bool same = true;
  for (int i = 0; i < 100; i++) {
    same = same && utils_rng_roll(0, 9, a) == utils_rng_roll(0, 9, b);
  }
  CHECK(same);

  // Inclusive, in either order, and fine over the whole int range.
  bool in_range = true;
  bool saw_min = false;
  bool saw_max = false;
  for (int i = 0; i < 1000; i++) {
    int n = utils_rng_roll(5, 3, seed);
    in_range = in_range && n >= 3 && n <= 5;
    saw_min = saw_min || n == 3;
    saw_max = saw_max || n == 5;
  }
  CHECK(in_range);
  CHECK(saw_min && saw_max);
  utils_rng_roll(INT_MIN, INT_MAX, seed);
  CHECK(utils_rng_roll(7, 7, seed) == 7);

  std::string letters = "abcdefghij";
  seed_t shuffle_seed = 7;
  utils_shuffle(letters, shuffle_seed);
  CHECK_EQ(letters, "ibfjaedcgh");

  CHECK(utils_derive_seed(1, "math") != utils_derive_seed(1, "maze"));
  CHECK(utils_derive_seed(1, "math") != utils_derive_seed(2, "math"));
  CHECK(utils_derive_seed(1, "math") == utils_derive_seed(1, "math"));
}

static void token_test()
{
  // Pinned: a player's tokens are frozen in their stored game, so changing these numbers makes
  // the tokens on already generated pages disagree with the ones a new game would show.
  CHECK_EQ(utils_token(1), "M4AKKRPE");
  CHECK_EQ(utils_token(2), "JF979W39");
  CHECK_EQ(utils_token(1), "M4AKKRPE");

  // Only characters that can't be mistaken for each other: no 0/O/Q, 1/I, 2/Z, 5/S, 6/G, 8/B, or U.
  CHECK(TOKEN_ALPHABET.find_first_of("01256"
                                     "BGIOQSUZ") == std::string::npos);

  std::set<std::string> tokens;
  bool right_shape = true;
  for (int i = 0; i < 1000; i++) {
    const std::string token = utils_token(utils_derive_seed(i, "token"));
    right_shape = right_shape && (int)token.size() == TOKEN_LENGTH
                  && token.find_first_not_of(TOKEN_ALPHABET) == std::string::npos;
    tokens.insert(token);
  }
  CHECK(right_shape);
  CHECK(tokens.size() == 1000);
}

static void files_test(const fs::path &dir)
{
  const std::string desc = (dir / "desc.txt").string();
  std::ofstream(desc) << "a %DELIM b %DELIM c";

  const std::string page = utils_html_printf("Title", desc, {"one", "two"});
  CHECK(page.find("<h2>Title</h2>") != std::string::npos);
  CHECK(page.find("a one b two c") != std::string::npos);
  CHECK(page.find(utils_file_to_str("../resources/templates/header.txt")) == 0);

  // The token block is added only when there is a token, and goes after the description.
  CHECK(page.find("class=\"token\"") == std::string::npos);
  const std::string with_token = utils_html_printf("Title", desc, {"one", "two"}, "ACDEFHJK");
  CHECK(with_token.find("<code>ACDEFHJK</code>") != std::string::npos);
  CHECK(with_token.find("class=\"token\"") > with_token.find("a one b two c"));
  CHECK_THROWS(utils_html_printf("Title", desc, {"one"}));
  CHECK_THROWS(utils_html_printf("Title", desc, {"one", "two", "three"}));
  CHECK_THROWS(utils_file_to_str((dir / "missing.txt").string()));

  // Zip entries skip names starting with '.', are sorted, and are named relative to the directory.
  const fs::path tree = dir / "tree";
  fs::create_directories(tree / "sub");
  fs::create_directories(tree / ".hidden-dir");
  for (const char *name : {"b.txt", "a.txt", ".hidden", "sub/c.txt", ".hidden-dir/d.txt"}) {
    std::ofstream(tree / name) << name;
  }
  std::string names;
  for (const ZipEntry &entry : utils_zip_entries(tree.string())) {
    names += entry.name + (entry.encrypted ? "(encrypted) " : " ");
  }
  CHECK_EQ(names, "a.txt b.txt sub/c.txt ");

  CHECK_THROWS(utils_zip_files((dir / "out.zip").string(), {{(dir / "missing.txt").string(), "missing.txt"}}, ""));
}

void utils_test()
{
  rng_test();
  token_test();

  const fs::path dir = fs::temp_directory_path() / ("pointless-utils-test-" + std::to_string(getpid()));
  fs::create_directories(dir);
  try {
    files_test(dir);
  } catch (...) {
    fs::remove_all(dir);
    throw;
  }
  fs::remove_all(dir);
}
