#include <iostream>
#include "lib/Game.h"


int main() {
    GUI::Init();
    GL::Init();
    Game::game.run();
    return EXIT_SUCCESS;
}