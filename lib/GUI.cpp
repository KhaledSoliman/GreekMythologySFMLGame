#include <iostream>
#include "GUI.h"
#include "triggers.h"
#include "Game.h"
#include "INIReader.h"

std::map<std::string, GUI::Menu *> GUI::menus;
std::map<std::string, GUI::Overlay *> GUI::overlays;
std::map<std::string, std::function<void()>> GUI::triggers;
std::map<std::string, sf::Font *> GUI::fonts;
std::vector<sf::Text *> GUI::texts;
std::map<std::string, const sf::Texture *> GUI::textures;

void GUI::Init() {
    sf::Font *pFont = new sf::Font;
    pFont->loadFromFile("../assets/fonts/PoseiAOE.ttf");
    fonts["PoseiAOE"] = pFont;
    pFont = new sf::Font;
    pFont->loadFromFile("../assets/fonts/sansation.ttf");
    fonts["sansation"] = pFont;
    sf::Texture *pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/menu_texture.jpg");
    textures["menu_bg"] = pTexture;
    triggers["New Game"] = triggers::startGame;
    triggers["Settings"] = triggers::openSettings;
    triggers["Music Volume"] = triggers::changeMusicVolume;
    triggers["GFX Volume"] = triggers::changeGFXVolume;
    triggers["Return to Main Menu"] = triggers::backToMainMenu;
    triggers["FullScreen Toggle"] = triggers::toggleFullscreen;
    triggers["FPS Toggle"] = triggers::toggleFPSOverlay;
    triggers["Quit"] = triggers::quit;
    addText("Day of Reckoning: The Road to Athens", "PoseiAOE", 72, sf::Color(0x990000FF), sf::Vector2f(600.f, 50.f));
    createMenu("main");
    createMenu("settings");
    createMenu("profiles");
    menus["main"]->setActive(true);
    Overlay *pOverlay = new GUI::Overlay;
    pOverlay->setText("sansation.ttf", 32, 0xFFFF00FF);
    overlays["fps"] = pOverlay;
    pOverlay = new GUI::Overlay;
    pOverlay->setText("sansation.ttf", 32, 0xFFFF00FF);
    pOverlay->addText("V0.0.1 Alpha", sf::Vector2f(1700.f, 1.f));
    pOverlay->setActive(true);
    overlays["version"] = pOverlay;
}

void GUI::Update() {
    overlays["fps"]->emptyText();
    overlays["fps"]->addText(std::to_string(Game::game.getFPS()) + " FPS | Frame Time: " +
                             std::to_string(Game::game.getFrameTime().asMilliseconds()) + " milliseconds",
                             sf::Vector2f(1.f, 1.f));
}

void GUI::Render(sf::RenderWindow &window) {
    for (auto element: texts) {
        window.draw(*element);
    }
    for (auto element: menus) {
        element.second->draw(window);
    }
    for (auto element: overlays) {
        element.second->draw(window);
    }
}

void GUI::addText(const std::string &text, const std::string &font, unsigned int charSize, const sf::Color &color,
                  const sf::Vector2f &pos) {
    sf::Text *pText = new sf::Text(text, *fonts[font], charSize);
    pText->setFillColor(color);
    pText->setPosition(pos);
    texts.push_back(pText);
}

void GUI::removeTexts() {
    for (auto element: texts)
        delete element;
    texts.clear();
}

void GUI::createMenu(const std::string &menuName) {
    INIReader reader("../config/menus.ini");
    std::string section = "menu-" + menuName;
    unsigned int numOfButtons = reader.GetUI(section, "buttons", 0);
    float posx = reader.GetFloat(section, "posx", 0.f);
    float posy = reader.GetFloat(section, "posy", 0.f);
    float incx = reader.GetFloat(section, "incx", 0.f);
    float incy = reader.GetFloat(section, "incy", 0.f);
    std::string menuFont = reader.Get(section, "font", "F");
    std::string hoverSound = reader.Get(section, "hover", "F");
    std::string clickSound = reader.Get(section, "click", "F");
    sf::Uint32 defaultColor = reader.GetUI(section, "defaultColor", 0xFFFFFFFF);
    sf::Uint32 hoverColor = reader.GetUI(section, "hoverColor", 0xFFFFFFFF);
    sf::Uint32 clickColor = reader.GetUI(section, "clickColor", 0xFFFFFFFF);
    unsigned int charSize = reader.GetUI(section, "pixelSize", 16);
    std::string buttonName;
    Menu *pMenu = new Menu;
    pMenu->setText(menuFont, charSize, defaultColor);
    pMenu->setHover(hoverSound, hoverColor);
    pMenu->setClick(clickSound, clickColor);
    pMenu->setInc(sf::Vector2f(incx, incy));
    pMenu->setPosition(sf::Vector2f(posx, posy));
    for (int i = 0; i < numOfButtons; i++) {
        buttonName = reader.Get(section, "button - " + std::to_string(i), "ERROR:UNABLE TO READ BUTTON VALUE");
        pMenu->addButton(buttonName);
    }
    pMenu->createBackground();
    menus[menuName] = pMenu;
}

