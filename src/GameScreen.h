#pragma once

#include "Screen.h"
#include "GeoSketchLogger.h"

namespace gs
{

class Tile : public sf::Drawable
{
    
public:
    Tile(const sf::Vector2f& location, const sf::Vector2f& size)
        : _color{sf::Color::Transparent}
    {
        _rect.setPosition(location.x, location.y);
        _rect.setSize(size);
        _rect.setFillColor(_color);
        _rect.setOutlineColor(sf::Color(122, 122, 122));
        _rect.setOutlineThickness(0.5f);
    }
    
    void setColor(const sf::Color& color)
    {
        _rect.setFillColor(color);
    }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_rect, states);
    }
    
private:
    sf::RectangleShape  _rect;
    sf::Color           _color;
};

class TileManager
{
  
public:
    TileManager(sf::RenderTarget& window, const sf::Vector2f& anchor, const sf::Vector2f& mapsize, const sf::Vector2u& gridsize)
        : _window{window},
          _anchor{anchor},
          _mapSize{mapsize},
          _gridSize{gridsize},
          _logger{ log::initializeLogger("TileManager") }
    {
        _logger->trace("Anchor: {},{}", anchor.x, anchor.y);
        _logger->trace("MapSize: {},{}", mapsize.x, mapsize.y);
        _logger->trace("GridSize: {},{}", gridsize.x, gridsize.y);
        const auto tileWidth = mapsize.x / gridsize.x;
        const auto tileHeight = mapsize.y / gridsize.y;
        const sf::Vector2f tileSize{ tileWidth, tileHeight };

        for (auto xidx = 0u; xidx < gridsize.x; xidx++)
        {
            _tileContainer.push_back(std::vector<std::shared_ptr<Tile>>{});
            float x = _anchor.x + (xidx * tileWidth);
            auto& back = _tileContainer.back();
            for (auto yidx = 0u; yidx < gridsize.y; yidx++)
            {
                float y = _anchor.y + (yidx * tileHeight);
                const sf::Vector2 loc{x, y};
                auto tile = std::make_shared<Tile>(loc, tileSize);
                back.push_back(tile);
            }
        }
    }
    
    void draw()
    {
        for (const auto& row : _tileContainer)
        {
            for (const auto& tile : row)
            {
                _window.draw(*tile);
            }
        }
    }
    
    void event(const sf::Event& e)
    {
        
    }
    
private:
    sf::RenderTarget&   _window;
    
    sf::Vector2f        _anchor;
    sf::Vector2f        _mapSize;
    sf::Vector2u        _gridSize;
    
    using TileContainer = std::vector<std::vector<std::shared_ptr<Tile>>>;
    TileContainer       _tileContainer;
    
    log::SpdLogPtr      _logger;
};

class GameScreen : public Screen
{
    
public:
    GameScreen(sf::RenderTarget& target, ResourceManager& resources);
    
    void draw() override;
    PollResult poll(const sf::Event&) override;
    
private:
    std::unique_ptr<TileManager>    _tiles;
};

} // namespace gs
