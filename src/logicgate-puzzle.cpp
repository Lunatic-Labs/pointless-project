#include <algorithm>
#include <iostream>
#include <cmath>
#include "./include/puzzle.h"
#include "./include/graphics.h"
#include "./include/utils.h"

#define BLANK {255, 255, 255} // black
#define OFF {255, 0, 0}       // red
#define ON {0, 255, 0}        // green
#define AND {255, 255, 0}   // gold
#define OR {255, 0, 255}     // purple

enum class Gate {
  And,
  Or,
};

static Image generate_image(std::vector<bool> orig_binary, std::vector<Gate> &gates)
{
  (void)gates;

  int height = std::log2(orig_binary.size())+1;
  Image image(orig_binary.size(), height);

  for (size_t i = 0; i < orig_binary.size(); i++) {
    if (orig_binary[i]) {
      image(height-1, i) = ON;
    }
    else {
      image(height-1, i) = OFF;
    }
  }

  int max_row = height;
  int row = 0;
  int column = 0;
  for (size_t i = 0; i < gates.size(); i++) {
    Pixel pixel = {0,0,0};
    switch (gates[i]) {
      case Gate::And:
        pixel = AND;
        break;
      case Gate::Or:
        pixel = OR;
        break;
      default:
        std::cerr << "unimplemented gate: " << (int)gates[i] << std::endl;
        exit(1);
    }

    image(height-column-2, row) = pixel;
    row++;
    if (row >= max_row) {
      max_row /= 2;
      row = 0;
      column += 1;
    }
  }

  return image;
}

static void generate_logicgate(std::vector<bool> memory, std::vector<bool> &answers, std::vector<Gate> &gates, long seed)
{
  while (memory.size() > 1) {
    for (auto m : memory) std::cout << m << ' ';
    std::cout << std::endl;
    bool bit1 = memory.at(0);
    bool bit2 = memory.at(1);
    memory.erase(memory.begin());
    memory.erase(memory.begin());

    Gate gate = (Gate)utils_rng_roll(0, 1, seed);
    gates.push_back(gate);
    if(gate == Gate::And) {
      memory.push_back(bit1&bit2);
      answers.push_back(bit1&bit2);
    }
    else {
      memory.push_back(bit1|bit2);
      answers.push_back(bit1|bit2);
    }
  }
}

Puzzle logicgate_puzzle_create(long seed)
{
  uint32_t old_flags = FLAGS;
  FLAGS &= ANS_ONLY;
  std::string binary = binary_addition_puzzle_create(seed).password;
  FLAGS = old_flags;
  std::vector<bool> memory;
  std::vector<bool> answers;
  std::vector<Gate> gates;
  for (char c : binary) memory.push_back(c == '1');

  generate_logicgate(memory, answers, gates, seed);
  Image img = generate_image(memory, gates);
  std::string svg = graphics_gen_svg(img, 40.f);

  std::string password = "";
  std::for_each(answers.begin(), answers.end(), [&](bool b) { password += std::to_string(b); });

  std::string html_content = utils_html_printf("Graph Paper Robot PT II", "./files-logicgate/.desc.txt", {svg});
  utils_generate_file("./files-logicgate/instructions.html", html_content);

  return {"files-logicgate", password, {}};
}
