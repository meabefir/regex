#include "Spline.h"

#include "Node.h"
#include "Mouse.h"
#include "Helper.h"
#include "Transition.h"

Spline::Spline(Transition* transition, Node* from, Node* to, sf::Text* text_render) :
	transition(transition), from(from), to(to), textRender(text_render), visibleCharacters(10),
	offsetScale(10.f), minOffset(150.f)
{
	absDist = Helper::VectorDistance(from->getShape().getPosition(), to->getShape().getPosition());

	// we want fixed offset for self splines
	if (dynamic_cast<SplineSelf*>(this) == nullptr)
		offset = std::max(minOffset, absDist / offsetScale);
	else
		offset = 3000.f;

	precision = 21;
	step = 1.f / precision;

	vertexArray = sf::VertexArray(sf::LinesStrip, precision);
	for (int i = 0; i < vertexArray.getVertexCount(); i++)
		vertexArray[i].color = sf::Color::Black;	

	this->updateAnchorPoints();
}

Spline::~Spline()
{
	
}

void Spline::updateAnchorPoints()
{
	this->p0 = this->from->getShape().getPosition();
	this->p3 = this->to->getShape().getPosition();

	absDist = Helper::VectorLength(p3 - p0);
	if (dynamic_cast<SplineSelf*>(this) == nullptr)
		offset = std::max(minOffset, absDist / offsetScale);

	// calc p1 and p2 based on p0 and p3
	// make new offset vector
	float rotation = -3.141592653f / 2.f;
	sf::Vector2f temp(Helper::GetNormalizedVector(this->p3 - this->p0) * offset);
	sf::Vector2f rotated_temp = Helper::vectorRotatedRad(temp, rotation);
	
	sf::Vector2f thirdVector = (this->p3 - this->p0) / 3.f;

	// p1
	this->p1 = this->p0 + thirdVector;
	// add the rotated ofssets to the original vector
	this->p1 += rotated_temp;

	// p2
	this->p2 = this->p0 + thirdVector * 2.f;
	// add the rotated ofssets to the original vector
	this->p2 += rotated_temp;

	sf::Vector2f p0_to_p1(p1 - p0);
	this->p0 += Helper::GetNormalizedVector(p0_to_p1) * this->from->getRadius();

	sf::Vector2f p3_to_p2(p2 - p3);
	this->p3 += Helper::GetNormalizedVector(p3_to_p2) * this->from->getRadius();

	// change p0 and p3 and change p1 and p2 on the nodes
	//this->p0 += Helper::vectorRotatedRad(this->p1 - this->p0, 3.1415926f);
	//this->p3 += Helper::vectorRotatedRad(this->p2 - this->p3, 3.1415926f);
	sf::Vector2f p0_cpy(p0);
	sf::Vector2f p3_cpy(p3);

	this->p0 += p0 - p1;
	this->p3 += p3 - p2;

	this->p1 = p0_cpy;
	this->p2 = p3_cpy;

	this->updateSplinePoints();
}

void Spline::updateSplinePoints()
{
	for (int i = 0; i < precision-1; i++)
	{
		float t = this->step * i;
		vertexArray[i].position = this->getSplinePoint(t);
	}
	vertexArray[precision - 1].position = this->p2;

	this->tip.update(vertexArray[precision - 2].position, vertexArray[precision - 1].position);
}

sf::Vector2f Spline::getSplinePoint(float t)
{
	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.f * tt - t;
	float q2 = 3.f * ttt - 5.f * tt + 2.f;
	float q3 = -3.f * ttt + 4.f * tt + t;
	float q4 = ttt - tt;

	float tx = .5f * (this->p0.x * q1 + this->p1.x * q2 + this->p2.x * q3 + this->p3.x * q4);
	float ty = .5f * (this->p0.y * q1 + this->p1.y * q2 + this->p2.y * q3 + this->p3.y * q4);

	return sf::Vector2f(tx, ty);
}

