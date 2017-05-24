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
            windowTitle = "Day of Reckoning: The Road to Athens V0.0.1 Alpha";
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


    struct UserProfile {
        std::string userName;
        unsigned int victories;
        unsigned int defeats;
        UserProfile(const std::string& userName,unsigned int victories, unsigned int defeats) {
            this->userName = userName;
            this->victories = victories;
            this->defeats = defeats;
        }
    };

    class GameEngine {
    public:
        GameEngine();

        void renderWindow();

        void renderFrame();

        void run();

        void quit();

        int getFPS() const;

        void playMusic(const std::string&);

        void stopMusic();

        void findUserProfiles();

        void setUserProfile(unsigned int);

        void toggleFullscreen();

        void setMusicVolume(unsigned int);

        unsigned int getMusicVolume() const;

        void setGFXVolume(unsigned int);

        unsigned int getGFXVolume() const;

        void startNewGame();

        sf::Time getFrameTime() const;

        void setBackground(const std::string &);

        void setBackground(const sf::Color &);

        GameEngine(const GameEngine &) = delete;

        Window& getWindow();

        GameEngine &operator=(const GameEngine &) = delete;

    private:
        Window gameWindow;
        bool inGame;
        sf::Clock clock;
        sf::Clock framesClock;
        sf::Time timePerFrame;
        Background background;
        bool musicPlayed;
        sf::Music music;
        int frameCounter;
        sf::Time framesTime;
        bool userProfileSelected;
        unsigned int musicVolume;
        unsigned int gfxVolume;
        unsigned int userProfile;
        bool fullscreen;
    };

    extern GameEngine game;
};


#endif //CS110_CAT_MOUSE_GAME_H
