#include "Screen.h"

namespace gs
{

Screen::Screen(RenderTargetPtr target)
    : _target{ target }
{
}

void Screen::draw()
{
    throw std::runtime_error("Not implemented");
}

PollResult Screen::poll(const sf::Event&)
{
    return {};
}

PollResult Screen::update()
{
    return {};
}

} // namespace gs