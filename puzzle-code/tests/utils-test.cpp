#include <algorithm>
#include <climits>
#include <filesystem>
#include <fstream>
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

static void email_seed_test()
{
  // Pinned, because changing the formula changes every player's puzzle.
  CHECK_EQ(std::to_string(utils_seed_from_email("test@example.com")), "7482486840424827176");
  // Like pointless_normalize_email() in web-server/includes/players.php.
  CHECK(utils_seed_from_email(" \tTest@Example.COM\n") == utils_seed_from_email("test@example.com"));
  CHECK(utils_seed_from_email("test2@example.com") != utils_seed_from_email("test@example.com"));
}

static void files_test(const fs::path &dir)
{
  const std::string desc = (dir / "desc.txt").string();
  std::ofstream(desc) << "a %DELIM b %DELIM c";

  const std::string page = utils_html_printf("Title", desc, {"one", "two"});
  CHECK(page.find("<h2 style=\"text-align:center\">Title</h2>") != std::string::npos);
  CHECK(page.find("a one b two c") != std::string::npos);
  CHECK(page.find(utils_file_to_str("../resources/templates/header.txt")) == 0);
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
  email_seed_test();

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
