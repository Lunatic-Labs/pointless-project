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
  std::set<std::string> tokens;
  int empty = 0;
  for (size_t i = 0; i < PLAYERS; i++) {
    // Stands in for the random seed the website gives each player at registration.
    const seed_t seed = utils_derive_seed(i, "player");
    std::string game;
    for (const Puzzle &puzzle : game_create_puzzles(seed)) {
      const std::string name = game_puzzle_name(puzzle);
      if (name != "fin" && (puzzle.password.empty() || puzzle.password == "0") && ++empty <= 5) {
        test_check(false, name + " has an answer for seed " + std::to_string(seed) +
                   " (got \"" + puzzle.password + "\")", __FILE__, __LINE__);
      }
      answers[name].insert(puzzle.password);
      tokens.insert(puzzle.token);
      game += puzzle.password + "|";
    }
    games.insert(game);
  }

  test_check(games.size() == PLAYERS, "every player gets a different game", __FILE__, __LINE__);
  // One token per layer, all different: a repeat would credit a player for a puzzle they didn't reach.
  test_check(tokens.size() == PLAYERS * 11, "no token is repeated, within a game or between players (got " +
             std::to_string(tokens.size()) + ")", __FILE__, __LINE__);
  for (const auto &[name, min] : min_different) {
    const size_t different = answers[name].size();
    test_check(different >= min, name + " has " + std::to_string(different) + " different answers among " +
               std::to_string(PLAYERS) + " players (expected at least " + std::to_string(min) + ")", __FILE__, __LINE__);
  }
}
