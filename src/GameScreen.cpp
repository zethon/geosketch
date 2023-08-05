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
    
    constexpr auto griddle = 7u;
    sf::Vector2u gridsize{griddle, griddle};
    
    _tiles = std::make_unique<TileManager>(_target, anchor, mapsize, gridsize);
//    constexpr auto RIGHT_BUFFER = 50u;
//    constexpr auto LEFT_BUFFER = 100u;
//    constexpr auto TOP_BUFFER = 20u;
//    constexpr auto BOTTOM_BUFFER = 20u;
//    
//    const auto winsize = _target.getSize();
//    const auto winwidth = winsize.x;
//    const auto winheight = winsize.y;
//    
//    const auto edgelen = (winwidth > winheight ? winheight : winwidth) * 0.95;
    
    auto outline = this->emplaceDrawable<sf::RectangleShape>(
        sf::Vector2f{static_cast<std::float_t>(edgelen), static_cast<std::float_t>(edgelen)});
//    
//    const auto xloc = (winwidth - (edgelen + (edgelen * 0.05)));
//    const auto yloc = (winheight / 2) - (edgelen / 2);
    outline->setPosition(xloc, yloc);
    outline->setFillColor(sf::Color::Transparent);
    outline->setOutlineThickness(5.0f);
    outline->setOutlineColor(sf::Color(122, 122, 122));
//    
//    constexpr auto griddle = 100u;
//    const auto step = edgelen / griddle;
//    for (auto x = step; x < edgelen; x+=step)
//    {
//        auto line = this->emplaceDrawable<sf::RectangleShape>(
//            sf::Vector2f{static_cast<std::float_t>(1), static_cast<std::float_t>(edgelen)});
//        line->setPosition(xloc+x, yloc);
//        line->setFillColor(sf::Color{50,50,50});
//    }
//    
//    for (auto y = step; y < edgelen; y+=step)
//    {
//        auto line = this->emplaceDrawable<sf::RectangleShape>(
//            sf::Vector2f{static_cast<std::float_t>(edgelen), static_cast<std::float_t>(1)});
//        line->setPosition(xloc, yloc+y);
//        line->setFillColor(sf::Color{50,50,50});
//    }
}

PollResult GameScreen::poll(const sf::Event&)
{
    return {};
}

void GameScreen::draw()
{
    Screen::draw();
    _tiles->draw();
}

} // namespace gs
