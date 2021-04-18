#pragma once
#include "State.h"
#include "Mouse.h"

class NodeCreationComponent;

class EditorState :
    public State
{
private:
    float radius;
    std::vector<Node*> nodes;
    std::unordered_map<std::string, Component*> components;
    std::unordered_map<std::string, int> sleepingComponents;
    Node* interactingWithNode;
    Node* hoveringNode;

    static int nodeId;

public:
    EditorState(sf::RenderWindow* window, std::vector<State*>* states);
    ~EditorState();

    void saveLevel(std::string name);

    // getter
    Node* getHoverNode();
    std::unordered_map<std::string, Component*>* getComponents();
    int getNodeId();

    // initializers
    void initFont();
    void initComponents();

    void reset();

    // components
    void deactivateAllBut(std::string);
    void activateAll();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target);

    void drawNodes(sf::RenderTarget* target);
    void interactionStarted(Node* node);

    friend class Node;
};

