#pragma once
#include "State.h"
#include "Mouse.h"
#include <stdlib.h>
#include <sys/stat.h>

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
    void removeNode(Node*);
    Node* getNodeFromId(std::string id);

    // getter setter
    Node* getHoverNode();
    std::unordered_map<std::string, Component*>* getComponents();
    Node* getStartNode();
    void setInteractigWith(Node*);
    Node* getInteractingWith();

    // initializers
    void initFont();
    void initComponents();

    void reset();

    void buildAutomata();

    // components
    void deactivateAllBut(std::string);
    void activateAllBut(std::string);
    void deactivateComponent(std::string);
    void activateComponent(std::string);
    void activateAll();

    void handleEvents(sf::Event);
    void updateInput(const float& dt);
    void update(const float& dt);
    void draw(sf::RenderTarget* target, sf::View* UIView = nullptr);

    void drawNodes(sf::RenderTarget* target);
    void interactionStarted(Node* node);

    // testing
    void shuffleNodes();

    friend class Node;
};

