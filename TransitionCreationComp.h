#pragma once
#include "Component.h"
#include "NodeCreationComp.h"
#include "Component.h"

class Node;
class EditorState;

class TransitionCreationComp :
    public Component
{
private:
    EditorState* editor;
    Node* node;

    sf::Color color;
    sf::VertexArray line;

public:
    TransitionCreationComp(EditorState* editor);
    ~TransitionCreationComp();

    void createTransitionTo(Node*);

    void update();
    void draw(sf::RenderTarget*);
};

