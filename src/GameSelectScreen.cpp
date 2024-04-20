#include "GameSelectScreen.h"
#include "AudioService.h"
#include "GameEngine.h"

namespace gs
{

constexpr auto SPLASH_SCREEN_DURATION = 2.5f; // seconds

GameSelectScreen::GameSelectScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen{ target, resources },
    _logger{ log::initializeLogger("GameSelectScreen") }
{
    initGui();
}

void GameSelectScreen::initGui()
{
    auto easy = tgui::Button::create();
    easy->setText("Easy");
    easy->setTextSize(100);
    easy->setSize(650, 250);
    easy->setPosition(50,100);
    easy->onPress([=]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;
        _guiResult.data = gs::NewGameSettings{ gs::NewGameSettings::Level::EASY };
    });
    _guicache.emplace("easybutton", easy);
    _gui->add(easy);


    auto medium = tgui::Button::create();
    medium->setText("Medium");
    medium->setTextSize(100);
    medium->setSize(650, 250);
    medium->setPosition(50,400);
    medium->onPress([=]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;
        _guiResult.data = gs::NewGameSettings{ gs::NewGameSettings::Level::MEDIUM };
    });
    _guicache.emplace("mediumbutton", medium);
    _gui->add(medium);

    auto hard = tgui::Button::create();
    hard->setText("Hard");
    hard->setTextSize(100);
    hard->setSize(650, 250);
    hard->setPosition(50,700);
    hard->onPress([=]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;
        _guiResult.data = gs::NewGameSettings{ gs::NewGameSettings::Level::HARD };
    });
    _guicache.emplace("hardbutton", hard);
    _gui->add(hard);
}

PollResult GameSelectScreen::poll(const sf::Event& e)
{
    Screen::poll(e);
    if (_guiResult.type != gs::ActionType::NONE)
    {
        return _guiResult;
    }
    return {};
}

PollResult GameSelectScreen::update()
{
    return {};
}

} // namespace gs
