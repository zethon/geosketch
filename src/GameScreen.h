#pragma once

#include "Screen.h"
#include "GeoSketchLogger.h"
#include <cassert>

namespace gs
{

class Tile : public sf::Drawable
{
    const sf::Color DEFAULT{sf::Color::Transparent};
    const sf::Color HIGHLIGHT{sf::Color{150,150,150}};
    const sf::Color SELECTED{sf::Color{0,100,0}};
    
public:
    enum class State
    {
        DEFAULT, HIGHLIGHT, SELECTED
    };
    
    Tile(const sf::Vector2f& location, const sf::Vector2f& size)
        : _color{sf::Color::Transparent}
    {
        _rect.setPosition(location.x, location.y);
        _rect.setSize(size);
        _rect.setFillColor(_color);
        _rect.setOutlineColor(sf::Color(122, 122, 122));
        _rect.setOutlineThickness(0.5f);
    }
    
    void setState(Tile::State state)
    {
        switch (state)
        {
            default:
                _rect.setFillColor(DEFAULT);
            break;
                
            case State::HIGHLIGHT:
                _rect.setFillColor(HIGHLIGHT);
            break;
                
            case State::SELECTED:
                _rect.setFillColor(SELECTED);
            break;
        }
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
          _logger{ log::initializeLogger("TileManager") },
          _xmin{_anchor.x},
          _xmax{_anchor.x + _mapSize.x},
          _ymin{_anchor.y},
          _ymax{_anchor.y + _mapSize.y},
          _tileWidth{_mapSize.x / _gridSize.x},
          _tileHeight{_mapSize.y / _gridSize.y}
    {
        _logger->trace("Anchor: {},{}", anchor.x, anchor.y);
        _logger->trace("MapSize: {},{}", mapsize.x, mapsize.y);
        _logger->trace("GridSize: {},{}", gridsize.x, gridsize.y);

        const sf::Vector2f tileSize{ _tileWidth, _tileHeight };
        for (auto xidx = 0u; xidx < gridsize.x; xidx++)
        {
            _tileContainer.push_back(std::vector<std::shared_ptr<Tile>>{});
            float x = _anchor.x + (xidx * _tileWidth);
            auto& back = _tileContainer.back();
            for (auto yidx = 0u; yidx < gridsize.y; yidx++)
            {
                float y = _anchor.y + (yidx * _tileHeight);
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
    
    void event(const sf::Event& ev)
    {
        switch (ev.type)
        {
            default:
            break;
                
            case sf::Event::MouseMoved:
            {
                if (ev.mouseMove.x < _xmin || ev.mouseMove.x > _xmax || ev.mouseMove.y < _ymin || ev.mouseMove.y > _ymax) return;

                const auto local = this->getXYCords(sf::Vector2i{ev.mouseMove.x, ev.mouseMove.y});
                if (auto ptr = _tileContainer[local.x][local.y]; _lastTile.get() != ptr.get())
                {
                    if (_lastTile != nullptr)
                    {
                        _lastTile->setState(Tile::State::DEFAULT);
                    }

                    _tileContainer[local.x][local.y]->setState(Tile::State::HIGHLIGHT);
                    _lastTile = _tileContainer[local.x][local.y];
                    _logger->trace("mouse: {}, {}", ev.mouseMove.x, ev.mouseMove.y);
                    _logger->trace("tile: {},{}", local.x, local.y);
                }
            }
            break;
        }
    }
    
private:
    sf::Vector2u getXYCords(const sf::Vector2i& mouseCord)
    {
        const auto localX = std::max(0.0f, mouseCord.x - _anchor.x);
        const auto localY = std::max(0.0f, mouseCord.y - _anchor.y);
        
        const std::uint32_t xcord = std::min(static_cast<std::uint32_t>(std::floor(localX / _tileWidth)), _gridSize.x - 1);
        const std::uint32_t ycord = std::min(static_cast<std::uint32_t>(std::floor(localY / _tileHeight)), _gridSize.y - 1);
        
        assert(xcord < _tileContainer.size());
        assert(ycord < _tileContainer[xcord].size());

        return {xcord, ycord};
    }
    
    sf::RenderTarget&   _window;

    sf::Vector2f        _anchor;
    sf::Vector2f        _mapSize;
    sf::Vector2u        _gridSize;
    
    using TileContainer = std::vector<std::vector<std::shared_ptr<Tile>>>;
    TileContainer       _tileContainer;
    
    log::SpdLogPtr      _logger;
    
    // const properties
    const std::float_t        _xmin;
    const std::float_t        _xmax;
    const std::float_t        _ymin;
    const std::float_t        _ymax;
    const std::float_t        _tileWidth;
    const std::float_t        _tileHeight;
    
    std::shared_ptr<Tile>     _lastTile = nullptr;
    
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
