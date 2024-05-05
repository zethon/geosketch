#include "../GameScreen.h"
#include "TimedGameController.h"

namespace gs
{

TimedGameController::TimedGameController(const GameControllerConfig& config)
    : GameController{config, ctrlrname}
{
    auto& tiles = config.screen->tiles();

    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _timer->setText("00:00.00");
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _timer->setPosition(tiles.anchor().x - ((tiles.anchor().x / 2) + (_timer->getSize().x / 2)), 1);
    _gui->add(_timer);

    auto underline = std::make_shared<sf::RectangleShape>(sf::Vector2f{static_cast<std::float_t>(_timer->getSize().x + 50), 5.0f});
    _drawables.push_back(underline);
    underline->setPosition(_timer->getPosition().x - 25, _timer->getPosition().y + _timer->getSize().y + 10);
}

PollResult TimedGameController::update()
{
    if (!_timeron)
    {
        _start2 = std::chrono::steady_clock::now();
        _timeron = true;
    }

    const auto timespace = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start2);
    const auto minutes = timespace.count() / 60000;
    const auto seconds = (timespace.count() % 60000) / 1000;
    const auto tenths = (timespace.count() % 1000) / 10;
    _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));

    return {};
}

PollResult TimedGameController::poll(const sf::Event&)
{ 
    return {}; 
}

void TimedGameController::draw()
{ 
    drawDrawables();
    drawGui();
};    

}  // namespace gs
