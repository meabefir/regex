#pragma once
#include "State.h"
#include "Mouse.h"
#include <stdlib.h>

class NodeEditorState;
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

public:
    EditorState(sf::RenderWindow* window, std::vector<State*>* states);
    ~EditorState();

    void saveLevel(std::string name);
    void loadLevel(std::string name);
    void clearNodes();
    Node* getNodeFromId(std::string id);

    // getter
    Node* getHoverNode();
    std::unordered_map<std::string, Component*>* getComponents();
    Node* getStartNode();

    // initializers
    void initFont();
    void initComponents();

    void reset();

    void buildAutomata();

    // components
    void deactivateAllBut(std::string);
    void activateAll();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target);

    void drawNodes(sf::RenderTarget* target);
    void interactionStarted(Node* node);

    // testing
    void shuffleNodes();

    friend class Node;
};

