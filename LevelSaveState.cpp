#include "LevelSaveState.h"
#include "EditorState.h"

LevelSaveState::LevelSaveState(sf::RenderWindow* window, std::vector<State*>* states):
	State(window, states)
{
	initFont();

	// white box
	sf::Vector2u window_size = this->window->getSize();
	sf::Vector2u size(400, 400);
	whiteBox.setFillColor(sf::Color(0, 0, 0, 185));
	whiteBox.setSize(sf::Vector2f(size.x, size.y));
	whiteBox.setPosition(sf::Vector2f(window_size.x / 2.f - size.x / 2.f, window_size.y / 2.f - size.y / 2.f));

	// button
	sf::Vector2f sizef(150.f, 75.f);
	this->buttons["SAVE"] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - sizef.x / 2.f,
		whiteBox.getPosition().y + this->whiteBox.getSize().y / 2.f - sizef.y / 2.f, sizef.x, sizef.y, this->font, "SAVE",
		sf::Color::Green, sf::Color::Blue, sf::Color::Red);

	// info text render
	infoTextRender.setFont(*this->font);
	infoTextRender.setString("Enter the name of the save");
	infoTextRender.setFillColor(sf::Color::White);
	infoTextRender.setPosition(window_size.x / 2.f, window_size.y / 2.f + 100.f);
	infoTextRender.setOrigin(infoTextRender.getGlobalBounds().width / 2.f,
							infoTextRender.getGlobalBounds().height / 2.f);

	// input text render
	inputTextRender.setFont(*this->font);
	inputTextRender.setString("");
	inputTextRender.setFillColor(sf::Color::White);
	inputTextRender.setPosition(window_size.x / 2.f, window_size.y / 2.f - 100.f);
	inputTextRender.setOrigin(inputTextRender.getGlobalBounds().width / 2.f,
								inputTextRender.getGlobalBounds().height / 2.f);
}

void LevelSaveState::initFont()
{
	this->font = new sf::Font();
	this->font->loadFromFile(AppData::fontPath);
}

LevelSaveState::~LevelSaveState()
{
}

void LevelSaveState::setText(std::string s)
{
	this->text = s;
	this->inputTextRender.setString(s);
	this->inputTextRender.setOrigin(infoTextRender.getGlobalBounds().width / 2.f,
									infoTextRender.getGlobalBounds().height / 2.f);
}

void LevelSaveState::handleEvents(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
		if (e.key.code == sf::Keyboard::Key::Escape)
		{
			this->endState();
			return;
		}
	if (e.type == sf::Event::TextEntered)
	{
		if (e.text.unicode == 8)
		{
			if (this->text.size() == 0)
				return;
			this->setText(text.substr(0, this->text.size() - 1));
		}
		else
		{
			if (e.text.unicode < 128 && this->text.size() < 15) {
				if (e.text.unicode == sf::Keyboard::Key::Space)
					return;
				this->setText(this->text + static_cast<char>(e.text.unicode));
			}
		}
	}
}

void LevelSaveState::updateInput(const float& dt)
{
}

void LevelSaveState::update(const float& dt)
{
	for (auto& per : this->buttons)
	{
		per.second->update(Mouse::mousePosView);
	}

	if (this->buttons["SAVE"]->isPressed())
	{
		if (this->text.size() == 0)
			return;
		for (auto& state : *this->states)
		{
			EditorState* ep = dynamic_cast<EditorState*>(state);
			if (ep != nullptr)
			{
				ep->saveLevel(this->text);
				this->endState();
				return;
			}
		}
	}
}

void LevelSaveState::draw(sf::RenderTarget* target, sf::View* UIView)
{
	// save the window view
	sf::View default_view = target->getView();
	// set the ui view
	target->setView(*UIView);

	target->draw(whiteBox);
	target->draw(infoTextRender);
	target->draw(inputTextRender);

	for (auto& per : this->buttons)
	{
		per.second->draw(target);
	}
	// set back the default view
	target->setView(default_view);
}
