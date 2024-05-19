#include <fmt/format.h>
#include "../GameScreen.h"
#include "CountdownGameController.h"

namespace gs
{

CountdownGameController::CountdownGameController(const GameControllerConfig& config)
    : GameController{ config, ctrlrname }
{
    auto& tiles = config.screen->tilemanager();

    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _timer->setPosition(tiles.anchor().x - ((tiles.anchor().x / 2) + (_timer->getSize().x / 2)), 1);

    const auto minutes = _timeRemaingMili / 60000;
    const auto seconds = (_timeRemaingMili % 60000) / 1000;
    const auto tenths = (_timeRemaingMili % 1000) / 10;
    _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));

    _gui->add(_timer);
}

void CountdownGameController::updateTimer()
{
    if (!_start.has_value())
    {
        _start = std::chrono::steady_clock::now();
    }

    if (!_timeron) return;
    const auto timespace = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start.value());
    auto elapsedTime = _timeRemaingMili - static_cast<std::int32_t>(timespace.count());
    if (elapsedTime < 0)
    {
        _finished = true;
        elapsedTime = 0;
    }

    const auto minutes = elapsedTime / 60000;
    const auto seconds = (elapsedTime % 60000) / 1000;
    const auto tenths = (elapsedTime % 1000) / 10;
    _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));
}

PollResult CountdownGameController::update()
{
    updateTimer();

    if (_finished)
    {
        //auto goc = std::static_pointer_cast<CountdownGameOverController>(this->nextController());
        //goc->setScores(_scores);

        PollResult result{ ActionType::CHANGE_GAME_STATE };
        return result;
    }

    return {};
}

PollResult CountdownGameController::poll(const sf::Event&) 
{ 
    return {}; 
}

void CountdownGameController::draw()
{
    drawDrawables();
    drawGui();
}

void CountdownGameController::startController()
{
    this->_screen->setVisible(true);
    _timeron = true;
}

void CountdownGameController::endController()
{
}


CountdownGameOverController::CountdownGameOverController(const GameControllerConfig& config)
    : GameController{ config, ctrlrname }
{
    auto label = tgui::Label::create();
    label->setWidgetName("gameoverlbl");
    label->setText("Game Over");
    label->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.1));
    label->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = (_target.getView().getSize().x / 2) - (label->getSize().x / 2);
    const auto yloc = (_target.getView().getSize().y / 3) - (label->getSize().y / 2);
    label->setPosition(xloc, yloc);
    _gui->add(label);
}

PollResult CountdownGameOverController::update()
{
    return {};
}

PollResult CountdownGameOverController::poll(const sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
        default:
            break;

        case sf::Keyboard::Escape:
        {
            gs::PollResult result;
            result.type = gs::ActionType::CHANGE_SCREEN;
            result.data = SCREEN_GAMESELECT;
            return result;
        }
        break;
        }
    }

    return {};
}

void CountdownGameOverController::draw()
{
    drawDrawables();
    drawGui();
}

void CountdownGameOverController::startController()
{
    this->_screen->setVisible(false);
}

void CountdownGameOverController::endController()
{
}

} // namespace gs