void Spline::udpateTextPos(bool applyLimit)
{
	std::string str = this->transition->text;
	if (applyLimit)
	{
		textLimit = true;
		this->visibleCharacters = std::min(10, std::max(1, (int)Helper::VectorDistance(this->from->getShape().getPosition(),
			this->to->getShape().getPosition()) / 50));
		if (str.size() <= this->visibleCharacters)
		{
			this->textRender->setString(str);
		}
		else
		{
			this->textRender->setString(str.substr(0, this->visibleCharacters) + "...");
		}
	}
	else
	{
		textLimit = false;
		this->textRender->setString(str);
	}

	sf::Vector2f mid_pos = this->vertexArray[this->precision / 2].position;
	this->transition->setMidPos(mid_pos);

	mid_pos += Helper::vectorRotatedRad(
		Helper::GetNormalizedVector(this->vertexArray[this->precision / 2 + 1].position -
									this->vertexArray[this->precision / 2].position),
							-3.141592653f / 2.f) * 15.f;
	mid_pos += (this->vertexArray[this->precision / 2 + 1].position -
		this->vertexArray[this->precision / 2].position) / 2.f;

	sf::Vector2f text_render_size(this->textRender->getGlobalBounds().width, this->textRender->getGlobalBounds().height);
	//this->textRender->setPosition(mid_pos.x - text_render_size.x / 2.f, mid_pos.y - text_render_size.y);
	this->textRender->setPosition(mid_pos.x - text_render_size.x / 2.f, mid_pos.y - 15.f);
}

void Spline::setVertexColor(sf::Color c)
{
	this->tip.setVertexColor(c);
	for (int i = 0; i < this->vertexArray.getVertexCount(); i++)
		this->vertexArray[i].color = c;
}

void Spline::draw(sf::RenderTarget* target)
{	
	// show or hide full text
	if (Helper::mouseInBox(this->textRender->getGlobalBounds()))
	{
		this->udpateTextPos(false);
		this->setVertexColor(sf::Color::Red);
	}
	else if (this->textLimit == false)
	{
		this->udpateTextPos(true);
		this->setVertexColor(sf::Color::Black);
	}

	// check if pos update is needed
	if (this->from->isActive() || this->to->isActive())
	{
		this->updateAnchorPoints();
		this->udpateTextPos();
	}

	// draw
	target->draw(this->vertexArray);
	target->draw(*this->textRender);
	// draw tip
	this->tip.draw(target);
	//////////////////////////////////////////////////////////////////

#if 0
	// thickness test
	std::vector<std::pair<int, int>> dir = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	for (auto& per : dir)
	{
		sf::VertexArray tt(this->vertexArray);
		for (int i = 0; i < tt.getVertexCount(); i++)
			tt[i].position += sf::Vector2f(per.first, per.second);
		target->draw(tt);
	}
#endif

#if 0
	// test
	sf::VertexArray t(sf::LinesStrip, 4);
	t[0].position = this->p0;
	t[1].position = this->p1;
	t[2].position = this->p2;
	t[3].position = this->p3;

	for (int i = 0; i < 4; i++)
		t[i].color = sf::Color::Red;
	target->draw(t);


	// temp
	sf::Vector2f thirdVector = (this->p3 - this->p0) / 3.f;

	sf::VertexArray test(sf::LinesStrip, 2);
	test[0].position = this->from->getShape().getPosition();
	test[1].position = test[0].position + thirdVector;

	test[0].color = sf::Color::Blue;
	test[1].color = sf::Color::Blue;

	target->draw(test);

	float rotation = 3.141592653f / 2.f;

	// calc p1 and p2 based on p0 and p3
	// make new offset vector
	sf::Vector2f temp(Helper::GetNormalizedVector(this->p3 - this->p0) * offset);
	sf::Vector2f rotated_temp = sf::Vector2f(cos(rotation) * temp.x - sin(rotation) * temp.y,
											sin(rotation) * temp.x + cos(rotation) * temp.y);
	//sf::Vector2f rotated_temp = sf::Vector2f(temp.y, -temp.x);
	
	sf::VertexArray test2(sf::LinesStrip, 2);
	test2[0].position = test[1].position;
	test2[1].position = test2[0].position + rotated_temp;

	test2[0].color = sf::Color::Red;
	test2[1].color = sf::Color::Red;

	target->draw(test2);

	sf::VertexArray test3(sf::LinesStrip, 2);
	test3[0].position = test[1].position;
	test3[1].position = test3[0].position + temp;

	test3[0].color = sf::Color::Green;
	test3[1].color = sf::Color::Green;

	target->draw(test3);
#endif
}
