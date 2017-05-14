#include "triggers.h"
#include "GUI.h"
#include "Game.h"

void triggers::quit() {
    Game::game.quit();
}

void triggers::startGame() {
    GUI::menus[GUI::Menus::Main]->setActive(false);
    GUI::menus[GUI::Menus::Modes]->setActive(true);
}

void triggers::back() {
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    GUI::menus[GUI::Menus::Main]->setActive(true);
}

void triggers::soloPlay() {
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    Game::game.setGameMode(0);
    for (auto element:GL::entities) {
        element.second->resetScore();
    }
    GL::world.populateWorld();
    GL::world.setActive(true);
}

void triggers::multiPlay() {
    GUI::menus[GUI::Menus::Modes]->setActive(false);
    Game::game.setGameMode(1);
    for (auto element:GL::entities) {
        element.second->resetScore();
    }
    GL::world.populateWorld();
    GL::world.setActive(true);
    GUI::addText("Player 1's (Mouse) turn","sansation", 32, sf::Color::Red, sf::Vector2f(100.f,100.f));
    GUI::addText("Use the W,A,S,D keys to Play!","sansation", 32, sf::Color::Red, sf::Vector2f(100.f,200.f));
}

void triggers::restartGame() {
    GUI::menus[GUI::Menus::EndGame]->emptyText();
    GUI::menus[GUI::Menus::EndGame]->setActive(false);
    GL::world.populateWorld();
    GL::world.setActive(true);
}

void triggers::backToMain() {
    GUI::menus[GUI::Menus::EndGame]->emptyText();
    GUI::menus[GUI::Menus::EndGame]->setActive(false);
    GUI::menus[GUI::Menus::Main]->setActive(true);
}