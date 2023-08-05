#pragma once

#include "Screen.h"
#include "TileManager.h"

namespace gs
{

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
