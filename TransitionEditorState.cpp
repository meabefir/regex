#include "TransitionEditorState.h"
#include "Transition.h"
#include "Button.h"
#include "Mouse.h"

TransitionEditorState::TransitionEditorState(sf::RenderWindow* window, std::vector<State*>* states, Transition* t):
	State(window, states), transition(t)
{
    this->init();
}

TransitionEditorState::TransitionEditorState(const State& s, Transition* t):
	State(s), transition(t)
{
    this->initFont();
    this->init();
}



TransitionEditorState::~TransitionEditorState()
{
    for (auto& per : this->buttons)
    {
        delete per.second;
        per.second = nullptr;
    }
}

void TransitionEditorState::init()
{
    sf::Vector2u window_size = this->window->getSize();

    // white box
    sf::Vector2f size(500, 260);
    this->whiteBox.setFillColor(sf::Color(255, 255, 255, 200));
    this->whiteBox.setOutlineColor(sf::Color::Black);
    this->whiteBox.setOutlineThickness(3);
    this->whiteBox.setSize(size);
    this->whiteBox.setPosition(window_size.x / 2 - size.x / 2.f, window_size.y * 3.f / 4.f - size.y / 2.f);

    // text render
    this->text = this->transition->text;
    this->textRender.setFont(*this->font);
    this->textRender.setCharacterSize(50);
    this->textRender.setFillColor(sf::Color::Black);
    this->textPosition = sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y * 3.f / 4.f - 55.f);

    this->setText(text);
    
    // info text
    this->infoText.setFont(*this->font);
    this->infoText.setCharacterSize(40);
    this->infoText.setFillColor(sf::Color::Black);
    this->infoText.setString("Type letters separated by comma");
    this->recenterText(this->infoText, this->textPosition + sf::Vector2f(0, 150));

    sf::Vector2u button_size = { 200,75 };
    int distance_between = 20;
    this->buttons["CANCEL"] = new Button((float)window_size.x / 2 - button_size.x - distance_between,
                            (float)window_size.y * 3.f / 4.f,
                            (float)button_size.x, (float)button_size.y,
                            this->font, "CANCEL", sf::Color::Blue, sf::Color::Green, sf::Color::Red);
    this->buttons["OK"] = new Button((float)window_size.x / 2 + distance_between,
                            (float)window_size.y * 3.f / 4.f,
                            (float)button_size.x, (float)button_size.y,
                            this->font, "OK", sf::Color::Blue, sf::Color::Green, sf::Color::Red);
}

void TransitionEditorState::initFont()
{
    this->font = new sf::Font();
    this->font->loadFromFile(AppData::fontPath);
}

void TransitionEditorState::setText(std::string s)
{
    this->text = s;
    this->textRender.setString(s);
    this->recenterText(this->textRender, this->textPosition);

    this->transition->setText(s);
}

void TransitionEditorState::recenterText(sf::Text& txt, sf::Vector2f pos)
{
    txt.setPosition(pos.x - txt.getGlobalBounds().width / 2.f,
        pos.y - txt.getGlobalBounds().height / 2.f);
}

void TransitionEditorState::ok()
{
    if (this->text == "")
        this->setText("e");
    this->endState();
    Mouse::freezeMouse(.4f);
}

void TransitionEditorState::cancel()
{
    
    if (this->text == "")
        this->transition->node1->removeTransition(this->transition);
    this->endState();
    Mouse::freezeMouse(.4f);
}

void TransitionEditorState::handleEvents(sf::Event e)
{
    if (e.type == sf::Event::KeyReleased)
        if (e.key.code == sf::Keyboard::Key::Escape)
        {
            this->cancel();
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
        else if (e.text.unicode == 13)
        {
            // ok
            this->ok();
            return;
        }
        else
        {
            if (e.text.unicode == 27)
            {
                return;
            }
            if (e.text.unicode < 128) {
                if (e.text.unicode == 32)
                    return;
                this->setText(this->text + static_cast<char>(e.text.unicode));
            }
        }
    }
}

void TransitionEditorState::updateInput(const float& dt)
{
}

void TransitionEditorState::update(const float& dt)
{
    for (auto& per : this->buttons)
        per.second->update(Mouse::mousePosView);

    // check if buttons are pressed
    if (this->buttons["OK"]->isPressed())
    {
        this->ok();
    }
    if (this->buttons["CANCEL"]->isPressed())
    {
        this->cancel();
    }
}



void TransitionEditorState::draw(sf::RenderTarget* target)
{
    target->draw(this->whiteBox);
    for (auto& per : this->buttons)
        per.second->draw(target);
    target->draw(this->textRender);
    target->draw(this->infoText);
}
