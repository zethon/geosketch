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
    auto easybtn = tgui::RadioButton::create();
    easybtn->setText("Easy");
    easybtn->setWidgetName("easybutton");
    easybtn->setTextSize(48);
    easybtn->setSize(100, 48);
    easybtn->setPosition({"10%", "10%"});
    easybtn->onClick([&]{ this->_difficulty = NewGameSettings::Difficulty::EASY; });
    easybtn->setChecked(true);
    _gui->add(easybtn);

    auto mediumbtn = tgui::RadioButton::create();
    mediumbtn->setText("Medium");
    mediumbtn->setWidgetName("mediumbutton");
    mediumbtn->setTextSize(48);
    mediumbtn->setSize(100, 48);
    mediumbtn->setPosition({"10%", "20%"});
    mediumbtn->onClick([&]{ this->_difficulty = NewGameSettings::Difficulty::MEDIUM; });
    _gui->add(mediumbtn);

    auto hardbtn = tgui::RadioButton::create();
    hardbtn->setText("Hard");
    hardbtn->setWidgetName("hardbutton");
    hardbtn->setTextSize(48);
    hardbtn->setSize(100, 48);
    hardbtn->setPosition({"10%", "30%"});
    hardbtn->onClick([&]{ this->_difficulty = NewGameSettings::Difficulty::HARD; });
    _gui->add(hardbtn);

    auto expertbtn = tgui::RadioButton::create();
    expertbtn->setText("Expert");
    expertbtn->setWidgetName("expertbutton");
    expertbtn->setTextSize(48);
    expertbtn->setSize(100, 48);
    expertbtn->setPosition({"10%", "40%"});
    expertbtn->onClick([&]{ this->_difficulty = NewGameSettings::Difficulty::EXPERT; });
    _gui->add(expertbtn);

    auto combobox = tgui::ComboBox::create();
    combobox->setWidgetName("combobox");
    combobox->setSize(480, 64);
    combobox->setTextSize(36);
    combobox->setPosition({ "50%", "10%" });
    // see: https://github.com/zethon/geosketch/wiki
    combobox->addItem("Free Mode");
    combobox->addItem("Timed Mode");
    combobox->addItem("Countdown Mode");
    combobox->setSelectedItemByIndex(0);
    _gui->add(combobox);

    auto back = tgui::Button::create();
    back->setText("< Back");
    back->setWidgetName("backbutton");
    back->setTextSize(48);
    back->setSize(360, 64);
    back->setPosition({ "10%", "90%" });
    back->onPress([=]
    {
        _guiResult.type = gs::ActionType::CHANGE_SCREEN;
        _guiResult.data = SCREEN_MAINMENU;
    });
    _gui->add(back);

    auto startgame = tgui::Button::create();
    startgame->setText("Start Game >");
    startgame->setWidgetName("startgamebutton");
    startgame->setTextSize(48);
    startgame->setSize(360, 64);
    startgame->setPosition({ "90% - width", "90%" });
    startgame->onPress([&](){ this->onStartGame(); });
    _gui->add(startgame);
}

NewGameSettings GameSelectScreen::getNewGameSettings() const
{
    // level gets set by the caller
    NewGameSettings settings;

    settings.difficulty = _difficulty;

    auto combobox = _gui->get<tgui::ComboBox>("combobox");
    switch (combobox->getSelectedItemIndex())
    {
        default:
            settings.gameType = NewGameSettings::GameType::FREE;
        break;
        case 1:
            settings.gameType = NewGameSettings::GameType::TIMED;
        break;
        case 2:
            settings.gameType = NewGameSettings::GameType::COUNTDOWN;
        break;
    }

    return settings;
}

void GameSelectScreen::onStartGame()
{
    _guiResult.type = gs::ActionType::NEW_GAME;

    gs::NewGameSettings settings{ this->getNewGameSettings() };
    _guiResult.data = settings;
}

PollResult GameSelectScreen::poll(const sf::Event& e)
{
    Screen::poll(e);
    if (_guiResult.type != gs::ActionType::NONE)
    {
        return _guiResult;
    }

    if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Escape)
        {
            _guiResult.type = gs::ActionType::CHANGE_SCREEN;
            _guiResult.data = SCREEN_MAINMENU;
            return _guiResult;
        }
        else if (e.key.code == sf::Keyboard::Up)
        {
            // get the easy button and increase the text size
            auto easy = _gui->get<tgui::RadioButton>("easybutton");
            easy->setTextSize(easy->getTextSize() + 10);
        } 
        else if (e.key.code == sf::Keyboard::Down)
        {
            // get the easy button and decrease the text size
            auto easy = _gui->get<tgui::RadioButton>("easybutton");
            easy->setTextSize(easy->getTextSize() - 10);
        }
    }
    return {};
}

PollResult GameSelectScreen::update()
{
    return {};
}

} // namespace gs
