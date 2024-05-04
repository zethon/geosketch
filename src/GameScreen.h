#pragma once

#include <memory>

#include "Screen.h"
#include "TileManager.h"
#include "GameEngine.h"
#include "GameController.h"

namespace gs
{

class GameScreen : public Screen
{
    
public:
    GameScreen(sf::RenderTarget& target, ResourceManager& resources, const NewGameSettings& settings);
    
    void draw() override;
    PollResult poll(const sf::Event&) override;
    PollResult update() override;

    const TileManager& tiles() const { return *_tiles; }
    
private:
    friend class GameController;

    void initGuit();
    
    std::unique_ptr<TileManager>    _tiles;
    sf::Texture                     _drawbtn_text;
    NewGameSettings                 _settings;
    log::SpdLogPtr                  _logger;

    GameControllerPtr                _controller;
};

} // namespace gs
