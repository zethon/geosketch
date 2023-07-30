#include "SplashScreen.h"

namespace gs
{

SplashScreen::SplashScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen{ target, resources }
{
    constexpr auto bgXScale = 0.33f;
    constexpr auto bgYScale = 0.33f;

    _bg = *(_resources.load<sf::Texture>("images/splash.jpg"));
    auto sprite = std::make_shared<sf::Sprite>(_bg);
    sprite->setScale(bgXScale, bgYScale);
    
    auto xpos = (_target.getSize().x / 2) - ((_bg.getSize().x * bgXScale) / 2);
    auto ypos = (_target.getSize().y / 2) - ((_bg.getSize().y * bgYScale) / 2);
    sprite->setPosition(xpos, ypos - 30);
    
    addDrawable(sprite);

    _clock.restart();
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
