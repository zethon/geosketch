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
    
    template <class DrawableT, typename... Args>
    std::shared_ptr<DrawableT> emplaceDrawable(Args&&... args)
    {
        auto item = std::make_shared<DrawableT>(std::forward<Args>(args)...);
        _drawables.push_back(item);
        return item;
    }

    void setVisible(bool visible) { _visible = visible; }
    bool isVisible() const { return _visible; }

protected:
    ResourceManager&            _resources;
    sf::RenderTarget&           _target;
    
    using WidgetCache = std::map<std::string, tgui::Widget::Ptr>;
    
    std::unique_ptr<tgui::Gui>  _gui;
    WidgetCache                 _guicache;
    PollResult                  _guiResult;
    
    std::vector<DrawablePtr>    _drawables;

private:
    bool                        _visible { true };
}; 

} // namespace gs
