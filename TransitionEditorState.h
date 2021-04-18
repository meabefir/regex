#pragma once
#include "State.h"

class Button;
class Transition;

class TransitionEditorState:
	public State
{
private:
	Transition* transition;
	std::unordered_map<std::string, Button*> buttons;
	std::string text;
	sf::Text textRender;
	sf::Text infoText;
	sf::Vector2f textPosition;
	sf::RectangleShape whiteBox;

public:
	TransitionEditorState(sf::RenderWindow* window, std::vector<State*>* states, Transition*);
	TransitionEditorState(const State&, Transition*);
	~TransitionEditorState();

	void init();
	void initFont();

	void setText(std::string);
	void recenterText(sf::Text& txt, sf::Vector2f pos);

	void ok();
	void cancel();

	void handleEvents(sf::Event);
	void updateInput(const float& dt);
	void update(const float& dt);
	void draw(sf::RenderTarget*);
};

