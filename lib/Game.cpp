#include <iostream>
#include "Game.h"
#include <time.h>

Game::GameEngine Game::game;

Game::GameEngine::GameEngine() : currentEntity(0) {
}

void Game::GameEngine::renderWindow() {
    gameWindow.window.create(gameWindow.windowMode, gameWindow.windowTitle, gameWindow.windowStyle,
                             gameWindow.contextSettings);
    gameWindow.window.setVerticalSyncEnabled(gameWindow.verticalSync);
    gameWindow.window.setFramerateLimit(gameWindow.frameLimit);
    gameWindow.window.setActive(gameWindow.active);
    std::srand(std::time(nullptr));
    background.setBG("../assets/common/bg-" + std::to_string(std::rand() % 4 + 1) + ".jpg", gameWindow.window);
}

void Game::GameEngine::renderFrame() {
    clock.restart();
    background.draw(gameWindow.window);
    GUI::Render(gameWindow.window);
    GL::Render(gameWindow.window);
    gameWindow.window.display();
    timePerFrame = clock.getElapsedTime();
}

void Game::GameEngine::run() {
    renderWindow();
    while (gameWindow.window.isOpen()) {
        sf::Event event;
        while (gameWindow.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                quit();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                bool found;
                for (auto element: GUI::menus)
                    if (element.second->isActive()) {
                        found = element.second->clickScan(
                                gameWindow.window.mapPixelToCoords(
                                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                        if (found) {
                            while (gameWindow.window.pollEvent(event)); //Drain All Events if found!
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
            renderFrame();
        }
    }
}

void Game::GameEngine::quit() {
    GL::Destroy();
    GUI::Destroy();
    gameWindow.window.close();
}

sf::Time Game::GameEngine::getFrameTime() const {
    return timePerFrame;
}

void Game::GameEngine::setGameMode(int mode) {
    gameMode = mode;
}

int Game::GameEngine::getGameMode() const {
    return gameMode;
}

void Game::GameEngine::setCurrentEntity(int entityType) {
    currentEntity = entityType;
}

int Game::GameEngine::getCurrentEntity() const {
    return currentEntity;
}

void Game::GameEngine::setBackground(const std::string &path) {
    background.setBG(path, gameWindow.window);
}

void Game::GameEngine::setBackground(const sf::Color &color) {
    background.useImage = false;
    background.backgroundColor = color;
}