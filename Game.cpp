#include "Game.h"
#include "EditorState.h"

void Game::initWindow()
{
	//sf::VideoMode window_bounds = sf::VideoMode::getDesktopMode();
	sf::VideoMode window_bounds = sf::VideoMode::getFullscreenModes()[0];
	window_bounds.width = 1280;
	window_bounds.height = 720;
	std::string title = "RegeX";
	
	this->window = new sf::RenderWindow(window_bounds, title, sf::Style::Close | sf::Style::Titlebar);
	this->view = new sf::View(sf::Vector2f(window_bounds.width, window_bounds.height) / 2.f,
						sf::Vector2f(window_bounds.width, window_bounds.height));
	this->UIView = new sf::View(sf::Vector2f(window_bounds.width, window_bounds.height) / 2.f,
		sf::Vector2f(window_bounds.width, window_bounds.height));
	this->window->setView(*this->view);
	//this->window->setFramerateLimit(120);
}

void Game::initStates()
{
	this->states.push_back(new EditorState(this->window, &this->states));
}

Game::Game()
{
	this->initWindow();
	this->initStates();
}

Game::~Game()
{
	delete this->window;
	for (auto& st : this->states)
	{
		delete st;
		st = nullptr;
	}
}

void Game::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::engineUpdate()
{
	this->updateDt();
	Mouse::update(this->window);
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->engineUpdate();
		this->update();
		this->draw();
	}
}

void Game::handleEvents()
{
	while (this->window->pollEvent(this->e))
	{
		if (this->e.type == sf::Event::Closed)
		{
			this->window->close();
		}

		// give event to active state
		this->states[this->states.size() - 1]->handleEvents(e);
	}
}

void Game::update()
{
	this->handleEvents();


	if (!this->states.empty())
	{
		// update state on top of stack
		State* last = this->states[this->states.size() - 1];
		last->update(this->dt);

		// check if state wants to quit
		if (last->getQuit())
		{
			last->endState();
			delete last;
			last = nullptr;
			this->states.pop_back();
		}
	}
	// Application end
	else
	{
		this->window->close();
	}
}

void Game::draw()
{
	this->window->clear(sf::Color::White);

	// Render items
	for (auto& sp : this->states)
		sp->draw(this->window, this->UIView);

	this->window->display();


}
