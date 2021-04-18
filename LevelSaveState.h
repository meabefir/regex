#pragma once
#include "State.h"
#include "Button.h"
class LevelSaveState :
    public State
{
private:
    sf::RectangleShape whiteBox;
    sf::Text infoTextRender;
    sf::Text inputTextRender;

    std::string text;

    std::unordered_map<std::string, Button*> buttons;

public:
    LevelSaveState(sf::RenderWindow* window, std::vector<State*>* states);
    ~LevelSaveState();

    void setText(std::string);
    
    void initFont();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target = nullptr);
};