void GUI::Destroy() {
    for (auto element: menus)
        delete element.second;
    for (auto element: overlays)
        delete element.second;
    for (auto element: texts)
        delete element;
    for (auto element: fonts)
        delete element.second;
    for (auto element: textures)
        delete element.second;
    menus.clear();
    texts.clear();
    textures.clear();
    overlays.clear();
    fonts.clear();
}

GUI::Button::Button() : Text(), state(disabled), clickAble(false), profileButton(false), profileNum(0) {}

GUI::Button::Button(const std::string &value, const sf::Font &font, const sf::Color &color, unsigned int charSize)
        : Text(value, font, charSize), state(enabled), clickAble(false), profileButton(false), profileNum(0) {
    setFillColor(color);
};

void GUI::Button::setState(int newState) {
    state = newState;
}

void GUI::Button::setProfileButton(unsigned int num) {
    profileButton = true;
    profileNum = num;
    clickAble = true;
}

int GUI::Button::getState() const {
    return state;
}

bool GUI::Button::isMouseOver(const sf::Vector2f &pos) const {
    return getGlobalBounds().contains(pos);
}

void GUI::Button::bindFunction(std::function<void()> trigger) {
    callBack = trigger;
    clickAble = true;
}

void GUI::Button::trigger() {
    if (clickAble) {
        if (profileButton) {
            triggers::selectProfile(profileNum);
        } else {
            callBack();
        }
    }
}

void GUI::Menu::addButton(const std::string &value) {
    Button *pButton = new Button(value, menuFont, defaultColor, charSize);
    pButton->setPosition(currentPosition);
    buttons.insert(pButton);
    if (triggers.find(value) != triggers.end())
        pButton->bindFunction(triggers[value]);
    else {
        std::string verificationValue = value.substr(0,7);
        if (verificationValue == "Name: \"") {
            pButton->setProfileButton((unsigned int) std::distance(buttons.begin(), buttons.find(pButton)));
        }
    }
    currentPosition += incVector;
}


void GUI::Menu::addText(const std::string &value) {
    sf::Text *pText = new sf::Text(value, menuFont, charSize);
    pText->setFillColor(defaultColor);
    pText->setPosition(currentPosition);
    texts.insert(pText);
    currentPosition += incVector;
}

void GUI::Menu::draw(sf::RenderWindow &window) const {
    if (active) {
        window.draw(background);
        for (auto button: buttons) {
            window.draw(*button);
        }
        for (auto text: texts) {
            window.draw(*text);
        }
    }

}

void GUI::Menu::setIdentity(const std::string &id) {
    this->id = id;
}

void GUI::Menu::setActive(bool flag) {
    if (active) {
        sf::Time totalTime = sf::seconds(0.0f);
        sf::Time time = sf::seconds(0.5f);
        sf::Time dt = sf::seconds(1.0f / 60.0f);
        sf::Vector2f offsetVector = sf::Vector2f(-1000.f, 0.f);
        while (totalTime <= time) {
            for (auto element: texts)
                element->move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            for (auto element: buttons)
                element->move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            background.move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            Game::game.renderFrame();
            totalTime += dt;
        }
        active = flag;
    } else {
        active = flag;
        sf::Time totalTime = sf::seconds(0.0f);
        sf::Time time = sf::seconds(0.5f);
        sf::Time dt = sf::seconds(1.0f / 60.0f);
        sf::Vector2f offsetVector = sf::Vector2f(1000.f, 0.f);
        while (totalTime <= time) {
            for (auto element: texts)
                element->move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            for (auto element: buttons)
                element->move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            background.move((dt.asSeconds() / time.asSeconds()) * offsetVector.x, 0);
            Game::game.renderFrame();
            totalTime += dt;
        }
    }
}

