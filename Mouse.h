#pragma once

#include "State.h"

class Mouse
{
public:
	static sf::Vector2f pos;
	static sf::Vector2f relativePos;
	static sf::Vector2f movementSinceLastClick;
	static sf::Vector2f movementSinceMousePressed;
	static sf::Clock timeSinceMousePessedClock;
	static sf::Clock dClickClock;
	static sf::Clock freezeClock;
	static float freezeTime;

	static sf::Vector2i mousePosScreen;
	static sf::Vector2i mousePosWindow;
	static sf::Vector2f mousePosView;

	static bool pressed;
	static bool doubleClick;
	static bool normalClick;
	static bool stillClick;

	static float dClickDuration;
	static float clickDuration;

	static void freezeMouse(const float time);

	static void updateMousePos(sf::RenderWindow* window);
	static void update(sf::RenderWindow* window);
	static void resetAll();
};

