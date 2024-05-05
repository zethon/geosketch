#include "../GameScreen.h"
#include "GameStartController.h"

namespace gs
{

GameStartController::GameStartController(const GameControllerConfig& config)
    : GameController{config, ctrlrname}
{
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

void GameStartController::start()
{
    this->_screen->setVisible(false);
}

} // namespace gs
