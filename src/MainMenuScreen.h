#pragma once

#include <SFML/Audio.hpp>

#include <TGUI/TGUI.hpp>

#include "Screen.h"

namespace gs
{

class MainMenuScreen : public Screen
{
    inline static constexpr auto THEME_SONG = "music/Theme-Song.wav";
    
public:
    MainMenuScreen(sf::RenderTarget& target, ResourceManager& resources);
    PollResult poll(const sf::Event&) override;
    
private:
    void initGui();

    sf::Texture _bg;
    
};

} // namespace gs
