/*
 * File: encrypt-puzzle.cpp
 * Author: Zachary Haskins, Turner Austin
 * Contact: zdhdev@yahoo.com, tcaustin@mail.lipscomb.edu
 * Date: 2/14/2024
 * Description:
 *   The user is presented with a word that has been encrypted using a series of operations.
 *   We provide them with the steps that were taken to encrypt the word and they must decrypt it
 *   by reversing the operations. The password is the original word.
 */

 #include <algorithm>
 #include <iostream>
 #include <cassert>
 
 #include "./include/puzzle.h"
 #include "./include/utils.h"
 
 #define ENCR_WORDS {       \
   "lipscomb_university",   \
   "computer_science",      \
   "programming_languages", \
   "data_structures",       \
   "dijkstras_algorithm",   \
   "depth_first_search",    \
   "breadth_first_search",  \
   "graph_theory",          \
   "dynamic_programming",   \
   "greedy_algorithms",     \
   "big_o_notation",        \
   "recursive_function",    \
   "binary_search_tree",    \
 }
 
 #define ENCR_OPS_SHIFT 0
 #define ENCR_OPS_SWAP 1
 #define ENCR_OPS_CHANGE 2
 
 #define ENCR_OPS {  \
   ENCR_OPS_CHANGE,  \
   ENCR_OPS_SWAP,    \
   ENCR_OPS_SWAP,    \
   ENCR_OPS_SWAP,    \
   ENCR_OPS_SHIFT,   \
 }
 
 #define ENCR_ROTATIONS_MIN 1 // Must be 1 or greater
 #define ENCR_ROTATIONS_MAX 3
 
 #define ENCR_CHANGE_MIN 1 // Must be 1 or greater
 #define ENCR_CHANGE_MAX 3
 
 static void shift(std::string &s, int dir, int n)
 {
   if (dir) {
     std::rotate(s.rbegin(), s.rbegin() + n, s.rend());
   } else {
     std::rotate(s.begin(), s.begin() + n, s.end());
   }
 }
 
 static void swap(std::string &s, int a, int b)
 {
   std::swap(s[a], s[b]);
 }
 
 static void change(std::string &s, int n)
 {
   assert(n > 0 && n < 27 && "Invalid change");
   for (size_t i = 0, c = 0; i < s.size(); ++i) {
     if (isalpha(s[i])) {
       ++c;
     }
     if (c == 4) {
       s[i] -= 'a';
       s[i] = (s[i] + n) % 26;
       s[i] += 'a';
       c = 0;
     }
   }
 }
 
 static std::string num_to_english(int idx) 
 {
   std::string numstr = std::to_string(idx);
   if (idx%10 == 1) {
     return numstr + "st";
   }
   if (idx%10 == 2) {
     return numstr  + "nd";
   }
   if (idx%10 == 3) {
     return numstr + "rd";
   }
   return numstr + "th";
 }
 
 Puzzle encrypt_puzzle_create(long seed)
 {
   std::string words[] = ENCR_WORDS;
   const int word_idx = utils_rng_roll(0, sizeof(words)/sizeof(*words)-1, seed);
 
   std::string word = words[word_idx];
   size_t len = word.size()-1;
 
   std::vector<int> ops = ENCR_OPS;
   std::string encrypt_steps = "";
 
   for (int i = 0; ops.size(); ++i) {
     int rng = utils_rng_roll(0, ops.size()-1, seed);
     encrypt_steps += "<li>";
     switch (ops[rng]) {
       case 0: {
         int dir = utils_rng_roll(0, 1, seed);
         int rotations = utils_rng_roll(ENCR_ROTATIONS_MIN, ENCR_ROTATIONS_MAX, seed);
         encrypt_steps += std::to_string(i+1) + ".) Shifted all characters by " + std::to_string(rotations) + " to the " + (dir ? "right" : "left") + ".\n";
         shift(word, dir, rotations);
       } break;
       case 1: {
         int idx1 = utils_rng_roll(0, len, seed);
         int idx2 = utils_rng_roll(0, len, seed);
         if (idx1 == idx2) { // In the off chance that they are the same.
           idx2 = (idx2+1) % len;
         }
         encrypt_steps += std::to_string(i+1) + ".) Swapped the " + num_to_english(idx1+1) + " and " + num_to_english(idx2+1) + " characters.\n";
         swap(word, idx1, idx2);
       } break;
       case 2: {
         int jump_dist = utils_rng_roll(ENCR_CHANGE_MIN, ENCR_CHANGE_MAX, seed);
         encrypt_steps += std::to_string(i+1) + ".) Alphabetically increased every 4th letter by " + std::to_string(jump_dist) + ", skiping the _ character.\n";
         change(word, jump_dist);
       } break;
       default:
         assert(false && "Invalid operation");
     }
     encrypt_steps += "</li>\n";
     ops.erase(ops.begin() + rng);
   }
   std::vector<std::string> instructions { word, encrypt_steps };
 
   std::string html_body = utils_html_printf("Encrypt", "./files-encrypt/.desc.txt", instructions);
   utils_generate_file("./files-encrypt/instructions.html", html_body);
 
   return {"files-encrypt", words[word_idx], {}};
 }