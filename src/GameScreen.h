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
    void initGuit();
    
    std::unique_ptr<TileManager>    _tiles;
    sf::Texture                     _drawbtn_text;
};

} // namespace gs
