#include "GameScreen.h"

namespace gs
{

GameScreen::GameScreen(sf::RenderTarget& target, ResourceManager& resources, const NewGameSettings& settings)
    : Screen(target, resources), 
      _settings{settings}, 
      _logger{ log::initializeLogger("GameScreen") }
{
    constexpr auto top_buffer = 75.0f;
    // constexpr auto bottom_buffer = 20.0f;

    const auto winsize = _target.getSize();
    const auto winwidth = winsize.x;
    const auto winheight = winsize.y;

    // const auto edgelen = static_cast<std::float_t>((winwidth > winheight ? winheight : winwidth) * 0.925);
    const auto edgelen = static_cast<std::float_t>(winheight * 0.9);
    const auto xloc = (winwidth - (edgelen + (edgelen * 0.05f)));
    // const auto yloc = (winheight / 2.f) - (edgelen / 2.f);
    const auto yloc = top_buffer;
    
    sf::Vector2f anchor{xloc, yloc};
    sf::Vector2f mapsize{edgelen, edgelen};
    
    std::uint32_t griddle = 0;
    auto outlineSize = 0.5f;
    switch (settings.difficulty)
    {
        default:
            griddle = 50;
        break;

        case NewGameSettings::Difficulty::MEDIUM:
            griddle = 75;
        break;

        case NewGameSettings::Difficulty::HARD:
            griddle = 100;
        break;

        case NewGameSettings::Difficulty::EXPERT:
            griddle = 100;
            outlineSize = 0.0f;
        break;
    }
    
    sf::Vector2u grid_dimensions{griddle, griddle};
    
    _tiles = std::make_unique<TileManager>(_target, anchor, mapsize, grid_dimensions, outlineSize);

    auto outline = this->emplaceDrawable<sf::RectangleShape>(
        sf::Vector2f{static_cast<std::float_t>(edgelen), static_cast<std::float_t>(edgelen)});
    outline->setPosition(xloc, yloc);
    outline->setFillColor(sf::Color::Transparent);
    outline->setOutlineThickness(5.0f);
    outline->setOutlineColor(sf::Color(122, 122, 122));
    
    this->initGuit();
    this->_tiles->setSelecting(true);

    std::stringstream ss;
    ss << "GameScreen initialized with settings: " << settings;
    _logger->debug(ss.str());
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

    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setPosition(1, 1);
    _timer->setTextSize(124);
    _timer->setText("00:00.00");
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _gui->add(_timer);

    auto label = tgui::Label::create();
    label->setWidgetName("label");
    // label->setPosition({"drawbtn.right + 20", "drawbtn.top - 48"});
    label->setText("Press Escape to clear | Press Space to submit");
    label->setTextSize(32);
    label->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = _tiles->anchor().x + (label->getSize().x / 2);
    const auto yloc = _tiles->anchor().y + _tiles->mapSize().y + 20;
    label->setPosition(xloc, yloc);
    _gui->add(label);
}

PollResult GameScreen::poll(const sf::Event& e)
{
    if (const auto result = Screen::poll(e);
        result.type != gs::ActionType::NONE)
    {
        return result;
    }

    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
            default:
            break;

            case sf::Keyboard::Escape:
            {
                this->_tiles->clear();
                this->_tiles->setSelecting(true);
            }
            break;

            case sf::Keyboard::Num9:
            {
                gs::PollResult result;
                result.type = gs::ActionType::CHANGE_SCREEN;
                result.data = SCREEN_GAMESELECT;
                return result;
            }
            break;

            case sf::Keyboard::Space:
            {
                if (_timeron)
                {
                    _timeron = false;
                }
                else
                {
                    _timeron = true;
                    _start = std::chrono::steady_clock::now();
                }
            }
            break;
        }
    }

    _tiles->event(e);
    return {};
}

PollResult GameScreen::update()
{
    Screen::update();

    if (_timeron)
    {
        const auto timespace = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start);
        const auto minutes = timespace.count() / 60000;
        const auto seconds = (timespace.count() % 60000) / 1000;
        const auto tenths = (timespace.count() % 1000) / 10;
        _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));
    }
    return {};
}

void GameScreen::draw()
{
    Screen::draw();
    _tiles->draw();
}

} // namespace gs
