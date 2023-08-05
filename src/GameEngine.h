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


class GameEngine
{

public:
    GameEngine(sf::RenderTarget& target, const boost::filesystem::path& respath, const GameSettings& settings);

    void drawScreen();
    void update();
    PollResult poll(const sf::Event& e);


private:
    void initAudioService();
    void changeScreen(std::uint16_t screenId);
    
    sf::RenderTarget&   _target;
    ScreenPtr           _currentScreen;
    ResourceManager     _resources;
    GameSettings        _settings;
};


} // namespace gs
