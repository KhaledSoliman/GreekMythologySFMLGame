#include <iostream>
#include "GL.h"
#include "GUI.h"

GL::World GL::world(12, 12, sf::Vector2f(600.f, 50.f), sf::Vector2f(80.f, 80.f));
std::map<std::string, const sf::Texture *> GL::textures;
std::map<GL::EntityType, GL::Entity *> GL::entities;

/*
 * Functions Native to the Namespace.
 */

void GL::Render(sf::RenderWindow &window) {
    if (world.isActive())
        world.draw(window);
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
    world.createEntities();
}

void GL::Destroy() {
    for (auto element: entities)
        delete element.second;
    for (auto element: textures)
        delete element.second;
    entities.clear();
    textures.clear();
}

/*
 * Tile Class Definition
 */

void GL::Tile::setEntity(GL::Entity *pEntity) {
    if (pEntity != nullptr)
        pEntity->setPosition((this->getPosition().x + (this->getSize().x / 2)) - pEntity->getRadius(),
                             (this->getPosition().y + (this->getSize().y / 2)) - pEntity->getRadius());
    occupant = pEntity;
}

GL::TileType GL::Tile::getType() const {
    return type;
}

GL::Entity *GL::Tile::getEntity() {
    return occupant;
}

GL::Point GL::Tile::getWorldPosition() const {
    return coords;
}

void GL::Tile::setTile(const sf::Vector2f &pos, const sf::Vector2f &size, TileType tileType,
                       Point location) {
    setSize(size);
    setPosition(pos);
    coords = location;
    type = tileType;
}

void GL::Tile::draw(sf::RenderWindow &window) {
    window.draw(*this);
    for(auto element: entities)
        element.second->draw(window);
}

/*
 * Entity Class Definition
 */

GL::Entity::Entity(EntityType entityType) {
    type = entityType;
    coords = generateRandomPosition();
}

GL::EntityType GL::Entity::getType() const {
    return type;
}

void GL::Entity::draw(sf::RenderWindow &window) {
    window.draw(*this);
};

void GL::Entity::incrementScore() {
    score++;
}

unsigned int GL::Entity::getScore() {
    return score;
}

void GL::Entity::resetScore() {
    score = 0;
}

void GL::Entity::updatePosition(Point pos) {
    coords = pos;
}

GL::Point GL::Entity::getWorldPosition() const {
    return coords;
}

GL::Point GL::Entity::generateRandomPosition() const {
    Point location;
    std::vector<Point> occupiedLocations;
    for (auto element: entities) {
        occupiedLocations.push_back(element.second->getWorldPosition());
    }
    do {
        location = Point(rand() % (world.getHeight() - 4) + 2, rand() % (world.getWidth() - 4) + 2);
    } while (std::find(occupiedLocations.begin(), occupiedLocations.end(), location) != occupiedLocations.end());
    return location;
}

void GL::Entity::resetWorldPosition() {
    coords = generateRandomPosition();
}

/*
 * World Class Definition
 */

void GL::World::setActive(bool boolean) {
    active = boolean;
}

bool GL::World::isActive() const {
    return active;
}

void GL::World::populateWorld() {
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            content[j][i].setEntity(nullptr);
    for (auto element: entities) {
        element.second->resetWorldPosition();
        content[element.second->getWorldPosition().y][element.second->getWorldPosition().x].setEntity(element.second);
    }
}

