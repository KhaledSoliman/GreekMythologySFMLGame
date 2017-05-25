#ifndef FinalProject110_GL_H
#define FinalProject110_GL_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <set>
#include "Game.h"
#include "GUI.h"
#include <time.h>

namespace GL {

    enum {
        male,
        female
    };

    enum ButtonFlags {
        disabled = 1 << 0,
        enabled = 1 << 1,
        hovered = 1 << 2,
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
        Skill(const std::string &, const sf::Vector2f &);

        bool isActive() const;

        void setActive(bool);

        void draw(sf::RenderWindow &);

        void trigger();

        bool isMouseOver(const sf::Vector2f &) const;

        void hover();

        int getState() const;

        void unHover();

        ~Skill();

    private:
        sf::RectangleShape *background;
        sf::RectangleShape *content;
        std::string name;
        bool active;
        const sf::Color hoverColor = sf::Color(0xFFFFFFFF);
        const sf::Color clickColor = sf::Color(0xFFFF00FF);
        const sf::Color lockedColor = sf::Color(0xFF0000FF);
        int state;
        int FaithCost;

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
    };

    class Citizen : public sf::Sprite {
    public:

        Citizen() : active(false), alive(true) {
            name = "Doe";
            std::srand(time(NULL));
            age = std::rand() % 99 + 1;
            sex = std::rand() % 2;
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
        std::string name;
        int sex;
        sf::Texture *texture;
    };

    class City {
    public:
    private:
    };

    class Environment {
    public:
        Environment(const sf::Vector2f &size);

        void setActive(bool);

        bool isActive() const;

        void draw(sf::RenderWindow &);

        ~Environment();

    private:
        struct Tile {
            sf::RectangleShape *shape;
        };
        Tile **grid;
        bool active;
    };


    class Overlay {
    public:
        Overlay() : active(false) {}

        void setActive(bool);

        bool isActive() const;

        void setText(const std::string &, unsigned int, const sf::Uint32 &);

        void addText(const std::string &, const sf::Vector2f &);

        void addSkill(Skill*);

        bool clickScan(const sf::Vector2f &);

        void hoveringScan(const sf::Vector2f &);

        void draw(sf::RenderWindow &) const;

        void emptyText();

        ~Overlay() {
            emptyText();
            for (auto element: skills)
                delete element;
            for (auto element: buttons)
                delete element;
            skills.clear();
            buttons.clear();
        }

    private:
        std::set<sf::Text *> texts;
        std::set<GUI::Button *> buttons;
        std::vector<Skill *> skills;
        sf::Font font;
        sf::Color defaultColor;
        bool active;
        unsigned int charSize;
    };


    void Render(sf::RenderWindow &);

    void Init();

    void Destroy();

    extern std::map<std::string, const sf::Texture *> textures;
    extern Environment *environment;
    extern Overlay* overlay;
};


#endif //FinalProject110_GL_H
