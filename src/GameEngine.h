#pragma once
#include <memory>

#include "boost/filesystem.hpp"

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "PollResult.h"
#include "ResourceManager.h"

namespace gs
{

using RenderTargetPtr = std::shared_ptr<sf::RenderTarget>;

struct GameSettings
{
    bool muteAllSounds = false;
    std::uint16_t startScreen = SCREEN_SPLASH;
};

struct NewGameSettings
{
    enum class Difficulty : std::uint16_t
    {
        EASY = 1,
        MEDIUM = 2,
        HARD = 3,
        EXPERT = 4
    };

    enum class GameType : std::uint16_t
    {
        FREE = 1,
        TIMED = 2,
        COUNTDOWN = 3
    };

    Difficulty difficulty = Difficulty::EASY;
    GameType gameType = GameType::FREE;
};

class GameEngine
{

public:
    GameEngine(sf::RenderTarget& target, const boost::filesystem::path& respath, const GameSettings& settings);

    void drawScreen();
    void update();
    PollResult poll(const sf::Event& e);


private:
    void initAudioService();
    void changeScreen(std::uint16_t screenId, const NewGameSettings& settings = {});
    
    sf::RenderTarget&   _target;
    ScreenPtr           _currentScreen;
    ResourceManager     _resources;
    GameSettings        _settings;
};

} // namespace gs

std::ostream& operator<<(std::ostream& os, const gs::NewGameSettings::Difficulty& diff);
std::ostream& operator<<(std::ostream& os, const gs::NewGameSettings::GameType& type);
std::ostream& operator<<(std::ostream& os, const gs::NewGameSettings& settings);
