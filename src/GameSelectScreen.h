#include "GeoSketchLogger.h"
#include "Screen.h"
#include "GameEngine.h"

namespace gs
{

class GameSelectScreen : public Screen
{
    inline static constexpr auto INTRO_LOGO = "audio/Dog-Finger-Studios.wav";

public:
    GameSelectScreen(sf::RenderTarget& target, ResourceManager& resources);

    PollResult poll(const sf::Event&) override;
    PollResult update() override;

private:
    void initGui();
    NewGameSettings getNewGameSettings() const;

    sf::Clock   _clock;
    sf::Texture _bg;
    sf::Font    _font;
    log::SpdLogPtr  _logger;
};

} // namespace gs
