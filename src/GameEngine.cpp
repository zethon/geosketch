#include "GameEngine.h"

namespace gs
{

GameEngine::GameEngine(RenderTargetPtr target)
    : _target{ target }
{
    changeScreen(SCREEN_SPLASH);
}

void GameEngine::drawScreen()
{
    assert(_currentScreen);
    _currentScreen->draw();
}

void GameEngine::update()
{
    assert(_currentScreen);
    _currentScreen->update();
}

PollResult GameEngine::poll(const sf::Event& e)
{
    const auto result = _currentScreen->poll(e);
    if (result.type == ActionType::CHANGE_SCREEN)
    {
        changeScreen(boost::any_cast<std::uint16_t>(result.data));
    }
    else if (result.type == ActionType::EXIT_GAME)
    {
        return result;
    }

    return {};
}

void GameEngine::changeScreen(std::uint16_t screenId)
{
    throw std::runtime_error("Not implemented");
}

} // namespace gs