#include "Node.h"
#include "EditorState.h"
#include "Transition.h"
#include "AppData.h"
#include "NodeEditorState.h"

sf::Color Node::defaultNodeColor = sf::Color(255, 183, 3);
sf::Color Node::startNodeColor = sf::Color(214, 40, 40);
sf::Color Node::finalNodeColor = sf::Color(58, 12, 163);
sf::Color Node::startFinalNodeColor = sf::Color(114, 9, 183);
sf::Color Node::activeColor = sf::Color(33, 158, 188);

Node::Node(EditorState* editor, sf::Font* font, sf::Vector2f pos,
			std::string name, const float radius, const float outlineThick):
	editor(editor), radius(radius), id(name)
{
	this->initVariables();

	// shape stuff
	this->shape.setPosition(pos);
	this->shape.setRadius(radius - outlineThick);
	this->shape.setOutlineColor(sf::Color::Black);
	this->shape.setFillColor(Node::defaultNodeColor);
	this->shape.setOutlineThickness(outlineThick);
	this->shape.setOrigin(sf::Vector2f(radius - outlineThick, radius - outlineThick));

	// other stuff
	this->name = name;
	// font and text
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(this->name);
	this->text.setCharacterSize(40);
	this->text.setFillColor(sf::Color::Black);
	
	this->text.setOrigin(sf::Vector2f(this->text.getGlobalBounds().width / 2.f,
									  this->text.getGlobalBounds().height));
	this->text.setPosition(this->shape.getPosition().x,
						   this->shape.getPosition().y);
}

Node::Node(EditorState* editor, sf::Font* font, sf::Vector2f pos, std::string id, std::string name,
		const float radius, const float outlineThick):
	editor(editor), radius(radius), id(id), name(name)
{
	this->initVariables();

	// shape stuff
	this->shape.setPosition(pos);
	this->shape.setRadius(radius - outlineThick);
	this->shape.setOutlineColor(sf::Color::Black);
	this->shape.setFillColor(Node::defaultNodeColor);
	this->shape.setOutlineThickness(outlineThick);
	this->shape.setOrigin(sf::Vector2f(radius - outlineThick, radius - outlineThick));

	// other stuff
	this->name = name;
	// font and text
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(this->name);
	this->text.setCharacterSize(40);
	this->text.setFillColor(sf::Color::Black);
	
	this->text.setOrigin(sf::Vector2f(this->text.getGlobalBounds().width / 2.f,
		this->text.getGlobalBounds().height));
	this->text.setPosition(this->shape.getPosition().x,
		this->shape.getPosition().y);
}

Node::~Node()
{
	std::cout << " Node " << this->name << " removed\n";
	for (auto& trans : transitions)
	{
		delete trans;
		trans = nullptr;
	}
}

void Node::initVariables()
{
	this->active = false;
	this->mouseOver = false;
	this->mouseClicked = false;
	this->isStartingNode = false;
	this->isFinalNode = false;
}

const bool Node::getMouseOver()
{
	return this->mouseOver;
}

const sf::CircleShape& Node::getShape() const
{
	return this->shape;
}

EditorState* Node::getEditor()
{
	return this->editor;
}

bool Node::isActive()
{
	return this->active;
}

float Node::getRadius()
{
	return this->radius;
}

std::string Node::getName()
{
	return this->name;
}

std::string Node::getId()
{
	return this->id;
}

void Node::setName(std::string s)
{
	this->name = s;
	this->text.setString(s);
	this->text.setOrigin(sf::Vector2f(this->text.getGlobalBounds().width / 2.f,
		this->text.getGlobalBounds().height));
}

std::vector<Transition*>* Node::getTransitions()
{
	return &this->transitions;
}

bool Node::isStartNode()
{
	return this->isStartingNode;
}

bool Node::isFinalNodeFunc()
{
	return this->isFinalNode;
}

const float Node::getDistToVector(const sf::Vector2f& vec)
{
	return sqrt(pow(this->shape.getPosition().x - vec.x, 2) + pow(this->shape.getPosition().y - vec.y, 2));
}

void Node::toggleFinal()
{
	this->isFinalNode = !this->isFinalNode;
	if (this->isFinalNode)
	{
		if (this->isStartingNode)
			this->shape.setFillColor(Node::startFinalNodeColor);
		else
			this->shape.setFillColor(Node::finalNodeColor);
	}
	else
	{
		if (this->isStartingNode)
			this->shape.setFillColor(Node::startNodeColor);
		else
			this->shape.setFillColor(Node::defaultNodeColor);
	}
}

