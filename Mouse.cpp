#include "Mouse.h"
#include "Helper.h"

sf::Vector2f Mouse::pos = { 0.f, 0.f };

sf::Vector2f Mouse::relativePos = { 0, 0 };

sf::Vector2f Mouse::movementSinceLastClick = { 0.f, 0.f };

sf::Vector2f Mouse::movementSinceMousePressed = { 0.f, 0.f };

sf::Vector2i Mouse::lastMousePosWindow = {0, 0 };

sf::Vector2i Mouse::mousePosScreen = { 0, 0 };

sf::Vector2i Mouse::mousePosWindow = { 0, 0 };

sf::Vector2f Mouse::mousePosView = { 0.f, 0.f };

sf::Clock Mouse::dClickClock;

sf::Clock Mouse::freezeClock;

sf::Clock Mouse::timeSinceMousePessedClock;

float Mouse::dClickDuration = .5f;

float Mouse::clickDuration = .35f;

float Mouse::freezeTime = 0.f;

float Mouse::zoom = 1.f;

bool Mouse::pressed= false;

bool Mouse::doubleClick = false;

bool Mouse::normalClick = false;

bool Mouse::stillClick = false;

void Mouse::resetAll()
{
	Mouse::doubleClick = false;
	Mouse::normalClick = false;
	Mouse::stillClick = false;
}

void Mouse::updateMousePos(sf::RenderWindow* window)
{
	Mouse::mousePosScreen = sf::Mouse::getPosition();
	Mouse::mousePosWindow = sf::Mouse::getPosition(*window);
	Mouse::mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

void Mouse::update(sf::RenderWindow* window)
{
	Mouse::updateMousePos(window);
	Mouse::resetAll();

	// calc relative pos when mouse is moving
	Mouse::relativePos = (sf::Vector2f(Mouse::mousePosWindow) - sf::Vector2f(Mouse::lastMousePosWindow)) * zoom;
	Mouse::lastMousePosWindow = Mouse::mousePosWindow;
	Mouse::movementSinceLastClick += sf::Vector2f(Mouse::relativePos.x, Mouse::relativePos.y);
	Mouse::movementSinceMousePressed += sf::Vector2f(abs(Mouse::relativePos.x), abs(Mouse::relativePos.y));;
	Mouse::pos = Mouse::mousePosView;

	// update freeze timer
	Mouse::freezeTime -= Mouse::freezeClock.restart().asSeconds();

	if (Mouse::freezeTime > 0)
	{
		return;
	}
		

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (Mouse::pressed == false)
			Mouse::timeSinceMousePessedClock.restart();
		Mouse::pressed = true;
	}
	else
	{
		// if mouse up
		if (Mouse::pressed == true && Mouse::timeSinceMousePessedClock.restart().asSeconds() < Mouse::clickDuration)
		{
			// normal click here
			Mouse::normalClick = true;
			
			//if (Mouse::movementSinceMousePressed == sf::Vector2f())
			if (Helper::VectorLength(Mouse::movementSinceMousePressed) < 10.f)
			{
				// still click here
				Mouse::stillClick = true;
				
				//std::cout << " normal click\n";
				//std::cout << movementSinceLastClick.x << ' ' << movementSinceLastClick.y << '\n';

				float time_between_pressed = Mouse::dClickClock.restart().asSeconds();
				if (time_between_pressed <= Mouse::dClickDuration)
				{
					if (Mouse::movementSinceLastClick == sf::Vector2f())
					{
						//std::cout << " double click true\n";
						Mouse::doubleClick = true;
					}
				}

				if (Mouse::normalClick)
					Mouse::movementSinceLastClick = { 0.f, 0.f };
			}
		}

		Mouse::pressed = false;
		Mouse::movementSinceMousePressed = { 0.f, 0.f };
	}
}

void Mouse::freezeMouse(const float time)
{
	Mouse::resetAll();
	Mouse::freezeTime = time;
}

float Mouse::getZoom()
{
	return zoom;
}