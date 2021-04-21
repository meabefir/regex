#pragma once
#include "State.h"
#include "Button.h"

class EditorState;

class NodeEditorState :
    public State
{
private:
	Node* node;
	std::unordered_map<std::string, Button*> buttons;
	std::string text;
	sf::Text textRender;
	sf::Text infoText;
	sf::Vector2f textPosition;
	sf::RectangleShape whiteBox;

	EditorState* editor;

public:
	NodeEditorState(sf::RenderWindow* window, std::vector<State*>* states, Node*, EditorState*);
	NodeEditorState(const State&, Node*);
	~NodeEditorState();

	void init();
	void initFont();

	void setText(std::string);
	void recenterText(sf::Text& txt, sf::Vector2f pos);

	void ok();
	void cancel();

	void handleEvents(sf::Event);
	void updateInput(const float& dt);
	void update(const float& dt);
	void draw(sf::RenderTarget*, sf::View* UIView = nullptr);
};

