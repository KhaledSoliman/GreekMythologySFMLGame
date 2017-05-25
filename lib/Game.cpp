#include <iostream>
#include "Game.h"
#include "INIReader.h"

Game::GameEngine Game::game;

Game::GameEngine::GameEngine() : userProfile(0), userProfileSelected(false), musicPlayed(false), inGame(false),
                                 fullscreen(false), gfxVolume(100), musicVolume(100), frameCounter(0) {
}

void Game::GameEngine::renderWindow() {
    gameWindow.window.create(gameWindow.windowMode, gameWindow.windowTitle, gameWindow.windowStyle,
                             gameWindow.contextSettings);
    gameWindow.window.setVerticalSyncEnabled(gameWindow.verticalSync);
    gameWindow.window.setFramerateLimit(gameWindow.frameLimit);
    gameWindow.window.setActive(gameWindow.active);
    std::srand(std::time(0));
    background.setBG("../assets/common/bg-" + std::to_string(std::rand() % 4 + 1) + ".jpg", gameWindow.window);
}

void Game::GameEngine::setUserProfile(unsigned int num) {
    userProfile = num;
    userProfileSelected = true;
}

void Game::GameEngine::renderFrame() {
    clock.restart();
    gameWindow.window.clear((sf::Color(0x000000FF)));
    if (!inGame)
        background.draw(gameWindow.window);
    GUI::Render(gameWindow.window);
    GL::Render(gameWindow.window);
    gameWindow.window.display();
    timePerFrame = clock.getElapsedTime();
}

void Game::GameEngine::startNewGame() {
    inGame = true;
    GL::environment->setActive(true);
}

void Game::GameEngine::findUserProfiles() {
    INIReader reader("../config/profiles/profile.ini");
    std::string settingProfile = "profile-0";
    std::string profile = "profile-";
    unsigned int num = reader.GetUI(settingProfile, "number of profiles", 0);
    GUI::menus["profiles"]->clear();
    GUI::menus["profiles"]->addText("Please select your user profile or create a new one");
    GUI::menus["profiles"]->addText("User Profiles Found: " + std::to_string(num));
    GUI::menus["profiles"]->addButton("Create a new profile");
    if (num) {
        for (int i = 1; i <= num; i++) {
            GUI::menus["profiles"]->addButton(
                    "Name: " + reader.Get(profile + std::to_string(i), "name", "NAME UNREADABLE!")
                    + " Victories: " + std::to_string(reader.GetUI(profile + std::to_string(i), "victories", 0))
                    + " Defeats: " + std::to_string(reader.GetUI(profile + std::to_string(i), "defeats", 0)));
        }
    }
    GUI::menus["profiles"]->addButton("Return to Main Menu");
    GUI::menus["profiles"]->createBackground();
    GUI::menus["main"]->setActive(false);
    GUI::menus["profiles"]->setActive(true);
}

void Game::GameEngine::toggleFullscreen() {
    if (fullscreen)
        gameWindow.windowStyle = sf::Style::Default;
    else
        gameWindow.windowStyle = sf::Style::Fullscreen;
    fullscreen = !fullscreen;
    renderWindow();
}

void Game::GameEngine::playMusic(const std::string &path) {
    if (!musicPlayed) {
        music.setLoop(true);
        music.openFromFile(path);
        music.play();
        musicPlayed = true;
    }
}

void Game::GameEngine::stopMusic() {
    music.stop();
    musicPlayed = false;
}

unsigned int Game::GameEngine::getMusicVolume() const {
    return musicVolume;
}

void Game::GameEngine::setMusicVolume(unsigned int vol) {
    musicVolume = vol;
    music.setVolume((float) vol);
}

unsigned int Game::GameEngine::getGFXVolume() const {
    return gfxVolume;
}

void Game::GameEngine::setGFXVolume(unsigned int vol) {
    gfxVolume = vol;
}

void Game::GameEngine::run() {
    renderWindow();
    while (gameWindow.window.isOpen()) {
        sf::Event event;
        if (GUI::menus["main"]->isActive()) playMusic("../assets/music/Main_Menu_Music.wav");
        while (gameWindow.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                quit();
                break;
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                bool found;
                for (auto element: GUI::menus)
                    if (element.second->isActive()) {
                        found = element.second->clickScan(
                                gameWindow.window.mapPixelToCoords(
                                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                        if (found) {
                            while (gameWindow.window.pollEvent(event));
                            break;
                        }
                    }
            } else if (event.type == sf::Event::MouseMoved) {
                for (auto element: GUI::menus)
                    if (element.second->isActive())
                        element.second->hoveringScan(
                                gameWindow.window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
            } else if (event.type == sf::Event::KeyPressed) {


            }
        }
        framesClock.restart();
        if (gameWindow.window.isOpen()) renderFrame();
        framesTime += framesClock.getElapsedTime();
        frameCounter++;
        if (framesTime.asMilliseconds() > 1000) {
            GUI::Update();
            frameCounter = 0;
            framesTime = sf::seconds(0);
        }
    }
}

void Game::GameEngine::quit() {
    GL::Destroy();
    GUI::Destroy();
    stopMusic();
    gameWindow.window.close();
}

sf::Time Game::GameEngine::getFrameTime() const {
    return timePerFrame;
}

int Game::GameEngine::getFPS() const {
    return frameCounter;
}

void Game::GameEngine::setBackground(const std::string &path) {
    background.setBG(path, gameWindow.window);
}

void Game::GameEngine::setBackground(const sf::Color &color) {
    background.useImage = false;
    background.backgroundColor = color;
}

Game::Window &Game::GameEngine::getWindow() {
    return gameWindow;
}