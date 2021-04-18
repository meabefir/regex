#include "TransitionCreationComp.h"

#include "EditorState.h"
#include "State.h"
#include "Mouse.h"

TransitionCreationComp::TransitionCreationComp(EditorState* editor):
	node(nullptr), editor(editor), line(sf::LinesStrip, 2), color(sf::Color::Blue)
{
	for (int i = 0; i < this->line.getVertexCount(); i++)
	{
		this->line[i].color = this->color;
	}
}

TransitionCreationComp::~TransitionCreationComp()
{

}

void TransitionCreationComp::createTransitionTo(Node* to_node)
{
	this->node->addTransitionToNode(to_node);
	this->node = nullptr;
	this->editor->activateAll();
}

void TransitionCreationComp::update()
{
	if (Mouse::stillClick && !Mouse::doubleClick)
	{
		if (this->editor->getHoverNode() != nullptr)
		{
			if (this->node == nullptr)
			{
				this->node = this->editor->getHoverNode();
				this->editor->deactivateAllBut("TRANSITION_CREATION");
			}
			else
			{
				this->createTransitionTo(this->editor->getHoverNode());
			}
		}
		else
		{
			if (this->node != nullptr)
			{
				NodeCreationComp* cp = dynamic_cast<NodeCreationComp*>(this->editor->getComponents()->operator[]("NODE_CREATION"));
				Node* new_node = cp->attemptNodeCreation();
				if (new_node != nullptr)
				{
					this->createTransitionTo(new_node);
				}
			}
			
			this->node = nullptr;
			this->editor->activateAll();
		}
	}
	else if (Mouse::doubleClick)
	{
		this->node = nullptr;
		this->editor->activateAll();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		this->node = nullptr;
		this->editor->activateAll();
	}
}

void TransitionCreationComp::draw(sf::RenderTarget* target)
{
	if (this->node != nullptr)
	{
		line[0].position = this->node->getShape().getPosition();
		this->line[1].position = Mouse::mousePosView;
		target->draw(this->line);
	}
}
