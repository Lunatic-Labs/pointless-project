#include "./include/puzzle.h"
#include "./include/utils.h"

#define MAX_LOOP 3// if i can modify the html somehow... 

static int get_pixel_count(const std::string& hexValue) {
    static std::unordered_map<std::string, int> pixelCounts = {
        {"#FFFFFF", 2},
        {"#000000", 30},
        {"#F4AA00", 6},
        {"#331E54", 66},
        {"#5A3A60", 183}
    };

    auto it = pixelCounts.find(hexValue);
    if (it != pixelCounts.end()) {
        return it->second;
    }
    return 0; // not found?? not sure how that could happen
}

Puzzle pixel_puzzle_create(long seed)
{
  std::vector<std::string> hex_vals = {"#FFFFFF", "#000000", "#F4AA00", "#331E54", "#5A3A60"};
  std::vector<std::string> delim_values; 
 
  for (int i = 0; i < MAX_LOOP; i++) {
    int prod = 1;
    for (int j = 0; j < i + 1; j++) {
      int rand = utils_rng_roll(0, hex_vals.size() - 1, seed);
      delim_values.push_back(hex_vals[rand]); 
      prod *= get_pixel_count(hex_vals[rand]);
    }
    delim_values.push_back(std::to_string(prod));
  }

  std::string html = utils_html_printf("Pixel Puzzle", "./files-pixel/.desc.txt", {delim_values}); 
  utils_generate_file("./files-pixel/instructions.html", html);
  std::string answer = delim_values.back();

  return {"files-pixel", std::move(answer), {}};
}