bool GUI::Menu::isActive() const {
    return active;
}

void GUI::Menu::setPosition(const sf::Vector2f &pos) {
    currentPosition = startingPosition = pos;
}

void GUI::Menu::setInc(const sf::Vector2f &inc) {
    incVector = inc;
}

void GUI::Menu::clear() {
    for (auto element: texts) {
        delete element;
    }
    for (auto element: buttons) {
        delete element;
    }
    texts.clear();
    buttons.clear();
    currentPosition = startingPosition;
}

void GUI::Menu::emptyText() {
    for (auto element: texts) {
        delete element;
        currentPosition -= incVector;
    }
    texts.clear();
    createBackground();
}

void GUI::Menu::createBackground() {
    sf::Vector2f size(50.f, 50.f);
    float maxWidth = 0;
    for (auto element: texts) {
        size.y += element->getLocalBounds().height + incVector.y;
        maxWidth = element->getLocalBounds().width > maxWidth ? element->getLocalBounds().width : maxWidth;
    }
    for (auto element: buttons) {
        size.y += element->getLocalBounds().height + incVector.y;
        maxWidth = element->getLocalBounds().width > maxWidth ? element->getLocalBounds().width : maxWidth;
    }
    size.x += maxWidth;
    background.setSize(size);
    background.setTexture(textures["menu_bg"]);
    background.setFillColor(sf::Color(0xFFFFFFA1));
    background.setPosition(startingPosition - sf::Vector2f(25.f, 25.f));
}

bool GUI::Menu::clickScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos)) {
            element->setState(enabled);
            element->setFillColor(clickColor);
            buttonClickSound.sound.setVolume((float) Game::game.getGFXVolume());
            buttonClickSound.sound.play();
            element->setFillColor(defaultColor);
            element->trigger();
            return true;
        }
    }
    return false;
}

void GUI::Menu::hoveringScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos) && element->getState() != hovered) {
            buttonHoverSound.sound.setVolume((float) Game::game.getGFXVolume());
            buttonHoverSound.sound.play();
            element->setState(hovered);
            element->setFillColor(hoverColor);
            break;
        } else if (!element->isMouseOver(mousePos) && element->getState() == hovered) {
            element->setState(enabled);
            element->setFillColor(defaultColor);
        }
    }
}

void GUI::Menu::setText(const std::string &fontPath, unsigned int characterSize, const sf::Uint32 &color) {
    menuFont.loadFromFile("../assets/fonts/" + fontPath);
    charSize = characterSize;
    defaultColor = sf::Color(color);
}

void GUI::Menu::setClick(const std::string &name, const sf::Uint32 &color) {
    buttonClickSound.setSound("../assets/sound/" + name);
    clickColor = sf::Color(color);
}

void GUI::Menu::setHover(const std::string &name, const sf::Uint32 &color) {
    buttonHoverSound.setSound("../assets/sound/" + name);
    hoverColor = sf::Color(color);
}

bool GUI::Overlay::isActive() const {
    return active;
}

void GUI::Overlay::setActive(bool active) {
    this->active = active;
}

void GUI::Overlay::setText(const std::string &fontPath, unsigned int characterSize, const sf::Uint32 &color) {
    font.loadFromFile("../assets/fonts/" + fontPath);
    charSize = characterSize;
    defaultColor = sf::Color(color);
}

void GUI::Overlay::addText(const std::string &value, const sf::Vector2f &pos) {
    sf::Text *pText = new sf::Text(value, font, charSize);
    pText->setFillColor(defaultColor);
    pText->setPosition(pos);
    texts.insert(pText);
}

void GUI::Overlay::emptyText() {
    for (auto element: texts) {
        delete element;
    }
    texts.clear();
}

void GUI::Overlay::draw(sf::RenderWindow &window) const {
    if (active) {
        for (auto text: texts) {
            window.draw(*text);
        }
    }
}