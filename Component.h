#pragma once
#include "State.h"
class Component
{
public:
	virtual void update() = 0;
	virtual void draw(sf::RenderTarget*, sf::View* UIView = nullptr);
};

