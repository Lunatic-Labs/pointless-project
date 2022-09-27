#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int getNum(){
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    return rand() % 10000000;
}
int main()
{
    int num;
    for(int i = 0; i < 8; ++i){
        num = getNum();
        cout << num << " ";
    }
}