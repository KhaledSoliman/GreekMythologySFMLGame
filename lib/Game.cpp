#include <iostream>
#include "Game.h"

Game::GameEngine Game::game;

Game::GameEngine::GameEngine() : currentEntity(0) {
}

void Game::GameEngine::renderWindow() {
    gameWindow.window.create(gameWindow.windowMode, gameWindow.windowTitle, gameWindow.windowStyle,
                             gameWindow.contextSettings);
    gameWindow.window.setVerticalSyncEnabled(gameWindow.verticalSync);
    gameWindow.window.setFramerateLimit(gameWindow.frameLimit);
    gameWindow.window.setActive(gameWindow.active);
    background.setBG("../assets/common/bg-1.jpg", gameWindow.window);
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
                if (gameMode && GL::world.isActive()) {
                    bool flag;
                    if (currentEntity) {
                        switch (event.key.code) {
                            case sf::Keyboard::Up:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::North);
                                flag = true;
                                break;
                            case sf::Keyboard::Right:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::East);
                                flag = true;
                                break;
                            case sf::Keyboard::Left:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::West);
                                flag = true;
                                break;
                            case sf::Keyboard::Down:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::South);
                                flag = true;
                                break;
                            default:
                                flag = false;
                                break;
                        }
                    } else {
                        switch (event.key.code) {
                            case sf::Keyboard::W:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::North);
                                flag = true;
                                break;
                            case sf::Keyboard::D:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::East);
                                flag = true;
                                break;
                            case sf::Keyboard::A:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::West);
                                flag = true;
                                break;
                            case sf::Keyboard::S:
                                GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::South);
                                flag = true;
                                break;
                            default:
                                flag = false;
                                break;
                        }
                    }

                    if (flag) {
                        currentEntity = 1 - currentEntity;
                        if(GL::world.isActive()){
                            GUI::removeTexts();
                            GUI::addText("Player " + std::to_string(currentEntity + 1) + "'s (" +
                                         (currentEntity ? "Cat" : "Mouse") + ") turn", "sansation", 32, sf::Color::Red,
                                         sf::Vector2f(100.f, 100.f));
                            GUI::addText(currentEntity ? "Use the Arrow Keys to Play!" : "Use the W,A,S,D keys to Play!",
                                         "sansation", 32, sf::Color::Red, sf::Vector2f(100.f, 200.f));
                        }
                    }
                } else if (GL::world.isActive()) {
                    bool flag;
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                        case sf::Keyboard::Up:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::North);
                            flag = true;
                            break;
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::East);
                            flag = true;
                            break;
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::West);
                            flag = true;
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::Down:
                            GL::world.moveEntity(static_cast<GL::EntityType>(currentEntity), GL::Direction::South);
                            flag = true;
                            break;
                        default:
                            flag = false;
                            break;
                    }
                    if (flag && GL::world.isActive()){
                        GL::world.moveEntity(GL::EntityType::Cat, static_cast<GL::Direction>(rand() % 4));
                    }
                }

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