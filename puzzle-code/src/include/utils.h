#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

// Bits of FLAGS.
constexpr uint32_t ANS_ONLY = 1 << 0; // Only compute answers: write no files, directories, or zips.
extern uint32_t FLAGS;

typedef std::vector<std::string> strvec_t;
typedef const std::string filepath_t;

// A player's seed. Every random choice in a puzzle comes from it through utils_rng_roll().
typedef uint64_t seed_t;

// Writes `output_body` to `filepath`. Does nothing under ANS_ONLY. Throws std::runtime_error on failure.
void utils_generate_file(filepath_t filepath, const std::string &output_body);

// Creates the directory `filepath` and its parents. Does nothing under ANS_ONLY.
void utils_mkdir(filepath_t filepath);

// Removes `filepath` and everything under it, if it exists. Does nothing under ANS_ONLY.
void utils_remove_all(filepath_t filepath);

// Returns a random number from `min` to `max` (inclusive, in either order) and advances `seed`.
// The numbers depend only on the seed, not on the platform or compiler.
// NOTE: Adding, removing, or reordering calls changes every later number.
int utils_rng_roll(int min, int max, seed_t &seed);

// Returns true `percentage` percent of the time.
bool utils_chance(int percentage, seed_t &seed);

// Shuffles `items` (a vector or string) using utils_rng_roll().
template <class T>
void utils_shuffle(T &items, seed_t &seed)
{
  for (size_t i = items.size(); i > 1; --i) {
    std::swap(items[i - 1], items[utils_rng_roll(0, (int)i - 1, seed)]);
  }
}

// Returns a seed for the part of a game named `name`, so that parts made from the same seed don't
// share random numbers.
seed_t utils_derive_seed(seed_t seed, const std::string &name);

// Returns an unpredictable seed.
seed_t utils_roll_seed(void);

// The characters a token is made of, and how many of them a token has.
// The alphabet leaves out every pair that is easy to misread in a sans-serif font
// (0/O/Q, 1/I, 2/Z, 5/S, 6/G, 8/B, U/V), so a player can copy a token off the page by eye.
extern const std::string TOKEN_ALPHABET;
constexpr int TOKEN_LENGTH = 8;

// Returns a puzzle page's token: TOKEN_LENGTH characters from TOKEN_ALPHABET, rolled from `seed`.
// The seed is taken by value, so a puzzle can call utils_token(utils_derive_seed(seed, "token"))
// without disturbing its own rolls.
std::string utils_token(seed_t seed);

// Returns the paths of all files under `path`, recursively, sorted.
// Skips files and directories whose names start with `.`.
strvec_t utils_walkdir(filepath_t path);

// A file to put in a zip.
struct ZipEntry {
  std::string path;       // The file on disk
  std::string name;       // Its name in the zip
  bool encrypted = false; // Whether to encrypt it with the zip's password
};

// Returns an unencrypted entry for each file under `dir` (see utils_walkdir()), named by its path relative to `dir`.
std::vector<ZipEntry> utils_zip_entries(filepath_t dir);

// Writes the zip `out_file_name`, replacing any existing file, holding `entries`.
// Entries marked `encrypted` are encrypted with traditional PKWARE (ZipCrypto) using `password`, which
// is weak but opens in the zip tools built into Windows and macOS. Throws std::runtime_error on failure.
void utils_zip_files(filepath_t out_file_name, const std::vector<ZipEntry> &entries, const std::string &password);

// Returns the contents of `filepath`. Throws std::runtime_error if it can't be read.
std::string utils_file_to_str(filepath_t filepath);

// Returns a puzzle page: resources/templates/header.html, then a <section> holding the title in an
// <h2> and the description, then `inventory` (see inventory_html()) and `token`, each in a block of
// its own and each left out when it is empty, and resources/templates/footer.html.
// `desc_filepath` holds the whole page in one file: its first line is "%TITLE <title>" and the rest
// is the body, with each %PARAM replaced by the next of `args`.
// Throws std::runtime_error without that first line, or unless the body has exactly args.size() %PARAMs.
std::string utils_html_printf(filepath_t desc_filepath, const strvec_t &args, const std::string &token = "",
                              const std::string &inventory = "");

#endif // UTILS_H
