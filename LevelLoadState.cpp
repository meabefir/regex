#include "LevelLoadState.h"
#include "EditorState.h"
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

LevelLoadState::LevelLoadState(sf::RenderWindow* window, std::vector<State*>* states) :
	State(window, states)
{
	initFont();

	// white box
	sf::Vector2u window_size = this->window->getSize();
	sf::Vector2u size(400, 700);
	whiteBox.setFillColor(sf::Color(0, 0, 0, 185));
	whiteBox.setSize(sf::Vector2f(size.x, size.y));
	whiteBox.setPosition(sf::Vector2f(window_size.x / 2.f - size.x / 2.f, window_size.y / 2.f - size.y / 2.f));

	// get file names from folder and create buttons
	sf::Vector2f sizef(200.f, 75.f);
	int y_start = whiteBox.getPosition().y + 70.f;
	int i = 0;
	std::string path = "./Levels";
	for (auto& entry : fs::directory_iterator(path))
	{
		std::stringstream ss;
		ss << entry.path();
		std::string s = ss.str();

		std::size_t found = s.find_last_of('\\');
		s = s.substr(found+1);
		s = s.substr(0, s.size() - 5);
		

		std::string name = s;

		this->buttons[name] = new Button(this->whiteBox.getPosition().x + this->whiteBox.getSize().x / 2.f - sizef.x / 2.f,
			y_start + i * (sizef.y + 20.f), sizef.x, sizef.y, this->font, name,
			sf::Color::Green, sf::Color::Blue, sf::Color::Red);

		i++;
	}

	// info text render
	infoTextRender.setFont(*this->font);
	infoTextRender.setString("Select the level to be loaded");
	infoTextRender.setFillColor(sf::Color::White);
	infoTextRender.setPosition(window_size.x / 2.f, whiteBox.getPosition().y + 20.f);
	infoTextRender.setOrigin(infoTextRender.getGlobalBounds().width / 2.f,
		infoTextRender.getGlobalBounds().height / 2.f);

}

void LevelLoadState::initFont()
{
	this->font = new sf::Font();
	this->font->loadFromFile(AppData::fontPath);
}

LevelLoadState::~LevelLoadState()
{
}

void LevelLoadState::handleEvents(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
		if (e.key.code == sf::Keyboard::Key::Escape)
		{
			this->endState();
			return;
		}
}

void LevelLoadState::updateInput(const float& dt)
{
}

void LevelLoadState::update(const float& dt)
{
	for (auto& per : this->buttons)
	{
		per.second->update(Mouse::mousePosView);

		if (per.second->isPressed())
		{
			for (auto& state : *this->states)
			{
				EditorState* ep = dynamic_cast<EditorState*>(state);
				if (ep != nullptr)
				{
					ep->loadLevel(per.first);
					this->endState();
					return;
				}
			}
		}
	}
}

void LevelLoadState::draw(sf::RenderTarget* target)
{
	target->draw(whiteBox);
	target->draw(infoTextRender);
	target->draw(inputTextRender);

	for (auto& per : this->buttons)
	{
		per.second->draw(target);
	}
}
