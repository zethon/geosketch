#pragma once
#include <memory>
// #include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

#include "Screens/Screen.h"
#include "PollResult.h"

namespace gs
{

constexpr std::uint16_t SCREEN_SPLASH = 10;
constexpr std::uint16_t SCREEN_MAINMENU = 20;
constexpr std::uint16_t SCREEN_LOADING = 30;
constexpr std::uint16_t SCREEN_GAME = 40;
constexpr std::uint16_t SCREEN_GAMEOVER = 50;
constexpr std::uint16_t SCREEN_SETTINGS = 60;

using RenderTargetPtr = std::shared_ptr<sf::RenderTarget>;

class GameEngine
{

public:
    GameEngine(RenderTargetPtr target);

    void drawScreen();
    void update();
    PollResult poll(const sf::Event& e);


private:
    void changeScreen(std::uint16_t screenId);

    RenderTargetPtr     _target;
    ScreenPtr           _currentScreen;

};


} // namespace gs