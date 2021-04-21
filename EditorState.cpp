#include "EditorState.h"
#include <fstream>
#include "NodeCreationComp.h"
#include "TransitionCreationComp.h"
#include "AppData.h"
#include "MenuState.h"
#include "Transition.h"
#include "NodeEditorState.h"
#include "Automata.h"
#include <direct.h>
#include "NodeRemovalComp.h"

EditorState::EditorState(sf::RenderWindow* window, std::vector<State*>* states):
	State(window, states), radius(50), interactingWithNode(nullptr), hoveringNode(nullptr)
{
	// init font
	this->initFont();

	// init components
	this->initComponents();
}

EditorState::~EditorState()
{
	for (auto node : this->nodes)
		delete node;
	for (auto& per : this->components)
		delete per.second;
}

void EditorState::initFont()
{
	this->font = new sf::Font();
	this->font->loadFromFile(AppData::fontPath);
}

void EditorState::initComponents()
{
	this->components["NODE_CREATION"] = new NodeCreationComp(this, &this->nodes);
	this->components["NODE_REMOVAL"] = new NodeRemovalComp(this);
	this->components["TRANSITION_CREATION"] = new TransitionCreationComp(this);

	this->deactivateComponent("NODE_REMOVAL");
}

void EditorState::updateInput(const float& dt)
{

}

void EditorState::saveLevel(std::string name)
{
	std::ofstream g("Levels/"+name + ".txt");

	if (!g.is_open())
	{
		_mkdir("./Levels");
		g.open("Levels/" + name + ".txt");
	}

	// write nr of nodes
	g << this->nodes.size() << '\n';
	// write pos.x pos.y id and name for all nodes
	int nr_final = 0;
	for (auto& node : this->nodes)
	{
		// increment nr of final nodes
		if (node->isFinalNodeFunc())
			nr_final++;
		g << node->getShape().getPosition().x << ' ' << node->getShape().getPosition().y << ' ' <<
			node->getId() << ' ' << node->getName() << '\n';
	}

	// calc nr of transitions and memorate them in auxiliar vector
	std::vector<Transition*> transitions;
	for (auto& node : this->nodes)
	{
		for (auto& trans : *node->getTransitions())
		{
			transitions.push_back(trans);
		}
	}
	// write nr of transitions
	g << transitions.size() << '\n';

	// write transitions -> from to text
	for (auto& trans : transitions)
	{
		g << trans->getNode1()->getId() << ' ' << trans->getNode2()->getId() << ' ' << trans->getText() << '\n';
	}

	// write start node id
	for (auto& node : this->nodes)
	{
		if (node->isStartNode())
		{
			g << node->getId() << '\n';
			break;
		}
	}

	// write nr of final nodes
	g << nr_final << '\n';
	
	// write final nodes ids
	for (auto& node : this->nodes)
	{
		if (node->isFinalNodeFunc())
		{
			g << node->getId() << ' ';
		}
	}

	g.close();
}

void EditorState::reset()
{
	this->hoveringNode = nullptr;
}

void EditorState::update(const float& dt)
{
	this->reset();

	this->updateInput(dt);

	// update nodes from last to first
	for (int i = this->nodes.size()-1; i >= 0; --i)
	{
		this->nodes[i]->update();
	}

	// update my components
	for (auto& per : this->components)
	{
		if (this->sleepingComponents.find(per.first) == this->sleepingComponents.end())
		{
			per.second->update();
		}
	}
}

Node* EditorState::getHoverNode()
{
	return this->hoveringNode;
}

void EditorState::loadLevel(std::string name)
{
	this->clearNodes();

	std::ifstream f("Levels/" + name + ".txt");

	int nrNodes;
	f >> nrNodes;
	std::cout << nrNodes;

	// create nodes
	for (int i = 0; i < nrNodes; i++)
	{
		int pos_x, pos_y;
		std::string id, name;
		f >> pos_x >> pos_y >> id >> name;

		this->nodes.push_back(new Node(this, this->font, sf::Vector2f(pos_x, pos_y), id, name));
	}
	
	int nrTransitions;
	f >> nrTransitions;
	// create transitions
	for (int i = 0; i < nrTransitions; i++)
	{
		std::string id1, id2, text;
		f >> id1 >> id2 >> text;
		Node* node1 = this->getNodeFromId(id1);
		Node* node2 = this->getNodeFromId(id2);

		node1->addTransitionToNode(node2, text);
	}

	// make start node
	std::string start_id;
	f >> start_id;
	Node* start_node = this->getNodeFromId(start_id);
	start_node->toggleStart();

	// make final nodes
	int nr_finals;
	f >> nr_finals;
	for (int i = 0; i < nr_finals; i++)
	{
		std::string final_id;
		f >> final_id;
		Node* final_node = this->getNodeFromId(final_id);
		final_node->toggleFinal();
	}

	f.close();
	// get max id
	int max_node = 0;
	for (auto& np : this->nodes)
	{
		if (std::stoi(np->getId()) > max_node)
			max_node = std::stoi(np->getId());
	}
	// set the max id
	NodeCreationComp* ncp = dynamic_cast<NodeCreationComp*>(this->components["NODE_CREATION"]);
	ncp->setId(max_node+1);
}

void EditorState::draw(sf::RenderTarget* target, sf::View* UIView)
{
	this->drawNodes(target);

	// draw my components
	for (auto& per : this->components)
	{
		if (this->sleepingComponents.find(per.first) == this->sleepingComponents.end())
		{
			per.second->draw(target, UIView);
		}
	}
}

