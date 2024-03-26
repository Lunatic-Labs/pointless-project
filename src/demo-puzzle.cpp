#include "./include/puzzle.h"
#include "./include/utils.h"

int fib(int n) {
    if(n < 2) {
        return n;
    }
    return fib(n-1) + fib(n-2);
}

Puzzle demo_puzzle_create(long seed) {

    int n =utils_rng_roll(3,8,seed);
    int fib_number = fib(n);

    std::string html = utils_html_printf("fib","files-demo/.desc.txt", {std::to_string(n)});
    utils_generate_file("files-demo/instrucuins.html", html);
    return Puzzle{"files-demo", std::to_string(fib_number), {}};
}