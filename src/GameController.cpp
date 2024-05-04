#include "GameScreen.h"
#include "GameController.h"

namespace chrono = std::chrono;
namespace gs
{

GameControllerPtr createGameController(const GameControllerConfig& config)
{
    GameControllerPtr retval;
    switch (config.settings.gameType)
    {
        case GameMode::COUNTDOWN:
        {
            retval = std::make_shared<GameStartController>(config);
            auto next = retval->setNextController(std::make_shared<TimeLimitGameController>(config));
            next = next->setNextController(std::make_shared<TimeLimitGameOverController>(config));
        }
        break;
            
        case GameMode::TIMED:
        {
            retval = std::make_shared<GameStartController>(config);
            auto next = retval->setNextController(std::make_shared<TimedGameController>(config));
            next = next->setNextController(std::make_shared<TimedGameOverController>(config));
        }
        break;

        case GameMode::FREE:
        {
            retval = std::make_shared<FreeGameController>(config);
            retval->setNextController(std::make_shared<FreeGameOverController>(config));
        }
        break;

        default:
            assert(false); // should never get here
            return nullptr;
    }

    retval->setNextController(std::make_shared<NullGameController>(config));
    return retval;
}

GameControllerPtr GameController::setNextController(GameControllerPtr nextController)
{
    _nextController = nextController;
    return _nextController;
}

GameControllerPtr GameController::nextController() const
{
    return _nextController;
}

void GameController::drawDrawables()
{
    auto i = _drawables.begin();
    while (i != _drawables.end())
    {
        auto ptr = *i;
        if (ptr)
        {
            _target.draw(*ptr);
            i++;
        }
        else
        {
            i = _drawables.erase(i);
        }
    }
}

void GameController::drawGui()
{
    _gui->draw();
}

GameStartController::GameStartController(const GameControllerConfig& config)
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

PollResult GameStartController::update()
{
    auto now = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - _start);
    if (elapsed.count() >= 1000)
    {
        _logger->info("countdown: {}", _countdown);
        _countdown++;
        _start = now;
    }

    if (_countdown >= 3)
    {
        PollResult result;
        result.type = ActionType::CHANGE_GAME_STATE;
        return result;
    }

    if (_timeron)
    {
        const auto timespace = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start2);
        const auto minutes = timespace.count() / 60000;
        const auto seconds = (timespace.count() % 60000) / 1000;
        const auto tenths = (timespace.count() % 1000) / 10;
        _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));
    }

    return {};
}

PollResult GameStartController::poll(const sf::Event& e)
{ 
    _gui->handleEvent(e);
    if (e.type == sf::Event::KeyPressed)
    {        
        switch (e.key.code)
        {
            default:
            break;

            case sf::Keyboard::Space:
            {
                if (_timeron)
                {
                    _timeron = false;
                }
                else
                {
                    _timeron = true;
                    _start = std::chrono::steady_clock::now();
                }
                return {};
            }
            break;
        }
    }

    return {}; 
}

void GameStartController::draw()
{ 
    drawDrawables();
    drawGui();
}

} // namespace gs
