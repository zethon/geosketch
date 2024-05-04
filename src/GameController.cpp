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
    return {};
}

PollResult GameStartController::poll(const sf::Event&)
{ 
    return {}; 
}

void GameStartController::draw()
{ 
    // nothing to do
};


} // namespace gs
