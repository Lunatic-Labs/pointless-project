#include <cctype>
#include <cmath>
#include <filesystem>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <zip.h>

#include "./include/test.h"
#include "../src/include/game.h"

namespace {

bool starts_with(const std::string &s, const std::string &prefix)
{
  return s.compare(0, prefix.size(), prefix) == 0;
}

bool ends_with(const std::string &s, const std::string &suffix)
{
  return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Opens a zip held in memory. `data` must outlive the returned archive.
zip_t *open_zip(const std::string &data)
{
  zip_error_t error;
  zip_error_init(&error);
  zip_t *archive = nullptr;
  if (zip_source_t *source = zip_source_buffer_create(data.data(), data.size(), 0, &error)) {
    archive = zip_open_from_source(source, ZIP_RDONLY, &error);
    if (!archive) {
      zip_source_free(source);
    }
  }
  zip_error_fini(&error);
  return archive;
}

// Returns how many of the zip's entries are encrypted.
zip_int64_t encrypted_entries(zip_t *archive)
{
  zip_int64_t count = 0;
  for (zip_int64_t i = 0; i < zip_get_num_entries(archive, 0); i++) {
    zip_stat_t stat;
    if (zip_stat_index(archive, i, 0, &stat) == 0 && stat.encryption_method != ZIP_EM_NONE) {
      count++;
    }
  }
  return count;
}

std::set<std::string> entry_names(zip_t *archive)
{
  std::set<std::string> names;
  for (zip_int64_t i = 0; i < zip_get_num_entries(archive, 0); i++) {
    names.insert(zip_get_name(archive, i, 0));
  }
  return names;
}

// Returns entry `name`, decrypted with `password` unless it is null, or nullopt if it can't be read.
std::optional<std::string> read_entry(zip_t *archive, const std::string &name, const char *password)
{
  zip_stat_t stat;
  if (zip_stat(archive, name.c_str(), 0, &stat) != 0) {
    return std::nullopt;
  }
  zip_file_t *file = password ? zip_fopen_encrypted(archive, name.c_str(), 0, password)
                              : zip_fopen(archive, name.c_str(), 0);
  if (!file) {
    return std::nullopt;
  }
  std::string data(stat.size, '\0');
  zip_int64_t read = zip_fread(file, data.data(), stat.size);
  if (zip_fclose(file) != 0 || read != (zip_int64_t)stat.size) {
    return std::nullopt;
  }
  return data;
}

// Evaluates an integer expression with + - * /, parentheses, and √, as written on the BST
// puzzle's signs. Returns nullopt if it can't, including when a division or square root isn't exact.
class Evaluator {
public:
  explicit Evaluator(std::string text) : s(std::move(text)) {}

  std::optional<long> evaluate()
  {
    long value = sum();
    skip_spaces();
    if (!ok || pos != s.size()) {
      return std::nullopt;
    }
    return value;
  }

private:
  std::string s;
  size_t pos = 0;
  bool ok = true;

  void skip_spaces()
  {
    while (pos < s.size() && s[pos] == ' ') {
      pos++;
    }
  }

  bool eat(const std::string &token)
  {
    skip_spaces();
    if (s.compare(pos, token.size(), token) != 0) {
      return false;
    }
    pos += token.size();
    return true;
  }

  long factor()
  {
    if (eat("√")) {
      long value = factor();
      long root = value < 0 ? -1 : std::lround(std::sqrt((double)value));
      ok = ok && root * root == value;
      return root;
    }
    if (eat("(")) {
      long value = sum();
      ok = ok && eat(")");
      return value;
    }
    skip_spaces();
    size_t start = pos;
    while (pos < s.size() && std::isdigit((unsigned char)s[pos])) {
      pos++;
    }
    if (start == pos) {
      ok = false;
      return 0;
    }
    return std::stol(s.substr(start, pos - start));
  }

  long product()
  {
    long value = factor();
    while (ok) {
      if (eat("*")) {
        value *= factor();
      } else if (eat("/")) {
        long divisor = factor();
        ok = ok && divisor != 0 && value % divisor == 0;
        value = ok ? value / divisor : 0;
      } else {
        break;
      }
    }
    return value;
  }

  long sum()
  {
    long value = product();
    while (ok) {
      if (eat("+")) {
        value += product();
      } else if (eat("-")) {
        value -= product();
      } else {
        break;
      }
    }
    return value;
  }
};

// Returns which way ('l' or 'r') the BST sign `text` points for `x`, or nullopt if it can't be followed.
std::optional<char> follow_sign(const std::string &text, long x)
{
  const std::string prefix = "if ";
  const std::string suffix = ", then go right\nelse, go left";
  if (!starts_with(text, prefix) || !ends_with(text, suffix) || text.size() < prefix.size() + suffix.size()) {
    return std::nullopt;
  }
  const std::string condition = text.substr(prefix.size(), text.size() - prefix.size() - suffix.size());

  std::smatch m;
  if (std::regex_match(condition, m, std::regex("x > (\\d+)"))) {
    return x > std::stol(m[1]) ? 'r' : 'l';
  }
  if (std::regex_match(condition, m, std::regex("([0-9A-F]+) is x in hexadecimal"))) {
    return std::stol(m[1], nullptr, 16) == x ? 'r' : 'l';
  }
  if (ends_with(condition, " = x")) {
    std::optional<long> value = Evaluator(condition.substr(0, condition.size() - 4)).evaluate();
    if (value) {
      return *value == x ? 'r' : 'l';
    }
  }
  return std::nullopt;
}

// Checks that every sign in the BST puzzle's tree can be followed, and that following them from
// the top leads along the puzzle's path to an ID.txt holding the password.
void check_bst_layer(zip_t *archive, const std::set<std::string> &names, const Puzzle &puzzle)
{
  std::smatch m;
  const bool found_x = std::regex_search(puzzle.contents_html, m, std::regex("is: <b>(\\d+)</b>"));
  test_check(found_x, "the BST page shows x", __FILE__, __LINE__);
  if (!found_x) {
    return;
  }
  const long x = std::stol(m[1]);

  int signs = 0;
  int unreadable = 0;
  for (const std::string &name : names) {
    if (!starts_with(name, "tree/") || ends_with(name, "/ID.txt")) {
      continue;
    }
    signs++;
    std::optional<std::string> text = read_entry(archive, name, nullptr);
    if ((!text || !follow_sign(*text, x)) && ++unreadable <= 5) {
      test_check(false, "can follow the sign " + name + (text ? ": " + *text : ""), __FILE__, __LINE__);
    }
  }
  test_check(signs == (1 << 10) - 1, "the BST tree has 1023 signs (has " + std::to_string(signs) + ")", __FILE__, __LINE__);

  std::string dir = "tree";
  std::string path;
  while (true) {
    if (std::optional<std::string> id = read_entry(archive, dir + "/ID.txt", nullptr)) {
      CHECK_EQ(*id, puzzle.password);
      break;
    }
    std::string sign;
    for (const std::string &name : names) {
      if (starts_with(name, dir + "/") && name.find('/', dir.size() + 1) == std::string::npos) {
        sign = name;
      }
    }
    std::optional<std::string> text = sign.empty() ? std::nullopt : read_entry(archive, sign, nullptr);
    std::optional<char> way = text ? follow_sign(*text, x) : std::nullopt;
    if (!way || path.size() > 20) {
      test_check(false, "can follow the signs from " + dir, __FILE__, __LINE__);
      break;
    }
    path += *way;
    dir += *way == 'r' ? "/right" : "/left";
  }
  CHECK_EQ("Path: " + path, puzzle.extra_info.value_or("(none)"));
}

// Unlocks each rematch's password zip with its puzzle's password and checks that the numbers
// inside, in order, are the rematch password.
void check_rematch_layer(zip_t *archive, const Puzzle &puzzle)
{
  const std::string extra_info = puzzle.extra_info.value_or("(none)");
  std::smatch passwords;
  const bool found = std::regex_match(extra_info, passwords,
                                      std::regex("\\(rematch1: (\\S+), rematch2: (\\S+), rematch3: (\\S+)\\)"));
  test_check(found, "the rematch puzzle's extra_info lists the rematch passwords: " + extra_info, __FILE__, __LINE__);
  if (!found) {
    return;
  }

  std::string numbers;
  for (size_t i = 0; i < 3; i++) {
    const std::string n = std::to_string(i + 1);
    const std::string password = passwords[i + 1];
    std::optional<std::string> data = read_entry(archive, "rematch" + n + ".zip", nullptr);
    zip_t *rematch = data ? open_zip(*data) : nullptr;
    test_check(rematch != nullptr, "the rematch layer has rematch" + n + ".zip", __FILE__, __LINE__);
    if (!rematch) {
      continue;
    }

    const std::string locked = "password" + n + ".zip";
    const std::string piece = "password" + n + ".txt";
    test_check(encrypted_entries(rematch) == 0, "rematch" + n + ".zip is not encrypted", __FILE__, __LINE__);
    test_check(read_entry(rematch, "instructions.html", nullptr).has_value(),
               "rematch" + n + ".zip has instructions.html", __FILE__, __LINE__);
    std::optional<std::string> locked_data = read_entry(rematch, locked, nullptr);
    zip_discard(rematch);
    zip_t *number = locked_data ? open_zip(*locked_data) : nullptr;
    test_check(number != nullptr, "rematch" + n + ".zip has " + locked, __FILE__, __LINE__);
    if (!number) {
      continue;
    }

    test_check(encrypted_entries(number) == zip_get_num_entries(number, 0), locked + " is encrypted", __FILE__, __LINE__);
    test_check(!read_entry(number, piece, nullptr), piece + " can't be read without the password", __FILE__, __LINE__);
    std::optional<std::string> text = read_entry(number, piece, password.c_str());
    std::smatch m;
    if (text && std::regex_search(*text, m, std::regex("Your number is (\\d{3})\\."))) {
      numbers += m[1];
    } else {
      test_check(false, "rematch " + n + "'s password unlocks " + piece + ", which has a number", __FILE__, __LINE__);
    }
    zip_discard(number);
  }
  CHECK_EQ(numbers, puzzle.password);
}

} // namespace

// Writes a real game (the other tests only compute answers) and unlocks it layer by layer, as a player would.
// Each zip must be locked as a whole by one password (see utils_zip_files()).
void game_zipfiles_test()
{
  const std::string zipdir = "zipfiles";
  const uint32_t old_flags = FLAGS;
  FLAGS &= ~ANS_ONLY;
  std::vector<Puzzle> puzzles;
  try {
    puzzles = game_create_puzzles(test_big_seed());
    game_write_zipfiles(puzzles, zipdir);
  } catch (...) {
    FLAGS = old_flags;
    throw;
  }
  FLAGS = old_flags;

  std::string data = utils_file_to_str(zipdir + "/puzzle1.zip");
  for (size_t n = 1; n <= puzzles.size(); n++) {
    const Puzzle &puzzle = puzzles[n - 1];
    const std::string layer = "puzzle" + std::to_string(n) + ".zip";
    const std::string next = "puzzle" + std::to_string(n + 1) + ".zip";

    zip_t *archive = open_zip(data);
    test_check(archive != nullptr, "can open " + layer, __FILE__, __LINE__);
    if (!archive) {
      break;
    }
    const std::set<std::string> names = entry_names(archive);

    if (n == 1) {
      test_check(encrypted_entries(archive) == 0, layer + " is not encrypted", __FILE__, __LINE__);
    } else {
      test_check(encrypted_entries(archive) == zip_get_num_entries(archive, 0),
                 "every entry in " + layer + " is encrypted", __FILE__, __LINE__);
      test_check(!read_entry(archive, "instructions.html", nullptr), layer + " can't be read without a password", __FILE__, __LINE__);
      test_check(!read_entry(archive, "instructions.html", "wrong password"),
                 layer + " can't be read with a wrong password", __FILE__, __LINE__);
      // The previous puzzle's password unlocks this layer; the checks below read it as the default.
      zip_set_default_password(archive, puzzles[n - 2].password.c_str());
    }
    test_check(read_entry(archive, "instructions.html", nullptr) == puzzle.contents_html,
               layer + " opens and has the puzzle's instructions.html", __FILE__, __LINE__);
    if (game_puzzle_name(puzzle) == "rematch") {
      check_rematch_layer(archive, puzzle);
    }
    if (game_puzzle_name(puzzle) == "bst") {
      check_bst_layer(archive, names, puzzle);
    }

    std::optional<std::string> inner;
    if (n == puzzles.size()) {
      test_check(names.count(next) == 0, "the last zip has no " + next, __FILE__, __LINE__);
    } else {
      inner = read_entry(archive, next, nullptr);
      test_check(inner.has_value(), layer + " has " + next, __FILE__, __LINE__);
    }
    zip_discard(archive);
    if (!inner) {
      break;
    }
    data = *inner;
  }

  std::filesystem::remove_all(zipdir);
}
