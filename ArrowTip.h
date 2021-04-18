#pragma once
#include "State.h"

class ArrowTip
{
private:
	sf::Vector2f p1;
	sf::Vector2f p2;

	sf::VertexArray vertexArray;

	float size;

public:
	ArrowTip(float size = 20.f);
	~ArrowTip();

	void update(sf::Vector2f, sf::Vector2f);
	void draw(sf::RenderTarget*);
};

