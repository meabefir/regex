#include "ArrowTip.h"
#include "Helper.h"

ArrowTip::ArrowTip(float size) :
	vertexArray(sf::LinesStrip, 3), size(size)
{
	for (int i = 0; i < 3; i++)
		vertexArray[i].color = sf::Color::Black;
}

ArrowTip::~ArrowTip()
{

}

void ArrowTip::setVertexColor(sf::Color c)
{
	for (int i = 0; i < this->vertexArray.getVertexCount(); i++)
		this->vertexArray[i].color = c;
}

void ArrowTip::update(sf::Vector2f v1, sf::Vector2f v2)
{
	p1 = v1;
	p2 = v2;

	sf::Vector2f p2_to_p1(Helper::GetNormalizedVector(p1 - p2) * size);
	
	float rotation = 3.141592653f / 7.f;

	vertexArray[0].position = p2 + Helper::vectorRotatedRad(p2_to_p1, rotation);
	vertexArray[1].position = p2;
	vertexArray[2].position = p2 + Helper::vectorRotatedRad(p2_to_p1, -rotation);

}

void ArrowTip::draw(sf::RenderTarget* target)
{
	target->draw(vertexArray);
}
