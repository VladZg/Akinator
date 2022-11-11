#include <stdlib.h>

int main()
{
    system("g++ main.cpp ./Stack/Stack.cpp ./Stack/Log.cpp ./Tree.cpp ./Game.cpp ./Database.cpp -o main");
    system("./main");

    return 1;
}
