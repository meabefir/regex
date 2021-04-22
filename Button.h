#pragma once

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

enum button_states { BTN_IDLE = 0, BTN_PRESSED, BTN_ACTIVE, BTN_HELD};

class Button
{
private:
	short unsigned buttonState;
	bool pressed;
	bool hover;

	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;



public:
	Button(float x, float y, float width, float height,
		sf::Font* font, std::string text,
		sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int character_size = 50);
	~Button();

	// accessors
	const bool isPressed() const;

	// functions 
	void update(const sf::Vector2f mousePos);
	void draw(sf::RenderTarget* target);

};

