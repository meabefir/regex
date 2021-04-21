#pragma once
#include "Component.h"
#include "EditorState.h"

class NodeRemovalComp :
    public Component
{
private:
    EditorState* editor;
    sf::CircleShape shape;
    sf::Clock clock;
    bool canIncrementTime;
    float timePassed;

public:
    NodeRemovalComp(EditorState* editor);
    ~NodeRemovalComp();

    void update();
    void draw(sf::RenderTarget*, sf::View* UIView = nullptr);
};

