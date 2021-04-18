#include "SplineSelf.h"
#include "Helper.h"
#include <math.h>

SplineSelf::SplineSelf(Transition* transition, Node* from, Node* to, sf::Text* text):
	Spline(transition, from, to, text)
{
	this->updateAnchorPoints();
}

void SplineSelf::updateAnchorPoints()
{
	float first_angle = 3.141592653f / 6.f;
	float second_angle = 3.141592653f / 4.f;

	this->p0 = this->from->getShape().getPosition() + Helper::vectorRotatedRad(sf::Vector2f(0.f, 1.f), -first_angle) * offset;
	this->p3 = this->from->getShape().getPosition() + Helper::vectorRotatedRad(sf::Vector2f(0.f, 1.f), first_angle) * offset;

	this->p1 = this->from->getShape().getPosition() + Helper::vectorRotatedRad(sf::Vector2f(0.f, -1.f), -second_angle) * this->from->getRadius();
	this->p2 = this->from->getShape().getPosition() + Helper::vectorRotatedRad(sf::Vector2f(0.f, -1.f), second_angle) * this->from->getRadius();

	this->updateSplinePoints();
}
