#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "../include/rng.h"

int get_num(int min, int max, int seed)
{
    srand(seed);

    assert(min != max);
    if(min > max)
    {
        return get_num(max, min, seed);
    }

    return rand() % (max - min) + min;
}

