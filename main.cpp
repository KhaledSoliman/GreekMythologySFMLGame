#include <iostream>
#include "lib/Game.h"


int main() {
    GL::Init();
    GUI::Init();
    Game::game.run();
    return EXIT_SUCCESS;
}