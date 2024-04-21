#include "SettingsScreen.h"
#include "AudioService.h"

namespace gs
{

constexpr auto SPLASH_SCREEN_DURATION = 2.5f; // seconds

SettingsScreen::SettingsScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen{ target, resources },
    _logger{ log::initializeLogger("SettingsScreen") }
{

}

PollResult SettingsScreen::poll(const sf::Event& e)
{
    return {};
}

PollResult SettingsScreen::update()
{
    return {};
}

} // namespace gs
