#pragma once

#include <memory>
#include <boost/any.hpp>

#include <SFML/Graphics.hpp>
#include <TGUI/Widget.hpp>
#include <TGUI/TGUI.hpp>

#include "ResourceManager.h"
#include "PollResult.h"

namespace gs
{

class Screen;
using ScreenPtr = std::shared_ptr<Screen>;

using DrawablePtr = std::shared_ptr<sf::Drawable>;

class Screen
{

public:
    Screen(sf::RenderTarget& target, ResourceManager& resources);

    virtual void draw();
    virtual PollResult poll(const sf::Event&);
    virtual PollResult update();
    virtual void close();
    
    void addDrawable(DrawablePtr object);

protected:
    ResourceManager&            _resources;
    sf::RenderTarget&           _target;
    
    using WidgetCache = std::map<std::string, tgui::Widget::Ptr>;
    
    std::unique_ptr<tgui::Gui>  _gui;
    WidgetCache                 _guicache;
    PollResult                  _guiResult;
    
    std::vector<DrawablePtr>    _drawables;
}; 

} // namespace gs
