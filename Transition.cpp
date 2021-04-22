#include "Transition.h"
#include "Node.h"
#include "Mouse.h"
#include "Helper.h"
#include "EditorState.h"
#include "TransitionEditorState.h"
#include "TransitionEditorState.h"

Transition::Transition(Node* node1, Node* node2, sf::Color color):
	node1(node1), node2(node2), color(color),
	editor(node1->getEditor()),
	deleteVisible(false), deleteButton(10),
	spline(node1 != node2 ? new Spline(this, node1, node2, &textRender) : new SplineSelf(this, node1, node2, &textRender)),
	text("")
{
	deleteButton.setFillColor(sf::Color(255, 0, 0, 150));
	deleteButton.setOrigin(sf::Vector2f(10.f, 10.f));

	textRender.setFont(*this->editor->getFont());
	textRender.setCharacterSize(35.f);
	textRender.setFillColor(sf::Color(233, 196, 106));


	std::cout << " new transition between " << node1 << ' ' << node2 << '\n';
	///////
	this->openEditMode();
}

Transition::Transition(Node* node1, Node* node2, std::string text, sf::Color color):
	node1(node1), node2(node2), color(color),
	editor(node1->getEditor()),
	deleteVisible(false), deleteButton(10),
	spline(node1 != node2 ? new Spline(this, node1, node2, &textRender) : new SplineSelf(this, node1, node2, &textRender)),
	text(text)
{
	deleteButton.setFillColor(sf::Color(255, 0, 0, 150));
	deleteButton.setOrigin(sf::Vector2f(10.f, 10.f));

	textRender.setFont(*this->editor->getFont());
	textRender.setCharacterSize(35.f);
	textRender.setString(text);
	textRender.setFillColor(sf::Color(233, 196, 106));


	std::cout << " new transition between " << node1 << ' ' << node2 << '\n';
}

Transition::~Transition()
{
	std::cout << " deleting transition\n";
	delete this->spline;
}

void Transition::setMidPos(sf::Vector2f v)
{
	this->midPos = v;
}

Node* Transition::getNode1()
{
	return this->node1;
}

Node* Transition::getNode2()
{
	return this->node2;
}

std::string Transition::getText()
{
	return this->text;
}

void Transition::update()
{
	// check for deletion
	float dist_to_middle = Helper::VectorDistance(this->midPos, Mouse::mousePosView);
	if (dist_to_middle < 50.f)
	{
		this->deleteVisible = true;

		if (dist_to_middle < 10 && Mouse::stillClick)
		{
			Mouse::freezeMouse(.1f);
			this->node1->removeTransition(this);
			return;
		}
	}
	else
	{
		this->deleteVisible = false;
		//this->node1->getEditor()->activateAll();
	}
}

void Transition::draw(sf::RenderTarget* target)
{
	this->spline->draw(target);

	return;

	// draw deletion button
	if (this->deleteVisible)
	{
		this->deleteButton.setPosition(this->midPos);
		target->draw(this->deleteButton);
	}
}

void Transition::setText(std::string s)
{
	this->text = s;
	this->textRender.setString(s);
	this->spline->udpateTextPos();
}

void Transition::openEditMode()
{
	//State* new_trans_editor = new TransitionEditorState(dynamic_cast<State&>(*this->editor));
	State* new_trans_editor = new TransitionEditorState(*this->editor, this);
	this->editor->addState(new_trans_editor);
}
