#pragma once

#include <memory>

#include "Screen.h"
#include "TileManager.h"
#include "GameEngine.h"

namespace gs
{

class GameScreen : public Screen
{
    
public:
    GameScreen(sf::RenderTarget& target, ResourceManager& resources, const NewGameSettings& settings);
    
    void draw() override;
    PollResult poll(const sf::Event&) override;
    PollResult update() override;
    
private:
    void initGuit();
    
    std::unique_ptr<TileManager>    _tiles;
    sf::Texture                     _drawbtn_text;
    NewGameSettings                 _settings;
    log::SpdLogPtr                  _logger;
};

} // namespace gs
