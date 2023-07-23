#pragma once
#include <memory>

#include <SFML/Graphics.hpp>

#include "Screen.h"
#include "PollResult.h"

namespace gs
{

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