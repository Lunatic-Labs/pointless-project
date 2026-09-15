#include <map>
#include <set>

#include "./include/test.h"
#include "../src/include/game.h"

// Checks, over many players, that every puzzle has an answer and that players get different puzzles.
void seeds_test()
{
  const size_t PLAYERS = 200;

  // The fewest different answers each puzzle should have among PLAYERS players.
  const std::map<std::string, size_t> min_different = {
    {"math", 40},
    {"color", 2},
    {"pixel", 10},
    {"maze", 150},
    {"based-intro", 190},
    {"encrypt", 13},
    {"rematch", 190},
    {"binary-addition", 40},
    {"logicgate", 150},
    {"bst", 190},
  };

  std::map<std::string, std::set<std::string>> answers;
  std::set<std::string> games;
  int empty = 0;
  for (size_t i = 0; i < PLAYERS; i++) {
    const std::string email = "player" + std::to_string(i) + "@example.com";
    std::string game;
    for (const Puzzle &puzzle : game_create_puzzles(utils_seed_from_email(email))) {
      const std::string name = game_puzzle_name(puzzle);
      if (name != "fin" && (puzzle.password.empty() || puzzle.password == "0") && ++empty <= 5) {
        test_check(false, name + " has an answer for " + email + " (got \"" + puzzle.password + "\")", __FILE__, __LINE__);
      }
      answers[name].insert(puzzle.password);
      game += puzzle.password + "|";
    }
    games.insert(game);
  }

  test_check(games.size() == PLAYERS, "every player gets a different game", __FILE__, __LINE__);
  for (const auto &[name, min] : min_different) {
    const size_t different = answers[name].size();
    test_check(different >= min, name + " has " + std::to_string(different) + " different answers among " +
               std::to_string(PLAYERS) + " players (expected at least " + std::to_string(min) + ")", __FILE__, __LINE__);
  }
}
