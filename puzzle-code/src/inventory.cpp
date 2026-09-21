#include <stdexcept>
#include <vector>

#include "./include/inventory.h"

namespace {

struct Item {
  const char *puzzle; // The puzzle that awards it, named as in game_puzzle_name()
  const char *item;   // What the page hands the player
  const char *fate;   // What it turned out to be good for; shown only in the final inventory
};

// In play order (see game_create_puzzles()). The first six were already awarded in the pages'
// own text; the rest were added so that the gag fires on every layer instead of half of them.
// A puzzle's item is handed over for solving it, so it shows up in the inventory on the *next*
// page, the way the prose already awards it there. The last row is the MacGuffin the fin page
// hands over itself, so it heads no later page and is never listed.
const std::vector<Item> ITEMS = {
  {"math",            "an abacus",                                   "never used"},
  {"color",           "a color wheel",                               "never used"},
  {"pixel",           "a slightly used pixel",                       "never used"},
  {"maze",            "a map of somewhere else",                     "never used"},
  {"based-intro",     "a spare bulb (green)",                        "never used"},
  {"encrypt",         "Dr. Pepper's code",                           "never used"},
  {"rematch",         "a master key",                                "used once, then broke"},
  {"binary-addition", "one dead battery",                            "never used"},
  {"logicgate",       "a Tower of Wisdom, flat-pack",                "some assembly required"},
  {"bst",             "a map of the maze you had already finished",  "never used"},
  {"fin",             "one (1) chicken",                             ""},
};

} // namespace

std::string inventory_html(const std::string &name)
{
  size_t last = 0;
  while (last < ITEMS.size() && name != ITEMS[last].puzzle) {
    last++;
  }
  if (last == ITEMS.size()) {
    throw std::runtime_error(name + " awards no item: it is not in ITEMS in inventory.cpp");
  }

  // The page's own puzzle has not been solved yet, so its item is not in hand: the first page
  // shows no inventory at all, and every later one shows what the puzzles before it awarded.
  if (last == 0) {
    return "";
  }

  // The last puzzle's page is the payoff, so it is the only one that says how each item was used.
  const bool final_inventory = last + 1 == ITEMS.size();
  std::string html = final_inventory ? "<p><b>Final inventory</b></p>\n" : "<p><b>Your inventory</b></p>\n";
  html += "<ul>\n";
  for (size_t i = 0; i < last; i++) {
    html += "<li>";
    html += ITEMS[i].item;
    if (final_inventory && *ITEMS[i].fate) {
      html += " &mdash; ";
      html += ITEMS[i].fate;
    }
    html += "</li>\n";
  }
  return html + "</ul>\n";
}
