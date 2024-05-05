#include "../GameScreen.h"
#include "TimedGameController.h"

namespace gs
{

using StringList = std::vector<std::string>;
StringList DATA = { "Bolivia11", "USA222", "Canada333", "Mexico444" };
// StringList DATA = { "Bolivia", "USA", "Canada", "Mexico", "Brazil", "Argentina", "Chile", "Peru", "Colombia", "Venezuela", "Ecuador", "Uruguay", "Paraguay" };

CountryDB::CountryDB(const std::vector<std::string>& data)
{
    for (const auto& country : data)
    {
        _countries.emplace_back(country);
    }
}

void CountryDB::addCountry(const std::string& name)
{
    _countries.emplace_back(name);
}

TimedGameController::TimedGameController(const GameControllerConfig& config)
    : GameController{config, ctrlrname}
{
    auto& tiles = config.screen->tiles();

    _timer = tgui::Label::create();
    _timer->setWidgetName("timerlbl");
    _timer->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _timer->setText("00:00.00");
    _timer->getRenderer()->setTextColor(sf::Color::White);
    _timer->setPosition(tiles.anchor().x - ((tiles.anchor().x / 2) + (_timer->getSize().x / 2)), 1);
    _gui->add(_timer);

    auto underline = std::make_shared<sf::RectangleShape>(sf::Vector2f{static_cast<std::float_t>(_timer->getSize().x + 50), 5.0f});
    _drawables.push_back(underline);
    underline->setPosition(_timer->getPosition().x - 25, _timer->getPosition().y + _timer->getSize().y + 10);

    _country_name = tgui::Label::create();
    _country_name->setWidgetName("countrylbl");
    _country_name->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.04));
    _country_name->setText("Country Name");
    _country_name->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = tiles.anchor().x - ((tiles.anchor().x / 2) + (_country_name->getSize().x / 2));
    const auto yloc = _timer->getPosition().y + _timer->getSize().y + 20;
    _country_name->setPosition(xloc, yloc);
    _gui->add(_country_name);

    _countrydb = std::make_unique<CountryDB>(DATA);
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
                auto country = _countrydb->next();
                if (country)
                {
                    setCountryName(country->name());
                }
                else
                {
                    setCountryName("DONE!");
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
    auto country = _countrydb->next();
    assert(country);
    this->setCountryName(country->name());
    _timeron = true;
}

void TimedGameController::setCountryName(const std::string& name)
{
    auto& tiles = this->_screen->tiles();
    _country_name->setText(name);
    const auto xloc = tiles.anchor().x - ((tiles.anchor().x / 2) + (_country_name->getSize().x / 2));
    const auto yloc = _timer->getPosition().y + _timer->getSize().y + 20;
    _country_name->setPosition(xloc, yloc);
}

}  // namespace gs
