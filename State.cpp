#include "State.h"

State::State(sf::RenderWindow* window, std::vector<State*>* states)
{
	this->window = window;
	this->states = states;
	this->quit = false;

	this->initFont();
}

State::State(const State& s)
{
	this->window = s.window;
	this->states = s.states;

	this->quit = false;
}

State::~State()
{
	delete this->font;
}

sf::Font* State::getFont()
{
	return this->font;
}

void State::initFont()
{
	this->font = nullptr;
}

const bool& State::getQuit() const
{
	return this->quit;
}

void State::endState()
{
	this->quit = true;
}

void State::addState(State* state)
{
	this->states->push_back(state);
}

void State::updateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

void State::handleEvents(sf::Event)
{
}
