#include "Screen.h"

namespace gs
{

class SplashScreen : public Screen
{

public:
    SplashScreen(RenderTargetPtr target);

    void draw() override;
    PollResult poll(const sf::Event&) override;
    PollResult update() override;

private:
    sf::Clock   _clock;

};

} // namespace gs