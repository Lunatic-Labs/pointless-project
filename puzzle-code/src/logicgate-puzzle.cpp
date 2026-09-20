/*
 * File: logicgate-puzzle.cpp
 * Date: 10/2/2024
 * Description:
 *   Graph Paper Robot II. The user is shown a row of logic gates (colored squares) and a queue of
 *   bits (red and green circles). Each gate in turn takes the first two bits off the queue and
 *   puts its result at the end. The password is the gates' results, in order.
 */

#include <string>
#include <vector>

#include "./include/graphics.h"
#include "./include/puzzle.h"
#include "./include/utils.h"

#define MEMORY "0011011000101110" // The starting queue

// Colors. The gate colors must match files-logicgate/.desc.txt.
static constexpr Pixel OFF{255, 0, 0};   // Red circle: 0
static constexpr Pixel ON{0, 255, 0};    // Green circle: 1
static constexpr Pixel AND{200, 205, 0}; // Gold square
static constexpr Pixel OR{210, 0, 200};  // Purple square
static constexpr Pixel XOR{0, 100, 200}; // Blue square

enum class Gate { And, Or, Xor, Count };

static bool apply(Gate gate, bool a, bool b)
{
  switch (gate) {
    case Gate::And: return a && b;
    case Gate::Or:  return a || b;
    default:        return a != b;
  }
}

static Pixel color(Gate gate)
{
  switch (gate) {
    case Gate::And: return AND;
    case Gate::Or:  return OR;
    default:        return XOR;
  }
}

// Returns the gates as a row of squares.
static Svg gates_svg(const std::vector<Gate> &gates)
{
  Image image(gates.size(), 1);
  for (size_t i = 0; i < gates.size(); i++) {
    image(0, i) = color(gates[i]);
  }
  return graphics_gen_svg_from_image(image, 48, "#000000");
}

// Returns the queue as a row of circles, with the first bit on the right.
static Svg memory_svg(const std::vector<bool> &memory)
{
  Svg svg(memory.size() * 40, 40);
  for (size_t i = 0; i < memory.size(); i++) {
    Svg::Circle circle((memory.size() - i - 0.5f) * 40, 20, 20, graphics_pixel_to_hex(memory[i] ? ON : OFF), "#000000");
    svg.add_shape(circle);
  }
  return svg;
}

Puzzle logicgate_puzzle_create(seed_t seed)
{
  std::vector<bool> memory;
  for (char c : std::string(MEMORY)) {
    memory.push_back(c == '1');
  }

  std::vector<Gate> gates;
  std::string password;
  std::vector<bool> queue = memory;
  while (queue.size() > 1) {
    Gate gate = (Gate)utils_rng_roll(0, (int)Gate::Count - 1, seed);
    bool result = apply(gate, queue[0], queue[1]);
    queue.erase(queue.begin(), queue.begin() + 2);
    queue.push_back(result);
    gates.push_back(gate);
    password += result ? '1' : '0';
  }

  const std::string token = utils_token(utils_derive_seed(seed, "token"));
  std::string html_content = utils_html_printf("Graph Paper Robot PT II", "../resources/files-logicgate/.desc.txt",
                                               {gates_svg(gates).build(false), memory_svg(memory).build(false)}, token);
  utils_generate_file("../resources/files-logicgate/instructions.html", html_content);
  return {"../resources/files-logicgate", html_content, password, token, {}};
}
