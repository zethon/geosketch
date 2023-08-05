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
    auto newgame = tgui::Button::create();
    newgame->setText("New Game");
    newgame->setTextSize(100);
    newgame->setSize(650, 250);
    newgame->setPosition(50,100);
    newgame->onPress([=]
    {
        gs::AudioLocator::music()->stop(THEME_SONG);
        _guiResult.type = gs::ActionType::CHANGE_SCREEN;
        _guiResult.data = gs::SCREEN_GAME;
    });
    _guicache.emplace("newgame", newgame);
    _gui->add(newgame);
    
    
    auto settings = tgui::Button::create();
    settings->setText("Settings");
    settings->setTextSize(100);
    settings->setSize(650, 250);
    settings->setPosition(50,400);
    _guicache.emplace("settings", settings);
    _gui->add(settings);
    
    auto exit = tgui::Button::create();
    exit->setText("Exit Game");
    exit->setTextSize(100);
    exit->setSize(650, 250);
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
