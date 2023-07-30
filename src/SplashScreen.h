#include "Screen.h"

namespace gs
{

class SplashScreen : public Screen
{
    inline static constexpr auto INTRO_LOGO = "audio/Dog-Finger-Studios.wav";

public:
    SplashScreen(sf::RenderTarget& target, ResourceManager& resources);

    PollResult poll(const sf::Event&) override;
    PollResult update() override;

private:
    sf::Clock   _clock;
    sf::Texture _bg;
    sf::Font    _font;

};

} // namespace gs
