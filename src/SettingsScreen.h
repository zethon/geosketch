#include "GeoSketchLogger.h"

#include "Screen.h"

namespace gs
{

class SettingsScreen : public Screen
{
    inline static constexpr auto INTRO_LOGO = "audio/Dog-Finger-Studios.wav";

public:
    SettingsScreen(sf::RenderTarget& target, ResourceManager& resources);

    PollResult poll(const sf::Event&) override;
    PollResult update() override;

private:
    sf::Clock   _clock;
    sf::Texture _bg;
    sf::Font    _font;

    log::SpdLogPtr  _logger;
};

} // namespace gs
