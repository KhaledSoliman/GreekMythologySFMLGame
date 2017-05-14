#ifndef CS110_CAT_MOUSE_GL_H
#define CS110_CAT_MOUSE_GL_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Game.h"

namespace GL {
    struct Point {
        int x, y;

        Point() : x(0), y(0) {}

        Point(int i, int j) : x(abs(i)), y(abs(j)) {};

        double distance(const Point& RHS){
            return sqrt(pow(RHS.x-x,2)+pow(RHS.y-y,2));
        }

        bool operator==(const Point &RHS) {
            return (RHS.x == x && RHS.y == y);
        };

        Point &operator=(const Point &RHS) {
            x = RHS.x;
            y = RHS.y;
            return *this;
        }
    };

    enum class EntityType {
        Mouse,
        Cat
    };
    enum class TileType {
        water,
        land,
        bridge
    };
    enum class Direction {
        North,
        East,
        West,
        South
    };

    class Entity : public sf::CircleShape {
    public:
        Entity(EntityType entityType);

        EntityType getType() const;

        void draw(sf::RenderWindow &);

        void incrementScore();

        unsigned int getScore();

        void resetScore();

        void updatePosition(Point);

        Point generateRandomPosition() const;

        Point getWorldPosition() const;

        void resetWorldPosition();

    private:
        Point coords;
        unsigned int score;
        EntityType type;
    };


    class Tile : public sf::RectangleShape {
    public:
        Tile() : occupant(NULL) {}

        void setTile(const sf::Vector2f &, const sf::Vector2f &, TileType, Point);

        Entity *getEntity();

        void setEntity(Entity *);

        TileType getType() const;

        Point getWorldPosition() const;

        void draw(sf::RenderWindow &);

    private:
        Entity *occupant;
        TileType type;
        Point coords;
    };

    class World {
    public:
        World() : active(false) {}

        World(const unsigned int, const unsigned int, const sf::Vector2f &, const sf::Vector2f &);

        void populateWorld();

        void createEntities();

        Direction getNextCatPosition() const;

        void moveEntity(EntityType, Direction);

        void examineLocal(Tile, EntityType);

        void setActive(bool);

        bool isActive() const;

        void endGame(const std::string &);

        void draw(sf::RenderWindow &) const;

        unsigned int getHeight() const;

        unsigned int getWidth() const;

        ~World() {
            for (int j = 0; j < height; j++) {
                delete[] content[j];
            }
        }

    private:
        unsigned int height;
        unsigned int width;
        sf::Vector2f startingPosition;
        sf::Vector2f tileSize;
        bool active;
        Tile **content;
    };

    void Render(sf::RenderWindow &);

    void Init();

    void Destroy();

    extern World world;
    extern std::map<std::string, const sf::Texture *> textures;
    extern std::map<EntityType, Entity *> entities;
};


#endif //CS110_CAT_MOUSE_GL_H
