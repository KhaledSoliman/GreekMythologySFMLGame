#include <iostream>
#include "GL.h"
#include "GUI.h"

std::map<std::string, const sf::Texture *> GL::textures;
/*
 * Functions Native to the Namespace.
 */

void GL::Render(sf::RenderWindow &window) {
}

void GL::Init() {
    sf::Texture *pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/mouse_texture.png");
    textures["mouse"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/water_texture.jpg");
    textures["water"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/grass_texture.png");
    textures["grass"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/cat_texture.png");
    textures["cat"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/bridge_texture.jpg");
    textures["bridge"] = pTexture;
}

void GL::Destroy() {
    for (auto element: textures)
        delete element.second;
    textures.clear();
}