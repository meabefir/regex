#include "NodeRemovalComp.h"
#include "Helper.h"
#include "Mouse.h"

NodeRemovalComp::NodeRemovalComp(EditorState* editor):
	editor(editor)
{
	shape.setRadius(50.f);
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(50.f, 50.f);
	shape.setPosition(50.f, 50.f);
}

NodeRemovalComp::~NodeRemovalComp()
{
}

void NodeRemovalComp::update()
{
	if (Helper::VectorDistance(Mouse::mousePosWindow, this->shape.getPosition()) <= this->shape.getRadius())
	{
		this->editor->removeNode(this->editor->getInteractingWith());
	}
}

void NodeRemovalComp::draw(sf::RenderTarget* target, sf::View* UIView)
{
	// save the window view
	sf::View default_view = target->getView();
	// set the ui view
	target->setView(*UIView);

	target->draw(this->shape);

	// set back the default view
	target->setView(default_view);
}
