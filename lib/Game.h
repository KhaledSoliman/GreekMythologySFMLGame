#ifndef CS110_CAT_MOUSE_GAME_H
#define CS110_CAT_MOUSE_GAME_H

#include <SFML/Graphics.hpp>
#include "GL.h"
#include "GUI.h"

namespace Game {
    struct Background {
        bool useImage; //0 for color, 1 for image
        sf::Sprite image;
        sf::Texture texture;
        sf::Color backgroundColor;

        Background() : useImage(false), backgroundColor(sf::Color::Black) {}

        void setBG(const std::string &path, sf::RenderWindow &window) {
            useImage = true;
            texture.setSmooth(true);
            texture.setSrgb(true);
            texture.loadFromFile(path);
            image.setTexture(texture);
            image.setScale((float) window.getSize().x / (float) texture.getSize().x,
                           (float) window.getSize().y / (float) texture.getSize().y);
        }

        void draw(sf::RenderWindow &window) {
            if (useImage) {
                window.clear();
                window.draw(image);
            } else {
                window.clear(backgroundColor);
            }
        }
    };

    struct Window {
        sf::VideoMode windowMode;
        std::string windowTitle;
        sf::Uint32 windowStyle;
        bool verticalSync;
        bool active;
        unsigned int frameLimit;
        sf::ContextSettings contextSettings;
        sf::RenderWindow window;

        Window() {
            windowMode = sf::VideoMode().getDesktopMode();
            windowTitle = "Mouse and Cat meow meow!";
            windowStyle = sf::Style::Default;
            verticalSync = false;
            active = true;
            frameLimit = 60;
            setContextSettings(24, 16, true, 8);
        }

        void setContextSettings(unsigned int depthBits, unsigned int AL, bool sRgb, unsigned int stencilBits) {
            contextSettings.depthBits = depthBits;
            contextSettings.antialiasingLevel = AL;
            contextSettings.sRgbCapable = sRgb;
            contextSettings.stencilBits = stencilBits;
        }

        Window(const Window &) = delete;

        Window &operator=(const Window &) = delete;
    };

    class GameEngine {
    public:
        GameEngine();

        void renderWindow();

        void renderFrame();

        void run();

        void quit();

        sf::Time getFrameTime() const;

        void setGameMode(int);

        int getGameMode() const;

        void setCurrentEntity(int);

        int getCurrentEntity() const;

        void setBackground(const std::string &);

        void setBackground(const sf::Color &);

        GameEngine(const GameEngine &) = delete;

        GameEngine &operator=(const GameEngine &) = delete;

    private:
        Window gameWindow;
        sf::Clock clock;
        sf::Time timePerFrame;
        Background background;
        int gameMode;
        int currentEntity;
    };

    extern GameEngine game;
};


#endif //CS110_CAT_MOUSE_GAME_H