std::unordered_map<std::string, Component*>* EditorState::getComponents()
{
	return &this->components;
}

void EditorState::clearNodes()
{
	NodeCreationComp* ncp = dynamic_cast<NodeCreationComp*>(this->components["NODE_CREATION"]);
	ncp->resetId();

	for (auto& node : this->nodes)
		delete node;
	while (this->nodes.size())
		this->nodes.pop_back();

	sf::View window_view(this->window->getView());
	window_view.reset(sf::FloatRect(0, 0, this->window->getViewport(window_view).width,
										this->window->getViewport(window_view).height));
	this->window->setView(window_view);
}

void EditorState::removeNode(Node* n)
{
	bool is_starting_node = false;
	if (this->getStartNode() == n)
		is_starting_node = true;

	auto node_it = std::find(nodes.begin(), nodes.end(), n);
	if (node_it != nodes.end())
	{	
		if (this->interactingWithNode == n)
			this->setInteractigWith(nullptr);
		this->nodes.erase(node_it);
		delete n;
	}

	// change start node if needed
	if (is_starting_node)
		if (this->nodes.size())
			this->nodes[0]->toggleStart();

	// REMOVE TRANSITIONS !!!!!!!!!!!!!!!!!!!
	for (auto& node : this->nodes)
	{
		int i = 0;
		for (auto& trans : *node->getTransitions())
		{
			if (trans->getNode2() == n)
			{
				node->removeTransition(trans);
			}
			i++;
		}
	}
}

Node* EditorState::getNodeFromId(std::string id)
{
	for (auto& np : this->nodes)
		if (np->getId() == id)
			return np;
}

void EditorState::drawNodes(sf::RenderTarget* target)
{
	for (auto node : this->nodes)
	{
		node->drawTransitions(target);
	}
	for (auto node : this->nodes)
	{
		node->draw(target);
	}
}

void EditorState::interactionStarted(Node* node)
{
	// put node on top
	auto it = std::find(this->nodes.begin(), this->nodes.end(), node);
	if (it == this->nodes.end())
		return;

	this->nodes.erase(it);
	this->nodes.push_back(node);
}

void EditorState::shuffleNodes()
{
	int max_size = this->nodes.size();
	for (int i = 0; i < max_size; i++)
	{
		int poz1 = rand() % max_size;
		int poz2 = rand() % max_size;

		Node* aux = nodes[poz1];
		nodes[poz1] = nodes[poz2];
		nodes[poz2] = aux;
	}
	std::cout << " Shuffled nodes\n";
}

void EditorState::deactivateAllBut(std::string name)
{
	for (auto& comp : this->components)
	{
		if (comp.first != name)
		{
			this->sleepingComponents[comp.first] = 1;
		}
	}
}

void EditorState::activateAllBut(std::string name)
{
	for (auto& comp : this->components)
	{
		if (comp.first != name)
		{
			this->sleepingComponents.erase(comp.first);
		}
	}
}

void EditorState::deactivateComponent(std::string comp)
{
	if (this->components.find(comp) != this->components.end())
	{
		this->sleepingComponents[comp] = 1;
	}
}

void EditorState::activateComponent(std::string comp)
{
	if (this->components.find(comp) != this->components.end())
	{
		this->sleepingComponents.erase(comp);
	}
}

void EditorState::activateAll()
{
	for (auto& comp : this->components)
	{
		if (this->sleepingComponents.find(comp.first) != this->sleepingComponents.end())
			this->sleepingComponents.erase(comp.first);
	}
}

Node* EditorState::getStartNode()
{
	for (Node* np :this->nodes)
		if (np->isStartNode())
			return np;
	return nullptr;
}

void EditorState::setInteractigWith(Node* n)
{
	//std::cout << n << '\n';
	this->interactingWithNode = n;
	if (n == nullptr)
		this->deactivateComponent("NODE_REMOVAL");
	else
		this->activateComponent("NODE_REMOVAL");
}

Node* EditorState::getInteractingWith()
{
	return this->interactingWithNode;
}

void EditorState::handleEvents(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Key::Escape)
			this->states->push_back(new MenuState(this->window, this->states));
	}
	if (e.type == sf::Event::KeyReleased)
		if (e.key.code == sf::Keyboard::Key::S)
			this->shuffleNodes();

	// uncomment this after you make ui elemets unaffected by panning
	if (e.type == sf::Event::MouseMoved)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		{
			sf::View curr_view = this->window->getView();
			curr_view.move(-Mouse::relativePos);
			this->window->setView(curr_view);
		}
	}

	// if scroll
	if (e.type == sf::Event::MouseWheelMoved)
	{
		// zoom out
		if (e.mouseWheel.delta == -1 && Mouse::zoom <= 2.f)
		{
			sf::View curr_view = this->window->getView();
			curr_view.zoom(1.1f);
			Mouse::zoom = Mouse::zoom * 1.1f;
			this->window->setView(curr_view);
		}
		else if (e.mouseWheel.delta == 1 && Mouse::zoom >= .5f) // zoom in
		{
			sf::View curr_view = this->window->getView();
			curr_view.zoom(.9f);
			Mouse::zoom = Mouse::zoom * .9f;
			this->window->setView(curr_view);
		}
	}
}

void EditorState::buildAutomata()
{
	Automata au(&this->nodes);

	std::cout << au.getRegex() << '\n';
}
