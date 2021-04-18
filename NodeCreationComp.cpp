#include "NodeCreationComp.h"
#include "AppData.h"

NodeCreationComp::NodeCreationComp(EditorState* editor, std::vector<Node*>* nodes):
	editor(editor), nodes(nodes), radius(AppData::nodeRadius), nodeId(0)
{

}

NodeCreationComp::~NodeCreationComp()
{
}

void NodeCreationComp::createNode(sf::Vector2f pos)
{
	this->nodes->push_back(new Node(this->editor, this->editor->getFont(), pos,
		std::to_string(this->nodeId++), sf::Color(0, 255, 0, 255)));
}

float NodeCreationComp::getClosestNodeDistance()
{
	if (this->nodes->size() == 0)
		return this->radius * 2;
	float dist = sqrt(pow((*this->nodes)[0]->getShape().getPosition().x - Mouse::mousePosView.x, 2) +
		pow((*this->nodes)[0]->getShape().getPosition().y - Mouse::mousePosView.y, 2));

	for (int i = 1; i < this->nodes->size(); i++)
	{
		const Node* curr_node = (*this->nodes)[i];
		const float curr_dist = sqrt(pow(curr_node->getShape().getPosition().x - Mouse::mousePosView.x, 2) +
			pow(curr_node->getShape().getPosition().y - Mouse::mousePosView.y, 2));

		if (curr_dist < dist)
		{
			dist = curr_dist;
		}
	}

	return dist;
}

Node* NodeCreationComp::attemptNodeCreation()
{
	if (this->getClosestNodeDistance() >= this->radius * 2)
	{
		Node* new_node = new Node(this->editor, this->editor->getFont(), Mouse::mousePosView,
			std::to_string(this->nodeId++), sf::Color(0, 255, 0, 255));

		this->nodes->push_back(new_node);
		return new_node;
	}

	return nullptr;
}

void NodeCreationComp::update()
{
	if (Mouse::stillClick)
	{
		if (this->getClosestNodeDistance() >= this->radius * 2)
		{
			this->createNode(Mouse::mousePosView);
		}
	}
}
