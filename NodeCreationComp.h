#pragma once
#include "Component.h"
#include "Node.h"
#include "EditorState.h"

class NodeCreationComp :
    public Component
{
private:
    std::vector<Node*>* nodes;
    EditorState* editor;
    float radius;
    int nodeId;

public:
    NodeCreationComp(EditorState*, std::vector<Node*>* nodes);
    ~NodeCreationComp();

    void createNode(sf::Vector2f pos);
    float getClosestNodeDistance();

    Node* attemptNodeCreation();

    void update();
};

