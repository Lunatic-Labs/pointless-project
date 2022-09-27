#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int getNum(int min, int max){
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    return rand() % max + min;
}