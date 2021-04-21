#pragma once
#include "State.h"

class Game
{
private:
    sf::RenderWindow* window;
    sf::View* view;
    sf::View* UIView;
    sf::Event e;

    sf::Clock dtClock;
    float dt;

    std::vector<State*> states;

public:
    Game();
    ~Game();

    // initializers
    void initWindow();
    void initStates();

    // engine
    void updateDt();
    void engineUpdate();

    void run();
    void handleEvents();
    void update();
    void draw();
};

