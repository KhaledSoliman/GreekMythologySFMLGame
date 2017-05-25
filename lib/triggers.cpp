#include <iostream>
#include "triggers.h"
#include "GUI.h"
#include "Game.h"

void triggers::quit() {
    Game::game.quit();
}

void triggers::startGame() {
    Game::game.findUserProfiles();
}

void triggers::openSettings() {
    GUI::menus["main"]->setActive(false);
    GUI::menus["settings"]->setActive(true);
}

void triggers::toggleFullscreen() {
    Game::game.toggleFullscreen();
}

void triggers::selectProfile(unsigned int num) {
    Game::game.setUserProfile(num);
    GUI::menus["profiles"]->setActive(false);
    GUI::removeTexts();
    Game::game.startNewGame();
}

void triggers::toggleFPSOverlay() {
    GUI::overlays["fps"]->setActive(!GUI::overlays["fps"]->isActive());
}

void triggers::changeMusicVolume() {
    unsigned int vol = Game::game.getMusicVolume();
    sf::Event event;
    bool flag = false;
    GUI::Overlay* pOverlay = new GUI::Overlay;
    pOverlay->setText("sansation.ttf", 32, 0xFFFF00FF);
    pOverlay->setActive(true);
    GUI::overlays["musicVol"] = pOverlay;
    while (!flag) {
        while (Game::game.getWindow().window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                Game::game.setMusicVolume(vol);
                flag = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (vol != 100)
                    vol++;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (vol != 0)
                    vol--;
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M){
                vol =  vol == 0 ? 100 : 0;
            }
        }
        pOverlay->emptyText();
        pOverlay->addText("Current Volume =  " + std::to_string(vol) + " | Press Escape to stop changing the volume (Up to increase, down to decrease, m to toggle mute)",  sf::Vector2f(5.f,900.f));
        Game::game.renderFrame();
    }
    GUI::overlays.erase("musicVol");
    delete pOverlay;
}

void triggers::changeGFXVolume() {
    unsigned int vol = Game::game.getGFXVolume();
    sf::Event event;
    bool flag = false;
    GUI::Overlay* pOverlay = new GUI::Overlay;
    pOverlay->setText("sansation.ttf", 32, 0xFFFF00FF);
    pOverlay->setActive(true);
    GUI::overlays["gfxVol"] = pOverlay;
    while (!flag) {
        while (Game::game.getWindow().window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                Game::game.setGFXVolume(vol);
                flag = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (vol != 100)
                    vol++;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (vol != 0)
                    vol--;
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M){
                vol =  vol == 0 ? 100 : 0;
            }
        }
        pOverlay->emptyText();
        pOverlay->addText("Current Volume =  " + std::to_string(vol) + " | Press Escape to stop changing the volume (Up to increase, down to decrease, m to toggle mute)",  sf::Vector2f(5.f,900.f));
        Game::game.renderFrame();
    }
    GUI::overlays.erase("gfxVol");
    delete pOverlay;
}

void triggers::skillButton(const std::string &skill) {
}

void triggers::backToMainMenu() {
    for (auto menu : GUI::menus)
        if (menu.second->isActive())
            menu.second->setActive(false);
    GUI::menus["main"]->setActive(true);
}