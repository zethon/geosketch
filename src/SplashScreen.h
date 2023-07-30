#include "Screen.h"

namespace gs
{

class SplashScreen : public Screen
{

public:
    SplashScreen(sf::RenderTarget& target, ResourceManager& resources);

    PollResult poll(const sf::Event&) override;
    PollResult update() override;

private:
    sf::Clock   _clock;
    sf::Texture _bg;

};

} // namespace gs
