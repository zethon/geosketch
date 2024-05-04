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

    this->_controller = gs::createGameController(settings.gameType);

    std::stringstream ss;
    ss << "GameScreen initialized with settings: " << settings;
    _logger->debug(ss.str());
}

void GameScreen::initGuit()
{
    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _timer->setText("00:00.00");
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _timer->setPosition(_tiles->anchor().x - ((_tiles->anchor().x / 2) + (_timer->getSize().x / 2)), 1);
    _gui->add(_timer);

    auto underline = this->emplaceDrawable<sf::RectangleShape>(sf::Vector2f{static_cast<std::float_t>(_timer->getSize().x + 50), 5.0f});
    underline->setPosition(_timer->getPosition().x - 25, _timer->getPosition().y + _timer->getSize().y + 10);

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
                return {};
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
