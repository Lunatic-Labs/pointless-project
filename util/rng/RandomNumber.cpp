#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "RandomNumber.h"

int getNum(int min, int max, int seed)
{
    srand(seed);

    assert(min != max);
    if(min > max) 
    {
        return getNum(max, min, seed);
    }
    
    return rand() % (max - min) + min;
}