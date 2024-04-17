#include "TileManager.h"

namespace gs
{

Tile::Tile(const sf::Vector2f& location, const sf::Vector2f& size)
    : _color{sf::Color::Transparent}
{
    _rect.setPosition(location.x, location.y);
    _rect.setSize(size);
    _rect.setFillColor(_color);
    _rect.setOutlineColor(sf::Color(122, 122, 122));
    _rect.setOutlineThickness(0.5f);
}

void Tile::resetColor()
{
    if (_selected)
    {
        if (_hovered)
        {
            _rect.setFillColor(HISELECTED);
        }
        else
        {
            _rect.setFillColor(SELECTED);
        }
    }
    else
    {
        if (_hovered)
        {
            _rect.setFillColor(HIGHLIGHT);
        }
        else
        {
            _rect.setFillColor(DEFAULT);
        }
    }
}

TileManager::TileManager(sf::RenderTarget& window, const sf::Vector2f& anchor, const sf::Vector2f& mapsize, const sf::Vector2u& gridsize)
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

void TileManager::draw()
{
    for (const auto& row : _tileContainer)
    {
        for (const auto& tile : row)
        {
            _window.draw(*tile);
        }
    }
}

void TileManager::event(const sf::Event& ev)
{
    switch (ev.type)
    {
        default:
        break;
            
        case sf::Event::MouseButtonPressed:
        {
            _dragging = true;
//            if (ev.mouseButton.button == sf::Mouse::Left)
//            {
//                _selecting = true;
//            }
//            else
//            {
//                _selecting = false;
//            }
        }
        break;
            
        case sf::Event::MouseButtonReleased:
        {
            _dragging = false;
        }
        break;
            
        case sf::Event::MouseMoved:
        {
            const auto local = this->getXYCords(sf::Vector2i{ev.mouseMove.x, ev.mouseMove.y});
            if (!local.has_value())
            {
                if (_lastTile)
                {
                    _lastTile->setHovered(false);
                    _lastTile.reset();
                }
                return;
            }
            
            if (const auto ptr = _tileContainer[local->x][local->y];
                    _lastTile.get() != ptr.get())
            {
                if (_lastTile != nullptr)
                {
                    _lastTile->setHovered(false);
                }

                if (_dragging)
                {
                    _tileContainer[local->x][local->y]->setSelected(_selecting);
                }
                
                _tileContainer[local->x][local->y]->setHovered(true);
                _lastTile = _tileContainer[local->x][local->y];
                //_logger->trace("mouse: {}, {}", ev.mouseMove.x, ev.mouseMove.y);
                //_logger->trace("tile: {},{}", local->x, local->y);
            }
        }
        break;
    }
}

void TileManager::clear()
{
    for (auto& column : _tileContainer)
    {
        for (auto& row : column)
        {
            row->setSelected(false);
        }
    }
}

void TileManager::setSelecting(bool v)
{
    _selecting = v;
}

std::optional<sf::Vector2u> TileManager::getXYCords(const sf::Vector2i& mouseCord)
{
    if (mouseCord.x < _xmin || mouseCord.x > _xmax
        || mouseCord.y < _ymin || mouseCord.y > _ymax)
    {
        return {};
    }
    
    const auto localX = std::max(0.0f, mouseCord.x - _anchor.x);
    const auto localY = std::max(0.0f, mouseCord.y - _anchor.y);
    
    const std::uint32_t xcord = std::min(static_cast<std::uint32_t>(std::floor(localX / _tileWidth)), _gridSize.x - 1);
    const std::uint32_t ycord = std::min(static_cast<std::uint32_t>(std::floor(localY / _tileHeight)), _gridSize.y - 1);
    
    assert(xcord < _tileContainer.size());
    assert(ycord < _tileContainer[xcord].size());

    return sf::Vector2u{xcord, ycord};
}

} // namespace gs
