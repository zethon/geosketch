#include "../GameScreen.h"
#include "GameController.h"
#include "GameStartController.h"
#include "TimedGameController.h"
#include "CountdownGameController.h"

namespace chrono = std::chrono;
namespace gs
{

GameControllerPtr createGameController(const GameControllerConfig& config)
{
    GameControllerPtr retval;
    switch (config.settings.gameType)
    {
        case GameMode::COUNTDOWN: // count down
        {
            retval = std::make_shared<GameStartController>(config);
            auto next = retval->setNextController(std::make_shared<CountdownGameController>(config));
            next = next->setNextController(std::make_shared<CountdownGameOverController>(config));
            next->setNextController(std::make_shared<NullGameController>(config));
        }
        break;
            
        case GameMode::TIMED: // count up
        {
            retval = std::make_shared<GameStartController>(config);
            auto next = retval->setNextController(std::make_shared<TimedGameController>(config));
            next = next->setNextController(std::make_shared<TimedGameOverController>(config));
            next->setNextController(std::make_shared<NullGameController>(config));
        }
        break;

        case GameMode::FREE:
        {
            retval = std::make_shared<FreeGameController>(config);
            auto next = retval->setNextController(std::make_shared<FreeGameOverController>(config));
            next->setNextController(std::make_shared<NullGameController>(config));
        }
        break;

        default:
            assert(false); // should never get here
            return nullptr;
    }

    return retval;
}

GameControllerPtr GameController::setNextController(GameControllerPtr nextController)
{
    _nextController = nextController;
    return _nextController;
}

GameControllerPtr GameController::nextController()
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

} // namespace gs
