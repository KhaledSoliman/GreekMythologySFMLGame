
#ifndef CS110_CAT_MOUSE_GUI_H
#define CS110_CAT_MOUSE_GUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <set>
#include <memory>


namespace GUI {

    enum ButtonFlags {
        disabled = 1 << 0,
        enabled = 1 << 1,
        hovered = 1 << 2,
    };

    struct Sound {
        sf::SoundBuffer buffer;
        sf::Sound sound;

        void setSound(const std::string &path) {
            buffer.loadFromFile(path);
            sound.setBuffer(buffer);
        }
    };

    class Button : public sf::Text {
    public:
        Button();

        Button(const std::string &, const sf::Font &, const sf::Color &, unsigned int);

        void setState(int);

        int getState() const;

        void setProfileButton(unsigned int);

        void bindFunction(std::function<void()>);

        bool isMouseOver(const sf::Vector2f &) const;

        void trigger();

    private:
        bool clickAble;
        bool profileButton;
        unsigned int profileNum;
        int state;
        std::function<void()> callBack;
    };

    class Menu {
    public:
        Menu() : active(false) {};

        void addButton(const std::string &);

        void addText(const std::string &);

        void draw(sf::RenderWindow &) const;

        bool clickScan(const sf::Vector2f &);

        void hoveringScan(const sf::Vector2f &);

        void createBackground();

        void setActive(bool);

        bool isActive() const;

        void setIdentity(const std::string &);

        void setText(const std::string &, unsigned int, const sf::Uint32 &);

        void setHover(const std::string &, const sf::Uint32 &);

        void setClick(const std::string &, const sf::Uint32 &);

        void setPosition(const sf::Vector2f &);

        void setInc(const sf::Vector2f &);

        void emptyText();

        void clear();

        ~Menu() {
            for (auto element: buttons) {
                delete element;
            };
            for (auto element: texts) {
                delete element;
            };
        }

    private:
        std::set<Button *> buttons;
        std::set<sf::Text *> texts;
        std::string id;
        sf::Vector2f startingPosition;
        sf::Vector2f currentPosition;
        sf::Vector2f incVector;
        bool active;
        Sound buttonHoverSound;
        Sound buttonClickSound;
        sf::Font menuFont;
        unsigned int charSize;
        sf::Color defaultColor;
        sf::Color hoverColor;
        sf::Color clickColor;
        sf::RectangleShape background;
    };

    class Overlay {
    public:
        Overlay() : active(false) {}

        void setActive(bool);

        bool isActive() const;

        void setText(const std::string &, unsigned int, const sf::Uint32 &);

        void addText(const std::string &, const sf::Vector2f &);

        void draw(sf::RenderWindow &) const;

        void emptyText();

        ~Overlay() {
            emptyText();
        }

    private:
        std::set<sf::Text *> texts;
        bool active;
        sf::Font font;
        unsigned int charSize;
        sf::Color defaultColor;
    };

    void Init();

    void Render(sf::RenderWindow &);

    void Update();

    void addText(const std::string &, const std::string &, unsigned int, const sf::Color &, const sf::Vector2f &);

    void removeTexts();

    void createMenu(const std::string &);

    void Destroy();

    extern std::map<std::string, Menu *> menus;
    extern std::map<std::string, Overlay *> overlays;
    extern std::map<std::string, std::function<void()>> triggers;
    extern std::map<std::string, sf::Font *> fonts;
    extern std::vector<sf::Text *> texts;
    extern std::map<std::string, const sf::Texture *> textures;
};


#endif //CS110_CAT_MOUSE_GUI_H
