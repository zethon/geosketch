#pragma once

#include <memory>
#include <boost/any.hpp>
#include <SFML/Graphics.hpp>

#include "PollResult.h"

namespace gs
{

class Screen;
using ScreenPtr = std::shared_ptr<Screen>;

using RenderTargetPtr = std::shared_ptr<sf::RenderTarget>;

class Screen
{

public:
    Screen(RenderTargetPtr target);

    virtual void draw();
    virtual PollResult poll(const sf::Event&);
    virtual PollResult update();

private:
    RenderTargetPtr     _target;

}; 

} // namespace gs
