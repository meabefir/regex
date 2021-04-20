#include "EditorState.h"
#include <fstream>
#include "NodeCreationComp.h"
#include "TransitionCreationComp.h"
#include "AppData.h"
#include "MenuState.h"
#include "Transition.h"
#include "NodeEditorState.h"
#include "Automata.h"

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
	this->components["TRANSITION_CREATION"] = new TransitionCreationComp(this);
}

void EditorState::updateInput(const float& dt)
{

}

void EditorState::saveLevel(std::string name)
{
	std::ofstream g("Levels/"+name + ".txt");

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

void EditorState::draw(sf::RenderTarget* target)
{
	this->drawNodes(target);

	// draw my components
	for (auto& per : this->components)
	{
		if (this->sleepingComponents.find(per.first) == this->sleepingComponents.end())
		{
			per.second->draw(target);
		}
	}
}

std::unordered_map<std::string, Component*>* EditorState::getComponents()
{
	return &this->components;
}

void EditorState::clearNodes()
{
	/*for (auto& cp : this->components)
	{
		NodeCreationComp* ncp = dynamic_cast<NodeCreationComp*>(cp);
		if (ncp != nullptr)
		{
			ncp->resetId();
			break;
		}
	}*/

	NodeCreationComp* ncp = dynamic_cast<NodeCreationComp*>(this->components["NODE_CREATION"]);
	ncp->resetId();

	for (auto& node : this->nodes)
		delete node;
	while (this->nodes.size())
		this->nodes.pop_back();
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
	return;
	if (e.type == sf::Event::MouseMoved)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		{
			sf::View curr_view = this->window->getView();
			curr_view.move(-Mouse::relativePos);
			this->window->setView(curr_view);
		}
	}
}

void EditorState::buildAutomata()
{
	Automata au(&this->nodes);

	std::cout << au.getRegex() << '\n';
}
