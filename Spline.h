#pragma once
#include "State.h"
#include "ArrowTip.h"

class Node;
class Transition;

class Spline
{
protected:
	sf::Vector2f p0;
	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::Vector2f p3;

	sf::Color defaultColor;
	sf::Color highlightColor;

	Transition* transition;

	sf::Text* textRender;
	int visibleCharacters;
	bool textLimit = false;

	int precision;
	float step;

	Node* from;
	Node* to;

	float absDist;
	float offset;
	float offsetScale;
	float minOffset;

	sf::VertexArray vertexArray;
	ArrowTip tip;

public:
	Spline(Transition* transition, Node*, Node*, sf::Text*);
	virtual ~Spline();

	virtual void updateAnchorPoints();
	virtual void updateSplinePoints();

	virtual void udpateTextPos(bool = true);
	virtual void setVertexColor(sf::Color);

	virtual sf::Vector2f getSplinePoint(float x);

	virtual void draw(sf::RenderTarget*);

};

