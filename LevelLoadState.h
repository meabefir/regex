#pragma once
#include "State.h"
#include "Button.h"
#include <fstream>
#include <filesystem>

class LevelLoadState :
    public State
{
private:
    sf::RectangleShape whiteBox;
    sf::Text infoTextRender;
    sf::Text inputTextRender;

    std::string text;

    std::unordered_map<std::string, Button*> buttons;

public:
    LevelLoadState(sf::RenderWindow* window, std::vector<State*>* states);
    ~LevelLoadState();

    void initFont();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target = nullptr);
};

