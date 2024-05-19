#include <fmt/core.h>

#include "MainMenuScreen.h"
#include "AudioService.h"

namespace gs
{

MainMenuScreen::MainMenuScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen(target, resources)
{
    initGui();
    gs::AudioLocator::music()->cacheAudio(THEME_SONG);
    gs::AudioLocator::music()->play(THEME_SONG);
    gs::AudioLocator::music()->setLoop(THEME_SONG, true);
}

void MainMenuScreen::initGui()
{
    _bg = *(_resources.load<sf::Texture>("images/background-mainmenu.png"));
    const float bg_ratio = static_cast<float>(_bg.getSize().x) / static_cast<float>(_bg.getSize().y);
    auto sprite = std::make_shared<sf::Sprite>(_bg);
    sprite->setPosition(0, 0);
    sprite->setScale(_target.getSize().x / sprite->getLocalBounds().width, _target.getSize().y / sprite->getLocalBounds().height);
    addDrawable(sprite);

    const auto buttonWidth = _target.getSize().x / 5;
    const auto buttonHeight = _target.getSize().y / 10;
    const auto textSize = _target.getSize().y / 25;

    auto newgame = tgui::Button::create();
    newgame->setText("New Game");
    newgame->setTextSize(textSize);
    newgame->setSize(buttonWidth, buttonHeight);
    newgame->setPosition(50,100);
    newgame->onPress([=]
    {
        gs::AudioLocator::music()->stop(THEME_SONG);
        _guiResult.type = gs::ActionType::CHANGE_SCREEN;
        _guiResult.data = gs::SCREEN_GAMESELECT;
    });
    _guicache.emplace("newgame", newgame);
    _gui->add(newgame);
    
    
    auto settings = tgui::Button::create();
    settings->setText("Settings");
    settings->setTextSize(textSize);
    settings->setSize(buttonWidth, buttonHeight);
    settings->setPosition(50,400);
    settings->onPress([=]
    {
        gs::AudioLocator::music()->stop(THEME_SONG);
        _guiResult.type = gs::ActionType::CHANGE_SCREEN;
        _guiResult.data = gs::SCREEN_SETTINGS;
    });
    _guicache.emplace("settings", settings);
    _gui->add(settings);
    
    auto exit = tgui::Button::create();
    exit->setText("Exit Game");
    exit->setTextSize(textSize);
    exit->setSize(buttonWidth, buttonHeight);
    exit->setPosition(50,700);
    exit->onPress([=]
    {
        gs::AudioLocator::music()->stop(THEME_SONG);
        _guiResult.type = gs::ActionType::EXIT_GAME;
    });
    _guicache.emplace("exit", exit);
    _gui->add(exit);
}

PollResult MainMenuScreen::poll(const sf::Event& e)
{
    Screen::poll(e);
    if (_guiResult.type != gs::ActionType::NONE)
    {
        return _guiResult;
    }
    
    return {};
}

} // namespace
