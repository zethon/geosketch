#include "Screen.h"
#include "ResourceManager.h"

namespace gs
{

Screen::Screen(sf::RenderTarget& target, ResourceManager& resources)
    : _target{ target },
      _resources{ resources },
      _gui{ std::make_unique<tgui::Gui>(_target) }
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
    
    _gui->draw();
}

PollResult Screen::poll(const sf::Event& e)
{
    _gui->handleEvent(e);
    return {};
}

PollResult Screen::update()
{
    return {};
}

void Screen::close()
{
    _guicache.clear();
}

void Screen::addDrawable(DrawablePtr object)
{
    _drawables.push_back(object);
}

} // namespace gs
