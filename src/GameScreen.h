#pragma once

#include "Screen.h"

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
        : _window{window}, _anchor{anchor}, _mapSize{mapsize}, _gridSize{gridsize}
    {
        const auto tileWidth = mapsize.x / gridsize.x;
        const auto tileHeight = mapsize.y / gridsize.y;
        const sf::Vector2f tileSize{ tileWidth, tileHeight };
        std::size_t xidx = 0;
        std::size_t yidx = 0;
        for (auto x = _anchor.x; x < (_anchor.x + _mapSize.x); x += tileWidth)
        {
            _tileContainer.push_back(std::vector<std::shared_ptr<Tile>>{});
            auto& back = _tileContainer.back();
            yidx = 0;
            for (auto y = _anchor.y; y < (_anchor.y + _mapSize.y); y += tileHeight)
            {
                const sf::Vector2 loc{x, y};
                auto tile = std::make_shared<Tile>(loc, tileSize);
                if ((xidx+yidx) % 2 == 0) tile->setColor(sf::Color(122,50,0));
                back.push_back(tile);
                yidx++;
            }
            xidx++;
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
