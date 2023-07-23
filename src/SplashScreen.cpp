#include "SplashScreen.h"

namespace gs
{

SplashScreen::SplashScreen(RenderTargetPtr target)
    : Screen{ target }
{
    // print stuff

    _clock.restart();
}

void SplashScreen::draw()
{

}

PollResult SplashScreen::poll(const sf::Event&)
{
    return {};
}

PollResult SplashScreen::update()
{
    if (_clock.getElapsedTime().asSeconds() > 3)
    {
        return { ActionType::CHANGE_SCREEN, SCREEN_MAINMENU };
    }

    return {};
}

} // namespace gs