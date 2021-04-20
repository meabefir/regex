#include <iostream>

#include "Game.h"
#include <ctime>

int main()
{
    srand(time(0));

    Game g;
    g.run();

    return 0;
}
