#include "Screen.h"
#include "ResourceManager.h"

namespace gs
{

Screen::Screen(sf::RenderTarget& target, ResourceManager& resources)
    : _target{ target },
      _resources{ resources }
{
}

void Screen::draw()
{
    auto i = _drawables.begin();
    while (i != _drawables.end())
    {
        auto ptr = *i;
        if (ptr)
        {
            _target.draw(*ptr);
            i++;
        }
        else
        {
            i = _drawables.erase(i);
        }
    }
}

PollResult Screen::poll(const sf::Event&)
{
    return {};
}

PollResult Screen::update()
{
    return {};
}

void Screen::close()
{
    // nothing to do
}

void Screen::addDrawable(DrawablePtr object)
{
    _drawables.push_back(object);
}

} // namespace gs
