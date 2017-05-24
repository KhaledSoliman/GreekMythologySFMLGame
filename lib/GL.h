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
        tree
    };

    class Skill {
    public:
    private:
        int destructiveness;


    };

    class Player {
    public:
    private:
        std::string name;
        PlayerType type;
        std::vector<Skill*> skills;
    };

    class Structure : public sf::Sprite {
    public:
        Structure()  {}
    private:
        int resistance;
        sf::Texture* texture;
    };

    class Citizen : public sf::Sprite {
    public:

        Citizen () : active(false), alive(true) {
            name = "Doe";
            std::srand(time(NULL));
            age = std::rand() % 99 + 1;
            sex =  std::rand() % 2;
        }
        void kill(){
            alive = false;
            active = false;

        }
        bool isAlive() {
            return alive;
        }
        void setActive(bool active){
            this->active = active;
        }
        bool isActive() {
            return active;
        }

    private:
        int age;
        bool alive;
        bool active;
        std::string name;
        int sex;
        sf::Texture* texture;
    };

    class City {
    public:
    private:
    };

    void Render(sf::RenderWindow &);

    void Init();

    void Destroy();

    extern std::map<std::string, const sf::Texture *> textures;
};


#endif //FinalProject110_GL_H
