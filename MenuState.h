#pragma once
#include "State.h"
#include "Button.h"

class MenuState :
    public State
{
private:
    std::unordered_map<std::string, Button*> buttons;
    sf::RectangleShape whiteBox;

public:
    MenuState(sf::RenderWindow* window, std::vector<State*>* states);
    ~MenuState();

    void initFont();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target = nullptr);
};

