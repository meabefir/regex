#pragma once
#include "SFML\Graphics.hpp"

class Helper
{
public:
	static float VectorDistance(const sf::Vector2f& v1, const sf::Vector2f& v2);
	static float VectorDistance(const sf::Vector2i& v1, const sf::Vector2f& v2);
	static float VectorLength(const sf::Vector2f&);
	static sf::Vector2f GetNormalizedVector(const sf::Vector2f&);
	static sf::Vector2f vectorRotatedRad(sf::Vector2f vec, float rad);
	static bool mouseInBox(sf::FloatRect);
};

