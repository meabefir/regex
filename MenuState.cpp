#include "MenuState.h"
#include "Mouse.h"
#include "LevelSaveState.h"

MenuState::MenuState(sf::RenderWindow* window, std::vector<State*>* states):
	State(window, states), whiteBox()
{
	initFont();

	// init white box
	float margin = 25.f;
	sf::Vector2f size(200, this->window->getSize().y - 2 * margin);

	this->whiteBox.setFillColor(sf::Color(255, 255, 255, 200));
	this->whiteBox.setOutlineColor(sf::Color::Black);
	this->whiteBox.setOutlineThickness(3);
	this->whiteBox.setSize(size);
	this->whiteBox.setPosition(margin, margin);

	// add buttons
	size = sf::Vector2f(150.f, 75.f);
	this->buttons["LOAD"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
							75.f, size.x, size.y, this->font, "LOAD",
							sf::Color::Green, sf::Color::Blue, sf::Color::Red);
	this->buttons["SAVE"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - size.x / 2.f,
							200.f, size.x, size.y, this->font, "SAVE",
							sf::Color::Green, sf::Color::Blue, sf::Color::Red);
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

	}
	else if (this->buttons["SAVE"]->isPressed())
	{
		this->states->push_back(new LevelSaveState(this->window, this->states));
	}
}

void MenuState::draw(sf::RenderTarget* target)
{
	target->draw(whiteBox);

	for (auto& per : this->buttons)
	{
		per.second->draw(target);
	}
}
