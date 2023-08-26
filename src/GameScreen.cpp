#include "GameScreen.h"

namespace gs
{

GameScreen::GameScreen(sf::RenderTarget& target, ResourceManager& resources)
    : Screen(target, resources)
{
    const auto winsize = _target.getSize();
    const auto winwidth = winsize.x;
    const auto winheight = winsize.y;

    const auto edgelen = static_cast<std::float_t>((winwidth > winheight ? winheight : winwidth) * 0.925);
    const auto xloc = (winwidth - (edgelen + (edgelen * 0.05f)));
    const auto yloc = (winheight / 2.f) - (edgelen / 2.f);
    
    sf::Vector2f anchor{xloc, yloc};
    sf::Vector2f mapsize{edgelen, edgelen};
    
    constexpr auto griddle = 100;
    sf::Vector2u gridsize{griddle, griddle};
    
    _tiles = std::make_unique<TileManager>(_target, anchor, mapsize, gridsize);
    auto outline = this->emplaceDrawable<sf::RectangleShape>(
        sf::Vector2f{static_cast<std::float_t>(edgelen), static_cast<std::float_t>(edgelen)});
    outline->setPosition(xloc, yloc);
    outline->setFillColor(sf::Color::Transparent);
    outline->setOutlineThickness(5.0f);
    outline->setOutlineColor(sf::Color(122, 122, 122));
    
    this->initGuit();
    
    this->_tiles->setSelecting(true);
}

void GameScreen::initGuit()
{
    const auto anchor = _tiles->anchor();
    _drawbtn_text = *(_resources.load<sf::Texture>("textures/draw-normal.png"));
    
    auto drawbtn = tgui::Button::create();
    drawbtn->setWidgetName("drawbtn");
    drawbtn->setSize(100, 100);
    drawbtn->setPosition(anchor.x - (drawbtn->getSize().x + 20), anchor.y);
    drawbtn->onPress([&]
    {
        this->_tiles->setSelecting(true);
    });
    drawbtn->getRenderer()->setTexture(_drawbtn_text);
    drawbtn->getRenderer()->setBackgroundColor(sf::Color::White);
    _gui->add(drawbtn);
    
    auto erasebtn = tgui::Button::create();
    erasebtn->setWidgetName("erasebtn");
    erasebtn->setPosition({"drawbtn.left", "drawbtn.bottom + 20"});
    erasebtn->setSize(100, 100);
    erasebtn->setText("Erase");
    erasebtn->onPress([&]
    {
        this->_tiles->setSelecting(false);
    });
    _gui->add(erasebtn);
    
    auto clearbtn = tgui::Button::create();
    clearbtn->setWidgetName("clearbtn");
    clearbtn->setPosition({"erasebtn.left", "erasebtn.bottom + 20"});
    clearbtn->setSize(100, 100);
    clearbtn->setText("Clear");
    clearbtn->onPress([&]
    {
        this->_tiles->clear();
        this->_tiles->setSelecting(true);
    });
    _gui->add(clearbtn);
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
