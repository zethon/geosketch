#include "SplashScreen.h"
#include "AudioService.h"

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
    
    _font = *(_resources.load<sf::Font>("fonts/pricedown.ttf"));
    
    auto logoText = std::make_shared<sf::Text>("lulzapps & Dog Finger Studios", _font);
    logoText->setCharacterSize(100);

    logoText->setOutlineColor(sf::Color(0,0,0,255));
    logoText->setOutlineThickness(5);

    auto txpos = (_target.getSize().x / 2) - ((logoText->getLocalBounds().width) / 2);
    logoText->setPosition(txpos, _target.getSize().y - 150.0f);

    addDrawable(logoText);

    gs::AudioLocator::sound()->cacheAudio(gs::SplashScreen::INTRO_LOGO);
    gs::AudioLocator::sound()->play(gs::SplashScreen::INTRO_LOGO);

    _clock.restart();
}

PollResult SplashScreen::poll(const sf::Event&)
{
    return {};
}

PollResult SplashScreen::update()
{
    if (_clock.getElapsedTime().asSeconds() > 2)
    {
        gs::AudioLocator::sound()->stop(gs::SplashScreen::INTRO_LOGO);
        return { ActionType::CHANGE_SCREEN, SCREEN_MAINMENU };
    }

    return {};
}

} // namespace gs