void GL::World::createEntities() {
    Entity *pEntity = new Entity(EntityType::Mouse);
    pEntity->setRadius(35);
    pEntity->setTexture(textures["mouse"]);
    content[pEntity->getWorldPosition().y][pEntity->getWorldPosition().x].setEntity(pEntity);
    entities[EntityType::Mouse] = pEntity;
    pEntity = new Entity(EntityType::Cat);
    pEntity->setRadius(35);
    pEntity->setTexture(textures["cat"]);
    content[pEntity->getWorldPosition().y][pEntity->getWorldPosition().x].setEntity(pEntity);
    entities[EntityType::Cat] = pEntity;
    sf::Vector2f currentPosition = startingPosition;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            currentPosition.x += tileSize.x;
            if ((j == ((height - 1) / 2)) && (i == (width - 1))) {
                content[j][i].setTile(currentPosition, tileSize, TileType::bridge, Point(i, j));
                content[j][i].setTexture(textures["bridge"]);
            } else if (j % (height - 1) == 0 || i % (width - 1) == 0) {
                content[j][i].setTile(currentPosition, tileSize, TileType::water, Point(i, j));
                content[j][i].setFillColor(sf::Color::Blue);
                content[j][i].setTexture(textures["water"]);
            } else {
                content[j][i].setTile(currentPosition, tileSize, TileType::land, Point(i, j));
                content[j][i].setTexture(textures["grass"]);
            }
            content[j][i].setEntity(nullptr);
        }
        currentPosition.x = startingPosition.x;
        currentPosition.y += tileSize.y;
    }
}

void GL::World::moveEntity(EntityType type, Direction direction) {
    Point newLocation;
    Entity *pEntity;
    pEntity = entities[type];
    Point previousLocation = pEntity->getWorldPosition();
    switch (direction) {
        case Direction::North:
            newLocation.y = previousLocation.y - 1;
            newLocation.x = previousLocation.x;
            break;
        case Direction::East:
            newLocation.x = previousLocation.x + 1;
            newLocation.y = previousLocation.y;
            break;
        case Direction::West:
            newLocation.x = previousLocation.x - 1;
            newLocation.y = previousLocation.y;
            break;
        case Direction::South:
            newLocation.y = previousLocation.y + 1;
            newLocation.x = previousLocation.x;
            break;
    }
    pEntity->updatePosition(newLocation);
    content[previousLocation.y][previousLocation.x].setEntity(nullptr);
    examineLocal(content[newLocation.y][newLocation.x], pEntity->getType());
    content[newLocation.y][newLocation.x].setEntity(pEntity);
}

void GL::World::examineLocal(Tile location, EntityType Entity) {
    if (Entity == EntityType::Cat) {
        if (location.getType() == TileType::water) {
            entities[EntityType::Mouse]->incrementScore();
            endGame("The cat has drowned :(");
        } else if (location.getWorldPosition() == entities[EntityType::Mouse]->getWorldPosition()) {
            entities[EntityType::Cat]->incrementScore();
            endGame("The cat has eaten the mouse :(");
        }
    } else {
        if (location.getWorldPosition() == entities[EntityType::Cat]->getWorldPosition()) {
            entities[EntityType::Cat]->incrementScore();
            endGame("The mouse has fed it self to the cat??");
        } else if (location.getType() == TileType::water) {
            entities[EntityType::Cat]->incrementScore();
            endGame("The mouse has jumped in the water lol!");
        } else if (location.getType() == TileType::bridge) {
            entities[EntityType::Mouse]->incrementScore();
            endGame("The mouse has escaped to god knows where!");

        }
    }
}

GL::Direction GL::World::getNextCatPosition() const {}

void GL::World::endGame(const std::string &message) {
    setActive(false);
    GUI::removeTexts();
    GUI::menus[GUI::Menus::EndGame]->addText(message);
    GUI::menus[GUI::Menus::EndGame]->addText("Mouse Score: " + std::to_string(GL::entities[GL::EntityType::Mouse]->getScore()));
    GUI::menus[GUI::Menus::EndGame]->addText("Cat Score: " + std::to_string(GL::entities[GL::EntityType::Cat]->getScore()));
    GUI::menus[GUI::Menus::EndGame]->createBackground();
    GUI::menus[GUI::Menus::EndGame]->setActive(true);
}

GL::World::World(unsigned int newHeight, unsigned int newWidth, const sf::Vector2f &pos, const sf::Vector2f &size)
        : height(newHeight), width(newWidth) {
    content = new Tile *[height];
    for (int i = 0; i < height; i++)
        content[i] = new Tile[width];
    startingPosition = pos;
    tileSize = size;
}

void GL::World::draw(sf::RenderWindow &window) const {
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            content[j][i].draw(window);
}

unsigned int GL::World::getHeight() const {
    return height;
}

unsigned int GL::World::getWidth() const {
    return width;
}