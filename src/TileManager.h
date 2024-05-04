#pragma once

#include <cassert>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "GeoSketchLogger.h"

namespace gs
{

class Tile : public sf::Drawable
{
    const sf::Color DEFAULT{sf::Color::Transparent};
    const sf::Color HIGHLIGHT{sf::Color{150,150,150}};
    const sf::Color SELECTED{sf::Color{200,200,200}};
    const sf::Color HISELECTED{sf::Color{100,100,50}};
    
public:
    
    Tile(const sf::Vector2f& location, const sf::Vector2f& size, float outlineThickness);
    
    bool selected() const { return _selected; }
    void setSelected(bool v)
    {
        _selected = v;
        resetColor();
    }
    
    bool hovered() const { return _hovered; }
    void setHovered(bool v)
    {
        _hovered = v;
        resetColor();
    }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_rect, states);
    }
    
    sf::Vector2f position() const
    {
        return _rect.getPosition();
    }
    
private:
    void resetColor();
    
    sf::RectangleShape  _rect;
    sf::Color           _color;

    bool                _selected = false;
    bool                _hovered = false;
};

class TileManager
{
  
public:
    TileManager(sf::RenderTarget& window, const sf::Vector2f& anchor, const sf::Vector2f& mapsize, const sf::Vector2u& gridsize, float outlineSize);
    
    void draw();
    void event(const sf::Event& ev);
    
    void clear();
    void setSelecting(bool v);
    
    sf::Vector2f anchor() const { return _anchor; }
    sf::Vector2f mapSize() const { return _mapSize; }
    
private:
    std::optional<sf::Vector2u> getXYCords(const sf::Vector2i& mouseCord);
    
    sf::RenderTarget&   _window;

    sf::Vector2f        _anchor;
    sf::Vector2f        _mapSize;
    sf::Vector2u        _gridSize;
    
    using TileContainer = std::vector<std::vector<std::shared_ptr<Tile>>>;
    TileContainer       _tileContainer;
    
    log::SpdLogPtr      _logger;
    
    // const properties
    const std::float_t      _xmin;
    const std::float_t      _xmax;
    const std::float_t      _ymin;
    const std::float_t      _ymax;
    const std::float_t      _tileWidth;
    const std::float_t      _tileHeight;
    
    std::shared_ptr<Tile>   _lastTile = nullptr;
    bool                    _dragging = false;
    bool                    _drawing = false;
    
};

} // namespace
