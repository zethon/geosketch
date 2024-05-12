#include "../GameScreen.h"
#include "TimedGameController.h"

namespace gs
{

using StringList = std::vector<std::string>;
StringList DATA = { "Bolivia1111111111", "US2", "Canada333", "Mexico444" };
// StringList DATA = { "Bolivia", "USA", "Canada", "Mexico", "Brazil", "Argentina", "Chile", "Peru", "Colombia", "Venezuela", "Ecuador", "Uruguay", "Paraguay" };

RegionDB::RegionDB(const std::vector<std::string>& data)
{
    for (const auto& region : data)
    {
        _countries.emplace_back(region);
    }
}

void RegionDB::addRegion(const std::string& name)
{
    _countries.emplace_back(name);
}

TimedGameController::TimedGameController(const GameControllerConfig& config)
    : GameController{config, ctrlrname}, _tiles{config.screen->tilemanager()}
{
    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _timer->setText("00:00.00");
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _timer->setPosition(_tiles.anchor().x - ((_tiles.anchor().x / 2) + (_timer->getSize().x / 2)), 1);
    _gui->add(_timer);

    auto underline = std::make_shared<sf::RectangleShape>(sf::Vector2f{static_cast<std::float_t>(_timer->getSize().x + 50), 5.0f});
    _drawables.push_back(underline);
    underline->setPosition(_timer->getPosition().x - 25, _timer->getPosition().y + _timer->getSize().y + 10);

    _country_name = tgui::Label::create();
    _country_name->setWidgetName("countrylbl");
    _country_name->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _country_name->setText("Region Name");
    _country_name->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = _tiles.anchor().x - ((_tiles.anchor().x / 2) + (_country_name->getSize().x / 2));
    const auto yloc = _timer->getPosition().y + _timer->getSize().y + 20;
    _country_name->setPosition(xloc, yloc);
    _gui->add(_country_name);

    _countrydb = std::make_unique<RegionDB>(DATA);
}

void TimedGameController::updateTimer()
{
    if (!_start.has_value())
    {
        _start = std::chrono::steady_clock::now();
    }

    if (!_timeron) return;
    const auto timespace = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start.value());
    const auto minutes = timespace.count() / 60000;
    const auto seconds = (timespace.count() % 60000) / 1000;
    const auto tenths = (timespace.count() % 1000) / 10;
    _timer->setText(fmt::format("{:02d}:{:02d}.{}", minutes, seconds, tenths));
}

PollResult TimedGameController::update()
{
    updateTimer();
    return {};
}

PollResult TimedGameController::poll(const sf::Event& e)
{ 
    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
            default:
            break;

            case sf::Keyboard::Space:
            {
                this->endRound();
               
                auto region = _countrydb->next();
                if (nullptr != region)
                {
                    this->startRound();
                    setRegionName(region->name());
                }
                else
                {
                    setRegionName("DONE!");
                    _timeron = false;
                }
            }
            break;
        }
    }

    return {}; 
}

void TimedGameController::draw()
{ 
    drawDrawables();
    drawGui();
}

void TimedGameController::start()
{
    this->_screen->setVisible(true);
    auto region = _countrydb->current();
    assert(region);
    this->setRegionName(region->name());
    _timeron = true;

    this->startRound();
}

void TimedGameController::setRegionName(const std::string& name)
{
    _country_name->setText(name);
    const auto xloc = _tiles.anchor().x - ((_tiles.anchor().x / 2) + (_country_name->getSize().x / 2));
    const auto yloc = _timer->getPosition().y + _timer->getSize().y + 20;
    _country_name->setPosition(xloc, yloc);
}

void TimedGameController::startRound()
{
    _roundStart = std::chrono::steady_clock::now();
}

void TimedGameController::endRound()
{
    auto curp = _countrydb->current();
    if (nullptr == curp) return;

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _roundStart);

    const auto& tiles = _tiles.tiles();
    auto totalSelected =  0u;

    for (const auto& row : tiles)
    {
        for (const auto& tile : row)
        {
            if (tile->selected())
            {
                totalSelected++;
            }
        }
    }

    auto& newscore = _scores.emplace_back(TiledScore{*(curp), totalSelected, elapsed});
    _tiles.clear(); 

    _logger->debug("SCORE - Region: {}, Score: {}, Elapsed: {}", newscore.region.name(), newscore.score, newscore.elapsed.count());
}

}  // namespace gs
