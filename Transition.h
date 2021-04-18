#pragma once

#include "State.h"
#include "Spline.h"
#include "SplineSelf.h"

class Node;
class EditorState;
class TransitionEditorState;

class Transition
{
private:
	Node* node1;
	Node* node2;
	EditorState* editor;

	std::string text;
	sf::Text textRender;

	sf::Vector2f midPos;

	sf::Color color;

	Spline* spline;

	bool deleteVisible;
	sf::CircleShape deleteButton;

public:
	Transition(Node* node1, Node* node2, sf::Color color = sf::Color::Blue);
	Transition(Node* node1, Node* node2, std::string text, sf::Color color = sf::Color::Blue);
	~Transition();

	// setter getter
	void setMidPos(sf::Vector2f);
	Node* getNode1();
	Node* getNode2();
	std::string getText();

	void update();
	void draw(sf::RenderTarget* target);
	
	void setText(std::string);

	void openEditMode();

	friend class Node;
	friend class TransitionEditorState;
	friend class Spline;
};
