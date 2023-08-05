#include "GameScreen.h"

namespace gs
{

GameScreen::GameScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen(target, resources)
{
    const auto winsize = _target.getSize();
    const auto winwidth = winsize.x;
    const auto winheight = winsize.y;

    const float edgelen = (winwidth > winheight ? winheight : winwidth) * 0.95;
    const float xloc = (winwidth - (edgelen + (edgelen * 0.05)));
    const float yloc = (winheight / 2) - (edgelen / 2);
    
    sf::Vector2f anchor{xloc, yloc};
    sf::Vector2f mapsize{edgelen, edgelen};
    
    constexpr auto griddle = 50u;
    sf::Vector2u gridsize{griddle, griddle};
    
    _tiles = std::make_unique<TileManager>(_target, anchor, mapsize, gridsize);
    auto outline = this->emplaceDrawable<sf::RectangleShape>(
        sf::Vector2f{static_cast<std::float_t>(edgelen), static_cast<std::float_t>(edgelen)});
    outline->setPosition(xloc, yloc);
    outline->setFillColor(sf::Color::Transparent);
    outline->setOutlineThickness(5.0f);
    outline->setOutlineColor(sf::Color(122, 122, 122));
}

PollResult GameScreen::poll(const sf::Event& e)
{
    if (const auto result = Screen::poll(e);
        result.type != gs::ActionType::NONE)
    {
        return result;
    }
    
    _tiles->event(e);
    return {};
}

void GameScreen::draw()
{
    Screen::draw();
    _tiles->draw();
}

} // namespace gs
