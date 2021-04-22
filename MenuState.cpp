#include "MenuState.h"
#include "Mouse.h"
#include "LevelSaveState.h"
#include "LevelLoadState.h"
#include "EditorState.h"

MenuState::MenuState(sf::RenderWindow* window, std::vector<State*>* states):
	State(window, states), whiteBox()
{
	initFont();

	// init white box
	float margin = 25.f;
	sf::Vector2f size(200, this->window->getSize().y - 2 * margin);

	this->whiteBox.setFillColor(sf::Color(0, 0, 0, 170));
	this->whiteBox.setOutlineColor(sf::Color::Black);
	this->whiteBox.setOutlineThickness(3);
	this->whiteBox.setSize(size);
	this->whiteBox.setPosition(margin, margin);

	// add buttons
	size = sf::Vector2f(150.f, 85.f);
	this->buttons["LOAD"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
							50.f, size.x, size.y, this->font, "LOAD",
							sf::Color(38, 70, 83), sf::Color(42, 157, 143), sf::Color(233, 196, 106));
	this->buttons["SAVE"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
							150.f, size.x, size.y, this->font, "SAVE",
							sf::Color(38, 70, 83), sf::Color(42, 157, 143), sf::Color(233, 196, 106));
	this->buttons["CLEAR"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
							250.f, size.x, size.y, this->font, "CLEAR",
							sf::Color(38, 70, 83), sf::Color(42, 157, 143), sf::Color(233, 196, 106));

	this->buttons["REGEX"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
		this->whiteBox.getSize().y + this->whiteBox.getPosition().y - size.y - 25.f, size.x, size.y, this->font, "REGEX",
		sf::Color(38, 70, 83), sf::Color(42, 157, 143), sf::Color(233, 196, 106));
}

MenuState::~MenuState()
{
	for (auto& per : this->buttons)
		delete per.second;
}

void MenuState::initFont()
{
	this->font = new sf::Font();
	this->font->loadFromFile(AppData::fontPath);
}

void MenuState::handleEvents(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Key::Escape)
			this->endState();
	}
}

void MenuState::updateInput(const float& dt)
{
}

void MenuState::update(const float& dt)
{
	for (auto& per : this->buttons)
	{
		per.second->update(Mouse::mousePosView);
	}
	
	if (this->buttons["LOAD"]->isPressed())
	{
		this->states->push_back(new LevelLoadState(this->window, this->states));
	}
	else if (this->buttons["SAVE"]->isPressed())
	{
		this->states->push_back(new LevelSaveState(this->window, this->states));
	}
	else if (this->buttons["CLEAR"]->isPressed())
	{
		//find editor
		for (auto& state : *this->states)
		{
			EditorState* ep = dynamic_cast<EditorState*>(state);
			if (ep != nullptr)
			{
				ep->clearNodes();
				return;
			}
		}
	}
	else if (this->buttons["REGEX"]->isPressed())
	{
		//find editor
		for (auto& state : *this->states)
		{
			EditorState* ep = dynamic_cast<EditorState*>(state);
			if (ep != nullptr)
			{
				ep->buildAutomata();
				return;
			}
		}
	}
}

void MenuState::draw(sf::RenderTarget* target, sf::View* UIView)
{
	// save the window view
	sf::View default_view = target->getView();
	// set the ui view
	target->setView(*UIView);


	target->draw(whiteBox);

	for (auto& per : this->buttons)
	{
		per.second->draw(target);
	}

	// set back the default view
	target->setView(default_view);
}
