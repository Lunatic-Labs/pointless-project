#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

int getNum(int min, int max, int seed=-1){

    assert(min != max);
    if(min > max) {
        return getNum(max, min, seed);
    }
    srand(seed);
    
    return rand() % (max - min) + min;
}