#include "SplashScreen.h"
#include "AudioService.h"

namespace gs
{

constexpr auto SPLASH_SCREEN_DURATION = 3.5f; // seconds

SplashScreen::SplashScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen{ target, resources },
      _logger{ log::initializeLogger("Audio") }
{
    //const float desired_x = static_cast<float>(target.getSize().x - 50);
    const float desired_x = static_cast<float>(target.getSize().x / 2);
    const float desired_y = static_cast<float>(target.getSize().y - 200);

    _bg = *(_resources.load<sf::Texture>("images/splash.jpg"));
    const float bg_ratio = static_cast<float>(_bg.getSize().x) / static_cast<float>(_bg.getSize().y);
    auto sprite = std::make_shared<sf::Sprite>(_bg);

    const float bgXScale = static_cast<float>(desired_x / _bg.getSize().x);
    const float bgYScale = static_cast<float>(desired_y / _bg.getSize().y);

    sprite->setScale(bgXScale, bgXScale);
    
    auto xpos = (_target.getSize().x / 2) - ((_bg.getSize().x * bgXScale) / 2);
    auto ypos = (_target.getSize().y / 2) - ((_bg.getSize().y * bgYScale) / 2);
    sprite->setPosition(10, 10);
    
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
    if (_clock.getElapsedTime().asSeconds() > SPLASH_SCREEN_DURATION)
    {
        gs::AudioLocator::sound()->stop(gs::SplashScreen::INTRO_LOGO);
        return { ActionType::CHANGE_SCREEN, SCREEN_MAINMENU };
    }

    return {};
}

} // namespace gs
