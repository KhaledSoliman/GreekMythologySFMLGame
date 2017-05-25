#include "GL.h"

std::map<std::string, const sf::Texture *> GL::textures;
GL::Environment *GL::environment;

void GL::Render(sf::RenderWindow &window) {
    environment->draw(window);
}

void GL::Init() {
    sf::Texture *pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/water_texture.jpg");
    textures["water"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/grass_texture.jpg");
    textures["grass"] = pTexture;
    environment = new Environment(Game::game.getWindow().window.getView().getSize());
}

void GL::Destroy() {
    for (auto element: textures)
        delete element.second;
    delete environment;
    textures.clear();
}

GL::Environment::Environment(const sf::Vector2f &size) {
    active = false;
    grid = new Tile *[8];
    for (int i = 0; i < 8; i++)
        grid[i] = new Tile[10];
    float tileWidth = 1930.f / 10.f;
    float tileHeight = 1180.f / 10.f;
    sf::Vector2f currentPosition = sf::Vector2f(0.f, 0.f);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j].shape = new sf::RectangleShape;
            grid[i][j].shape->setSize(sf::Vector2f(tileWidth, tileHeight));
            grid[i][j].shape->setPosition(currentPosition);
            grid[i][j].shape->setTexture(textures["grass"]);
            grid[i][j].shape->setOutlineColor(sf::Color(0x203000FF));
            grid[i][j].shape->setOutlineThickness(1.f);
            currentPosition.x += tileWidth + 2.f;
        }
        currentPosition.x = 0.f;
        currentPosition.y += tileHeight + 2.f;
    }

}

bool GL::Environment::isActive() const {
    return active;
}

void GL::Environment::setActive(bool active) {
    this->active = active;
}

void GL::Environment::draw(sf::RenderWindow &window) {
    if (active) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 10; j++) {
                window.draw(*grid[i][j].shape);
            }
        }
    }
}

GL::Environment::~Environment() {
    for (int i = 0; i < 8; i++)
        delete[] grid[i];
}
