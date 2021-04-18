#pragma once

#include <stack>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <math.h>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Node.h"
#include "Component.h"

class State
{
protected:
	std::vector<State*>* states;
	sf::RenderWindow* window;
	sf::Font* font;
	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// resources
	std::map<std::string, sf::Texture> textures;


public:
	State(sf::RenderWindow* window, std::vector<State*>* states);
	State(const State&);
	virtual ~State();

	// getter setter
	sf::Font* getFont();

	virtual void initFont();

	const bool& getQuit() const;
	void endState();

	void addState(State*);
	virtual void updateMousePositions();
	virtual void handleEvents(sf::Event);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void draw(sf::RenderTarget* target = nullptr) = 0;
};

