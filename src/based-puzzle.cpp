#include "./include/puzzle.h"
#include "./include/utils.h"


//TODO: Get the puzzle solution


Puzzle based_puzzle_create(long seed)
{
    
    std::vector<std::string> values;

    for (int i = 0; i < 12; i++) {
        short rand = utils_rng_roll(0, 67, seed);
        if (rand > 42) {
            values.push_back(std::string(1, static_cast<char>(rand + 23)));
        } else {
            values.push_back(std::to_string(rand));
        }
    }
    std::string seed_val = "seed: " + std::to_string(seed);
    values.insert(values.end(), seed_val);

    std::string html_content = utils_html_printf("Base Puzzle", "./files-based/.desc.txt", values);
    utils_generate_file("./files-based/instructions.html", html_content);
    
    return Puzzle{"files-based", "test_pass"};
}

