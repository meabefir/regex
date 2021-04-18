#pragma once
#include "Spline.h"
class SplineSelf:
	public Spline
{
private:

public:
	SplineSelf(Transition* transition, Node*, Node*, sf::Text*);

	void updateAnchorPoints();
};

