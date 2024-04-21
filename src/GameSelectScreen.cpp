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
    easy->setWidgetName("easybutton");
    easy->setTextSize(100);
    easy->setSize(650, 250);
    easy->setPosition(50,100);
    easy->onPress([=]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;

        gs::NewGameSettings settings{ this->getNewGameSettings() };
        settings.level = gs::NewGameSettings::Level::EASY;
        _guiResult.data = settings;
    });
    _guicache.emplace("easybutton", easy);
    _gui->add(easy);


    auto medium = tgui::Button::create();
    medium->setText("Medium");
    medium->setWidgetName("mediumbutton");
    medium->setTextSize(100);
    medium->setSize(650, 250);
    medium->setPosition(50,400);
    medium->onPress([=]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;

        gs::NewGameSettings settings{ this->getNewGameSettings() };
        settings.level = gs::NewGameSettings::Level::MEDIUM;
        _guiResult.data = settings;
    });
    _guicache.emplace("mediumbutton", medium);
    _gui->add(medium);

    auto hard = tgui::Button::create();
    hard->setText("Hard");
    hard->setWidgetName("hardbutton");
    hard->setTextSize(100);
    hard->setSize(650, 250);
    hard->setPosition(50,700);
    hard->onPress([&]
    {
        _guiResult.type = gs::ActionType::NEW_GAME;

        gs::NewGameSettings settings{ this->getNewGameSettings() }; 
        settings.level = gs::NewGameSettings::Level::HARD;
        _guiResult.data = settings;
    });
    _guicache.emplace("hardbutton", hard);
    _gui->add(hard);

    auto combobox = tgui::ComboBox::create();
    combobox->setWidgetName("combobox");
    combobox->setSize(360, 64);
    combobox->setTextSize(60);
    combobox->setPosition({ "easybutton.right + 50", "easybutton.top" });
    combobox->addItem("Free Mode");
    combobox->addItem("Timed Mode");
    combobox->setSelectedItemByIndex(0);
    combobox->onItemSelect([&](int index) 
        {
            auto label = _gui->get<tgui::Label>("comboboxlabel");
            if (index == 0)  
            {
                label->setText("Free Mode: No time limit");
            }
            else
            {
                label->setText("Timed Mode: 2 minutes");
            }
        });
    _gui->add(combobox);

    auto label = tgui::Label::create();
    label->setWidgetName("comboboxlabel");
    label->setSize(1024, 360);
    label->setTextSize(48);
    label->setPosition({ "combobox.left", "combobox.bottom + 25" });
    label->setText("Free Mode: No time limit");
    _gui->add(label);
}

NewGameSettings GameSelectScreen::getNewGameSettings() const
{
    // level gets set by the caller
    NewGameSettings settings;

    auto combobox = _gui->get<tgui::ComboBox>("combobox");
    if (combobox->getSelectedItemIndex() == 0)
    {
        settings.gameType = NewGameSettings::GameType::FREE;
    }
    else
    {
        settings.gameType = NewGameSettings::GameType::TIMED;
    }

    return settings;
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
