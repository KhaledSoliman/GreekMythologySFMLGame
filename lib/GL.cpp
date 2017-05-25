#include "GL.h"

std::map<std::string, const sf::Texture *> GL::textures;
GL::Environment *GL::environment;
GL::Overlay* GL::overlay;

void GL::Render(sf::RenderWindow &window) {
    environment->draw(window);
    overlay->draw(window);
}

void GL::Init() {
    sf::Texture *pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/water_texture.jpg");
    textures["water"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/grass_texture.jpg");
    textures["grass"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/skill_background.jpg");
    textures["skill"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/skill_tsunami.jpeg");
    textures["tsunami"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/earthquake_texture.jpg");
    textures["earthquake"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/touch_texture.jpg");
    textures["touch"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/lightning_texture.jpg");
    textures["lightning"] = pTexture;
    pTexture = new sf::Texture;
    pTexture->loadFromFile("../assets/textures/babies_texture.jpg");
    textures["babies"] = pTexture;
    environment = new Environment(Game::game.getWindow().window.getView().getSize());
    overlay = new Overlay;
    Skill* pSkill = new Skill("tsunami", sf::Vector2f(193.f * 2.f, 118.f * 8.f));
    overlay->addSkill(pSkill);
    pSkill = new Skill("lightning", sf::Vector2f(193.f * 3.f, 118.f * 8.f));
    overlay->addSkill(pSkill);
    pSkill = new Skill("earthquake", sf::Vector2f(193.f * 4.f, 118.f * 8.f));
    overlay->addSkill(pSkill);
    pSkill = new Skill("touch", sf::Vector2f(193.f * 5.f, 118.f * 8.f));
    overlay->addSkill(pSkill);
    pSkill = new Skill("babies", sf::Vector2f(193.f * 6.f, 118.f * 8.f));
    overlay->setText("sansation.ttf", 32, 0xFFFFFFFF);
    overlay->addSkill(pSkill);
}

void GL::Destroy() {
    for (auto element: textures)
        delete element.second;
    delete environment;
    textures.clear();
}

GL::Skill::Skill(const std::string &name,const sf::Vector2f& pos) : active(false), state(enabled) {
    this->name = name;
    content = new sf::RectangleShape;
    content->setSize(sf::Vector2f(189.f,114.f));
    content->setTexture(textures[name]);
    content->setPosition(pos.x + 2.f, pos.y + 2.f);
    background = new sf::RectangleShape;
    background->setPosition(pos);
    background->setSize(sf::Vector2f(193.f,118.f));
    background->setTexture(textures["skill"]);
}

bool GL::Overlay::clickScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos)) {
            element->trigger();
            return true;
        }
    }
    for(auto element: skills) {
        if (element->isMouseOver(mousePos)) {
            element->trigger();
            return true;
        }
    }
    return false;
}

void GL::Overlay::hoveringScan(const sf::Vector2f &mousePos) {
    for (auto element : buttons) {
        if (element->isMouseOver(mousePos) && element->getState() != hovered) {
            element->setState(hovered);
            break;
        } else if (!element->isMouseOver(mousePos) && element->getState() == hovered) {
            element->setState(enabled);
        }
    }
    for(auto element: skills){
        if (element->isMouseOver(mousePos) && element->getState() != hovered) {
            element->hover();
            break;
        } else if (!element->isMouseOver(mousePos) && element->getState() == hovered) {
            element->unHover();
        }
    }
}

void GL::Skill::hover(){
    state = hovered;
    overlay->addText(name, sf::Vector2f(content->getPosition().x,content->getPosition().y - 100.f));
}

void GL::Skill::unHover() {
    state = enabled;
    overlay->emptyText();
}

int GL::Skill::getState() const {
    return state;
}

bool GL::Skill::isActive() const {
    return active;
}

void GL::Skill::setActive(bool active) {
    this->active = active;
}

void GL::Skill::draw(sf::RenderWindow &window) {
    if(active){
        window.draw(*background);
        window.draw(*content);
    }
}

void GL::Skill::trigger(){

};

bool GL::Skill::isMouseOver(const sf::Vector2f &pos) const {
    return content->getGlobalBounds().contains(pos);
}

GL::Skill::~Skill() {
    delete background;
    delete content;
}

GL::Environment::Environment(const sf::Vector2f &size) : active(false) {
    active = false;
    grid = new Tile *[8];
    for (int i = 0; i < 8; i++)
        grid[i] = new Tile[10];
    float tileWidth = 1930.f / 10.f;
    float tileHeight = 1180.f / 10.f;
    sf::Vector2f currentPosition = sf::Vector2f(0.f, 0.f);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j].shape = new sf::RectangleShape;
            grid[i][j].shape->setSize(sf::Vector2f(tileWidth, tileHeight));
            grid[i][j].shape->setPosition(currentPosition);
            grid[i][j].shape->setTexture(textures["grass"]);
            grid[i][j].shape->setOutlineColor(sf::Color(0x203000FF));
            grid[i][j].shape->setOutlineThickness(1.f);
            currentPosition.x += tileWidth + 2.f;
        }
        currentPosition.x = 0.f;
        currentPosition.y += tileHeight + 2.f;
    }

}

bool GL::Environment::isActive() const {
    return active;
}

void GL::Environment::setActive(bool active) {
    this->active = active;
}

void GL::Environment::draw(sf::RenderWindow &window) {
    if (active) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 10; j++) {
                window.draw(*grid[i][j].shape);
            }
        }
    }
}

GL::Environment::~Environment() {
    for (int i = 0; i < 8; i++)
        delete[] grid[i];
}

bool GL::Overlay::isActive() const {
    return active;
}

void GL::Overlay::setActive(bool active) {
    this->active = active;
    for(auto element: skills)
        element->setActive(active);
}

void GL::Overlay::setText(const std::string &fontPath, unsigned int characterSize, const sf::Uint32 &color) {
    font.loadFromFile("../assets/fonts/" + fontPath);
    charSize = characterSize;
    defaultColor = sf::Color(color);
}

void GL::Overlay::addText(const std::string &value, const sf::Vector2f &pos) {
    sf::Text *pText = new sf::Text(value, font, charSize);
    pText->setFillColor(defaultColor);
    pText->setPosition(pos);
    texts.insert(pText);
}

void GL::Overlay::addSkill(Skill* pSkill) {
    skills.push_back(pSkill);
}

void GL::Overlay::emptyText() {
    for (auto element: texts) {
        delete element;
    }
    texts.clear();
}

void GL::Overlay::draw(sf::RenderWindow &window) const {
    if (active) {
        for (auto text: texts) {
            window.draw(*text);
        }
        for(auto button: buttons){
            window.draw(*button);
        }
        for(auto skill : skills){
            skill->draw(window);
        }
    }
}