void Node::toggleStart(bool self)
{
	if (self == true)
	{
		this->isStartingNode = false;
		this->shape.setFillColor(this->getCurrentColor());
		return;
	}

	Node* startNode = this->editor->getStartNode();
	if (startNode != nullptr)
	{
		if (startNode != this)
		{
			startNode->toggleStart(true);
			this->isStartingNode = true;
		}
		else
		{
			this->isStartingNode = false;
		}
	}
	else if (startNode == nullptr)
	{
		this->isStartingNode = true;
	}
	this->shape.setFillColor(this->getCurrentColor());
}

sf::Color Node::getCurrentColor()
{
	if (this->isFinalNode && this->isStartingNode)
		return Node::startFinalNodeColor;
	if (this->isFinalNode)
		return Node::finalNodeColor;
	if (this->isStartingNode)
		return Node::startNodeColor;
	return Node::defaultNodeColor;
}

void Node::setText(std::string str)
{
	this->text.setString(str);
	this->text.setOrigin(this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().height);
}

void Node::updateMouseInput(const sf::Vector2f& mouse_pos)
{
	const float dist = this->getDistToVector(mouse_pos);
	if (dist <= this->shape.getRadius())
	{
		this->editor->hoveringNode = this;
		this->mouseOver = true;
	}
	else
		this->mouseOver = false;

	if (this->mouseOver && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		this->mouseClicked = true;
	else
		this->mouseClicked = false;

	if (this->mouseOver)
	{
		if (Mouse::doubleClick)
		{
			// edit name
			this->openEditMode();
		}
	}
	//std::cout << this->mouseOver << ' ' << this->mouseClicked << '\r';
}

void Node::openEditMode()
{
	//State* new_trans_editor = new TransitionEditorState(dynamic_cast<State&>(*this->editor));
	State* new_node_editor = new NodeEditorState(*this->editor, this);
	this->editor->addState(new_node_editor);
}


void Node::resetTransitionCreation()
{
	for (auto& tran : this->transitions)
	{
		if (tran->node2 == NULL)
		{
			auto it = std::find(this->transitions.begin(), this->transitions.end(), tran);
			if (it != this->transitions.end())
			{
				this->transitions.erase(it);
				delete tran;
				tran = NULL;
			}
		}
	}
}

void Node::addTransitionToNode(Node* node)
{
	// check if there are other transitions to that node
	for (auto& trans : this->transitions)
	{
		if (trans->node2 == node)
		{
			// edit that transition, return for now
			trans->openEditMode();
			return;
		}
	}
	this->transitions.push_back(new Transition(this, node));
}

void Node::addTransitionToNode(Node* node, std::string text)
{
	// check if there are other transitions to that node
	for (auto& trans : this->transitions)
	{
		if (trans->node2 == node)
		{
			// edit that transition, return for now
			trans->openEditMode();
			return;
		}
	}
	this->transitions.push_back(new Transition(this, node, text));
}

void Node::removeTransition(Transition* trans)
{
	auto it = std::find(this->transitions.begin(), this->transitions.end(), trans);
	if (it != this->transitions.end())
	{
		this->transitions.erase(it);
		delete trans;
		trans = nullptr;
	}
}

void Node::checkForInteraction()
{
	if (this->mouseOver && this->mouseClicked && this->editor->getInteractingWith() != this)
	{
		this->editor->interactionStarted(this);

		this->editor->setInteractigWith(this);
		this->active = true;
		this->shape.setFillColor(this->activeColor);
	}
	else if (this->active && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		this->editor->setInteractigWith(nullptr);
		this->active = false;
		this->shape.setFillColor(this->getCurrentColor());
	}
}

void Node::update()
{
	// only update it if we are interacting with it
	if (this->editor->interactingWithNode != nullptr && this->editor->interactingWithNode != this)
		return;

	this->updateMouseInput(Mouse::mousePosView);

	// check if mouse is interacting with this node
	this->checkForInteraction();

	// if interacting with this, follow mouse
	if (this->editor->interactingWithNode == this)
	{
		this->shape.move(Mouse::relativePos.x, Mouse::relativePos.y);
		this->text.setPosition(this->shape.getPosition());
	}

	// update transitions
	for (auto& tran : this->transitions)
	{
		tran->update();
	}
}

void Node::drawTransitions(sf::RenderTarget* target)
{
	for (auto& tran : this->transitions)
	{
		tran->draw(target);
	}
}

void Node::draw(sf::RenderTarget* target)
{
	target->draw(this->shape);
	target->draw(this->text);
}
