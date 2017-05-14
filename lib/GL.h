#ifndef CS110_CAT_MOUSE_GL_H
#define CS110_CAT_MOUSE_GL_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Game.h"

namespace GL {


    void Render(sf::RenderWindow &);

    void Init();

    void Destroy();

    extern std::map<std::string, const sf::Texture *> textures;
};


#endif //CS110_CAT_MOUSE_GL_H
