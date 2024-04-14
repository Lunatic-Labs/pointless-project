#include <algorithm>
#include <iostream>
#include <cmath>
#include "./include/puzzle.h"
#include "./include/graphics.h"
#include "./include/utils.h"

#define BLANK {255, 255, 255}// black
#define OFF {255, 0, 0}      // red
#define ON {0, 255, 0}       // green
#define AND {200, 205, 0}    // gold
#define XOR {0, 100, 200}    // bluish
#define NAND {200, 100, 0}   // orange
#define NOR {180, 230, 180}  // odd green
#define OR {210, 0, 200}     // purple

enum class Gate {
  And,
  Or,
  Xor,
  Nand,
  Nor,
  NumberOfGates, // UNUSED -- only get the number of items
};

static Image generate_image(std::vector<bool> orig_binary, std::vector<Gate> &gates)
{
  int height = std::log2(orig_binary.size());
  Image image(orig_binary.size()/2, height);

  int max_row = orig_binary.size()/2;
  int row = 0;
  int column = 0;
  for (size_t i = 0; i < gates.size(); i++) {
    Pixel pixel{0,0,0};
    switch (gates[i]) {
      case Gate::And:
        pixel = AND;
        break;
      case Gate::Or:
        pixel = OR;
        break;
      case Gate::Xor:
        pixel = XOR;
        break;
      case Gate::Nand:
        pixel = NAND;
        break;
      case Gate::Nor:
        pixel = NOR;
        break;
      default:
        std::cerr << "unimplemented gate: " << (int)gates[i] << std::endl;
        exit(1);
    }

    image(height-column-1, row) = pixel;
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
    bool bit1 = memory.at(0);
    bool bit2 = memory.at(1);
    memory.erase(memory.begin());
    memory.erase(memory.begin());

    Gate gate = (Gate)utils_rng_roll(0, (int)Gate::NumberOfGates-1, seed);
    gates.push_back(gate);
    if(gate == Gate::And) {
      memory.push_back(bit1&bit2);
      answers.push_back(bit1&bit2);
    }
    else if(gate == Gate::Xor) {
      memory.push_back(bit1^bit2);
      answers.push_back(bit1^bit2);
    }
    else if(gate == Gate::Nand) {
      memory.push_back(!(bit1&bit2));
      answers.push_back(!(bit1&bit2));
    }
    else if(gate == Gate::Nor) {
      memory.push_back(!(bit1|bit2));
      answers.push_back(!(bit1|bit2));
    }
    else {
      memory.push_back(bit1|bit2);
      answers.push_back(bit1|bit2);
    }
  }
}

Svg generate_memory_image(std::vector<bool> &orig_binary)
{
  Svg svg(orig_binary.size()*40, 40);
  for (size_t i = 0; i < orig_binary.size(); i++) {
    std::string color = "";
    if (orig_binary[i]) {
      color = graphics_pixel_to_hex(Pixel ON);
    }
    else {
      color = graphics_pixel_to_hex(Pixel OFF);
    }
    // Svg::Circle circle(20.f, (float)(orig_binary.size()-i-0.5f)*40, 20.f, color, {}, {});
    Svg::Circle circle((float)(orig_binary.size()-i-0.5f)*40, 20.f, 20.f, color, {"#000000"}, {});
    svg.add_shape(circle);
  }

  return svg;
}

Puzzle logicgate_puzzle_create(long seed)
{
  uint32_t old_flags = FLAGS;
  FLAGS &= ANS_ONLY;
  // std::string binary = binary_addition_puzzle_create(seed).password;
  std::string binary = "0011011000101110";
  FLAGS = old_flags;
  std::vector<bool> memory;
  std::vector<bool> answers;
  std::vector<Gate> gates;
  for (char c : binary) memory.push_back(c == '1');

  generate_logicgate(memory, answers, gates, seed);

  Image gates_img = generate_image(memory, gates);
  Svg svg = graphics_gen_svg_from_image(gates_img, 60, "#000000");
  Svg mem_svg = generate_memory_image(memory);

  std::string password = "";
  std::for_each(answers.begin(), answers.end(), [&](bool b) { password += std::to_string(b); });

  std::string html_content = utils_html_printf("Graph Paper Robot PT II", "./files-logicgate/.desc.txt", {svg.build(), mem_svg.build()});
  utils_generate_file("./files-logicgate/instructions.html", html_content);

  return {"files-logicgate", password, {}};
}
