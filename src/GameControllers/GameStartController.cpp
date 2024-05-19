#include "../GameScreen.h"
#include "GameStartController.h"

namespace gs
{

GameStartController::GameStartController(const GameControllerConfig& config)
    : GameController{config, ctrlrname}
{
    auto label = tgui::Label::create();
    label->setWidgetName("label");
    label->setText("Start in...");
    label->setTextSize(static_cast<std::uint32_t>(_target.getSize().y * 0.0750));
    label->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = (config.target.getSize().x / 2) - (label->getSize().x / 2);
    const auto yloc = (config.target.getSize().y * 0.1);
    label->setPosition(xloc, yloc);
    _gui->add(label);

    auto countdown = tgui::Label::create();
    countdown->setWidgetName("countdown");
    countdown->setTextSize(static_cast<std::uint32_t>(_target.getSize().y * 0.25));
    countdown->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc2 = (config.target.getSize().x / 2) - (countdown->getSize().x / 2);
    const auto yloc2 = (config.target.getSize().y * 0.5) - (countdown->getSize().y / 2);
    countdown->setPosition(xloc2, yloc2);
    _gui->add(countdown);
}

PollResult GameStartController::update()
{
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - _start);
    if (elapsed.count() >= _stepdelay)
    {
        _countdown--;

        auto countdown = _gui->get<tgui::Label>("countdown");
        if (_countdown > 0)
        {
            countdown->setText(std::to_string(_countdown));
        }
        else
        {
            countdown->setText("Go!");
            _stepdelay = 750;
        }
        
        const auto xloc2 = (this->_target.getSize().x / 2) - (countdown->getSize().x / 2);
        const auto yloc2 = (this->_target.getSize().y * 0.5) - (countdown->getSize().y / 2);
        countdown->setPosition(xloc2, yloc2);
        
        _start = now;
    }

    if (_countdown < 0)
    {
        PollResult result;
        result.type = ActionType::CHANGE_GAME_STATE;
        return result;
    }

    return {};
}

PollResult GameStartController::poll(const sf::Event& e)
{ 
    _gui->handleEvent(e);
    return {}; 
}

void GameStartController::draw()
{ 
    drawDrawables();
    drawGui();
}

void GameStartController::startController()
{
    auto countdown = _gui->get<tgui::Label>("countdown");
    countdown->setText(std::to_string(COUNTDOWN));
    const auto xloc2 = (this->_target.getSize().x / 2) - (countdown->getSize().x / 2);
    const auto yloc2 = (this->_target.getSize().y * 0.5) - (countdown->getSize().y / 2);
    countdown->setPosition(xloc2, yloc2);

    this->_screen->setVisible(false);
}

} // namespace gs
