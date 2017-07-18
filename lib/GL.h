#ifndef FinalProject110_GL_H
#define FinalProject110_GL_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Game.h"
#include <time.h>

namespace GL {

    enum {
        male,
        female
    };

    enum class PlayerType {
        god,
        king
    };

    enum class StructureState {
        standing,
        shaky,
        destroyed
    };

    enum class StructureType {
        palace,
        marketplace,
        tavern,
        smallHouse,
        mediumHouse,
        largeHouse,
        farm,
        wall,
        gate,
    };

    class Skill {
    public:
        



    private:
        std::string name;
        int destructiveness;



    };

    class Player {
    public:
    private:
        std::string name;
        PlayerType type;
        std::vector<Skill *> skills;
    };

    class Structure : public sf::Sprite {
    public:
        Structure() {}

    private:
        int resistance;
        sf::Texture *texture;
        StructureType type;
    };

    class Citizen : public sf::Sprite {
    public:

        Citizen() : active(false), alive(true) {
            name = "Doe";
            std::srand(time(NULL));
            age = std::rand() % 99 + 1;
            sex = std::rand() % 2;
            fortified = std::rand() % 2;
        }

        void kill() {
            alive = false;
            active = false;

        }

        bool isAlive() {
            return alive;
        }

        void setActive(bool active) {
            this->active = active;
        }

        bool isActive() {
            return active;
        }

    private:
        int age;
        bool alive;
        bool active;
        bool fortified;
        std::string name;
        int sex;
        sf::Texture *texture;
    };

    class City {
    public:
        City () {
            Citizen * temp;
            for (int i=0; i<2000; i++){
                temp = new Citizen;
                population.push_back(temp);
            }
        }
        ~City(){
            for (auto element : population){
                delete element;

            }
            population.clear();
        }
       void setActive(bool);
       void setTime ();
       void setInitTime(std::time_t);
       bool isActive();

    private:
        std::vector <Citizen*> population;
        sf::Time time;
        const sf::Time incTime = sf::seconds(1800);
        bool active;
        std::time_t initTime;

    };

    class Environment {
    public:
        Environment(const sf::Vector2f &size);

        void setActive(bool);

        bool isActive() const;

        void draw(sf::RenderWindow&);

        ~Environment();

    private:
        struct Tile {
            sf::RectangleShape *shape;
        };
        Tile** grid;
        bool active;
    };

    class Map {

    };

    class Overlay {
    public:

    private:
        std::vector<GUI::Button *> buttons;
        Map* map;
    };


    void Render(sf::RenderWindow &);

    void Update();

    void Init();

    void Destroy();

    extern std::map<std::string, const sf::Texture *> textures;
    extern Environment* environment;
    extern City* city;
};


#endif //FinalProject110_GL_H
