#include "../src/include/inventory.h"
#include "./include/test.h"

// Returns the number of times `needle` occurs in `haystack`.
static int count(const std::string &haystack, const std::string &needle)
{
  int found = 0;
  for (size_t pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + needle.size())) {
    found++;
  }
  return found;
}

void inventory_test()
{
  // An item is awarded for solving its puzzle, so the first page has nothing in hand yet and each
  // later page lists what the puzzles before it awarded, keeping every earlier one.
  CHECK_EQ(inventory_html("math"), "");

  const std::string maze = inventory_html("maze");
  CHECK_EQ(std::to_string(count(maze, "<li>")), "3");
  CHECK(maze.find("an abacus") != std::string::npos);
  // Items the player has not been given yet are not shown, the maze's own included.
  CHECK(maze.find("a map of somewhere else") == std::string::npos);
  CHECK(maze.find("chicken") == std::string::npos);

  // Only the last page is the payoff, so only it says what each item turned out to be good for.
  CHECK(maze.find("never used") == std::string::npos);
  const std::string fin = inventory_html("fin");
  CHECK_EQ(std::to_string(count(fin, "<li>")), "10");
  CHECK(fin.find("Final inventory") != std::string::npos);
  CHECK(fin.find("a master key &mdash; used once, then broke") != std::string::npos);
  // The chicken is the fin page's own MacGuffin: the page hands it over, so it is not in the list.
  CHECK(fin.find("chicken") == std::string::npos);

  CHECK_THROWS(inventory_html("rematch-maze"));
  CHECK_THROWS(inventory_html(""));
}
