#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "./puzzle.h"
#include "./utils.h"

// Creates every puzzle in the game for `seed`, in play order.
std::vector<Puzzle> game_create_puzzles(seed_t seed);

// Returns a puzzle's name, for example "math" for contents_fp "../resources/files-math".
std::string game_puzzle_name(const Puzzle &puzzle);

// Prints each puzzle's name, password, token, and extra info, for a person to read.
void game_print_passwords(const std::vector<Puzzle> &puzzles);

// Prints the same facts as one line of JSON, for the website to store as a player's answer key:
// {"seed":"...","puzzles":[{"n":1,"name":"math","password":"99","token":"...","extra":null},...]}.
// The website must never recompute these: a stored game keeps the tokens it was generated with.
void game_print_json(seed_t seed, const std::vector<Puzzle> &puzzles);

// Writes `zipdir`/puzzle1.zip through puzzleN.zip. puzzleN.zip holds puzzle N's files and
// puzzle{N+1}.zip, all encrypted with puzzle N-1's password (puzzle1.zip is not encrypted), so
// puzzle1.zip is the whole game.
// Throws std::runtime_error on failure.
void game_write_zipfiles(const std::vector<Puzzle> &puzzles, const std::string &zipdir);

#endif // GAME_H
