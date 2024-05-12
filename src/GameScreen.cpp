#include "GameScreen.h"

namespace gs
{

GameScreen::GameScreen(sf::RenderTarget& target, ResourceManager& resources, const NewGameSettings& settings)
    : Screen(target, resources), 
      _settings{settings}, 
      _logger{ log::initializeLogger("GameScreen") }
{
    const auto winsize = _target.getSize();
    const auto winwidth = winsize.x;
    const auto winheight = winsize.y;

    // const auto edgelen = static_cast<std::float_t>((winwidth > winheight ? winheight : winwidth) * 0.925);
    const auto edgelen = static_cast<std::float_t>(winheight * 0.9);
    const auto xloc = (winwidth - (edgelen + (edgelen * 0.05f)));
    const auto yloc = winheight * 0.025f;

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

    this->_controller = gs::createGameController({target, resources, settings, this});

    std::stringstream ss;
    ss << "GameScreen initialized with settings: " << settings;
    _logger->debug(ss.str());
}

void GameScreen::initGuit()
{
    auto label = tgui::Label::create();
    label->setWidgetName("label");
    label->setText("Press Escape to clear   Press Space to submit");
    label->setTextSize(static_cast<std::uint32_t>(_target.getSize().x * 0.0125));
    label->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = (_tiles->anchor().x + (_tiles->mapSize().x / 2)) - (label->getSize().x / 2);
    const auto yloc = (_tiles->anchor().y + _tiles->mapSize().y) + ((_target.getSize().y - (_tiles->anchor().y + _tiles->mapSize().y)) / 2);
    label->setPosition(xloc, yloc - (label->getSize().y / 2));
    _gui->add(label);
}

PollResult GameScreen::poll(const sf::Event& e)
{
    if (const auto result = Screen::poll(e);
        result.type != gs::ActionType::NONE)
    {
        return result;
    }

    // TODO: will controller handle this event?
    if (_controller && _controller->poll(e).type != gs::ActionType::NONE)
    {
        return _controller->poll(e);
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
                return {};
            }
            break;

            case sf::Keyboard::Num7: // fall through
            case sf::Keyboard::F7:
            {
                gs::PollResult result;
                result.type = gs::ActionType::CHANGE_SCREEN;
                result.data = SCREEN_GAMESELECT;
                return result;
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
    if (!_started)
    {
        _controller->startController();
        _started = true;
    }

    if (_controller)
    {
        if (const auto res = _controller->update(); 
            res.type == ActionType::CHANGE_GAME_STATE)
        {
            _controller = _controller->nextController();
            assert(_controller);
            _controller->startController();
        }
    }

    return {};
}

void GameScreen::draw()
{
    Screen::draw();
    if (this->isVisible()) _tiles->draw();
    if (_controller) _controller->draw();
}

} // namespace gs
