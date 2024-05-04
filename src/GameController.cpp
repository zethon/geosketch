#include "GameController.h"

namespace gs
{


GameControllerPtr createGameController(GameMode mode)
{
    switch (mode)
    {
        case GameMode::COUNTDOWN:
            return std::make_shared<CountdownController>(60);
        case GameMode::TIMED:
            return std::make_shared<TimedGameController>();
        case GameMode::FREE:
            return std::make_shared<FreeGameController>();
        default:
            return nullptr;
    }
}

CountdownController::CountdownController(std::uint32_t totalTime)
    : _totalTime{totalTime}
{ 
    // nothing to do
}

PollResult CountdownController::update()
{
    return {};
}

PollResult GameStartController::update()
{
    return PollResult();
}

void GameStartController::setNextController(GameControllerPtr nextController)
{
    _nextController = nextController;
}

GameControllerPtr GameStartController::nextController() const
{
    return _nextController;
}

} // namespace gs
