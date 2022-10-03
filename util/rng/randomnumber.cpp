#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

int getNum(int min, int max){
    assert(min != max);
    if(min > max) {
        int hold = max;
        max = min;
        min = hold;
    }
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    return rand() % (max - min) + min;
}