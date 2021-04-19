#pragma once

#include "Mouse.h"
#include "AppData.h"

class Transition;
class EditorState;

class Node
{
private:
	sf::CircleShape shape;
	sf::Font* font;
	sf::Text text;

	std::string name;
	std::string id;
	std::vector<Transition*> transitions;

	bool mouseOver;
	bool mouseClicked;
	bool active;

	bool isStartingNode;
	bool isFinalNode;

	static sf::Color defaultNodeColor;
	static sf::Color startNodeColor;
	static sf::Color finalNodeColor;
	static sf::Color startFinalNodeColor;
	static sf::Color activeColor;

	float radius;

	EditorState* editor;

public:
	Node(EditorState* editor, sf::Font*, sf::Vector2f pos, std::string name = "", 
					const float radius = AppData::nodeRadius, const float outlineThick = 4.f);
	Node(EditorState* editor, sf::Font*, sf::Vector2f pos, std::string id, std::string name,
		const float radius = AppData::nodeRadius,
		const float outlineThick = 4.f);
	~Node();

	void initVariables();

	// getters
	const bool getMouseOver();
	const sf::CircleShape& getShape() const;
	EditorState* getEditor();
	bool isActive();
	float getRadius();
	std::string getName();
	std::string getId();
	void setName(std::string);
	std::vector<Transition*>* getTransitions();
	bool isStartNode();

	const float getDistToVector(const sf::Vector2f&);

	void toggleFinal();
	void toggleStart(bool self = false);
	sf::Color getCurrentColor();

	void setText(std::string);
	void openEditMode();

	void updateMouseInput(const sf::Vector2f& mouse_pos);
	void checkForInteraction();
	void resetTransitionCreation();

	void addTransitionToNode(Node* node);
	void addTransitionToNode(Node* node, std::string text);
	void removeTransition(Transition* trans);

	void update();
	void draw(sf::RenderTarget* target);
	void drawTransitions(sf::RenderTarget*);
};

