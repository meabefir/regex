#include "NodeEditorState.h"
#include "AppData.h"
#include "EditorState.h"

NodeEditorState ::NodeEditorState(sf::RenderWindow* window, std::vector<State*>* states, Node* node, EditorState* editor):
	State(window, states), node(node), editor(editor)
{
	this->init();
}

NodeEditorState::NodeEditorState(const State& s, Node* node):
	State(s), node(node)
{
    this->initFont();
    this->init();
}

NodeEditorState::~NodeEditorState()
{
	for (auto& per : this->buttons)
	{
		delete per.second;
		per.second = nullptr;
	}
}

void NodeEditorState::init()
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
    this->text = this->node->getName();
    this->textRender.setFont(*this->font);
    this->textRender.setCharacterSize(50);
    this->textRender.setFillColor(sf::Color::Black);
    this->textPosition = sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y * 3.f / 4.f - 55.f);

    this->setText(text);

    // info text
    this->infoText.setFont(*this->font);
    this->infoText.setCharacterSize(40);
    this->infoText.setFillColor(sf::Color::Black);
    this->infoText.setString("Type a new name for the node");
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

    button_size = { 150,70 };
    distance_between = 75;
    this->buttons["START"] = new Button((float)window_size.x / 2 - button_size.x - distance_between,
        (float)window_size.y * 3.f / 4.f - button_size.y - 10,
        (float)button_size.x, (float)button_size.y,
        this->font, "START", sf::Color::Blue, sf::Color::Green, sf::Color::Red);
    this->buttons["FINAL"] = new Button((float)window_size.x / 2 + distance_between,
        (float)window_size.y * 3.f / 4.f - button_size.y - 10,
        (float)button_size.x, (float)button_size.y,
        this->font, "FINAL", sf::Color::Blue, sf::Color::Green, sf::Color::Red);

}

void NodeEditorState::initFont()
{
    this->font = new sf::Font();
    this->font->loadFromFile(AppData::fontPath);
}

void NodeEditorState::setText(std::string s)
{
    this->text = s;
    this->textRender.setString(s);
    this->recenterText(this->textRender, this->textPosition);

    this->node->setName(s);
}

void NodeEditorState::recenterText(sf::Text& txt, sf::Vector2f pos)
{
    txt.setPosition(pos.x - txt.getGlobalBounds().width / 2.f,
        pos.y - txt.getGlobalBounds().height / 2.f);
}

void NodeEditorState::ok()
{
    if (this->text == "")
        this->setText("lul");
    this->endState();
    Mouse::freezeMouse(.4f);
}

void NodeEditorState::cancel()
{
    this->endState();
    Mouse::freezeMouse(.4f);
}

void NodeEditorState::handleEvents(sf::Event e)
{
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
            //std::cout << e.text.unicode << '\n';
            if (e.text.unicode < 128 && this->text.size() < 3) {
                if (e.text.unicode == sf::Keyboard::Key::Space)
                    return;
                this->setText(this->text + static_cast<char>(e.text.unicode));
            }
        }
    }
}

void NodeEditorState::updateInput(const float& dt)
{

}

void NodeEditorState::update(const float& dt)
{
    for (auto& per : this->buttons)
    {
        per.second->update(Mouse::mousePosView);

        if (per.second->isPressed())
        {
            if (per.first == "OK")
                this->ok();
            else if (per.first == "CANCEL")
                this->cancel();
            else if (this->buttons["FINAL"]->isPressed())
            {
                this->node->toggleFinal();
                this->cancel();
                return;
            }
            else if (this->buttons["START"]->isPressed())
            {
                EditorState* ep = nullptr;
                for (auto& sp : *this->states)
                {
                    ep = dynamic_cast<EditorState*>(sp);
                    if (ep != nullptr)
                        break;
                }
                Node* start_node = ep->getStartNode();
                if (start_node != this->node)
                {
                    this->node->toggleStart();
                    this->cancel();
                    return;
                }
                else
                    this->cancel();
            }
        }
    }

   
}

void NodeEditorState::draw(sf::RenderTarget* target)
{
    target->draw(this->whiteBox);
    for (auto& per : this->buttons)
        per.second->draw(target);
    target->draw(this->textRender);
    target->draw(this->infoText);
}
