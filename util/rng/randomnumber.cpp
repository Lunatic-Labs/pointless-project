#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

int getNum(int min, int max){

    assert(min != max);
    if(min > max) {
        return getNum(max, min);
    }
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    
    return rand() % (max - min) + min;
}