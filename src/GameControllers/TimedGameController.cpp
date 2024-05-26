#include <numeric>

#include "../GameScreen.h"
#include "TimedGameController.h"

namespace gs
{

using StringList = std::vector<std::string>;
StringList DATA = { "Bolivia", "USA", "Canada", "Mexico" };
// StringList DATA = { "Bolivia", "USA", "Canada", "Mexico", "Brazil", "Argentina", "Chile", "Peru", "Colombia", "Venezuela", "Ecuador", "Uruguay", "Paraguay" };

RegionDB::RegionDB(const std::vector<std::string>& data)
{
    for (const auto& region : data)
    {
        _countries.emplace_back(std::make_shared<gs::Country>(region));
    }
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

    if (_finished)
    {
        auto goc = std::static_pointer_cast<TimedGameOverController>(this->nextController());
        goc->setScores(_scores);

        PollResult result{ ActionType::CHANGE_GAME_STATE };
        return result;
    }

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
                    _timeron = false;
                    _finished = true;
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

// @TODO: Clean up this start-round/controller/game mess
void TimedGameController::startController()
{
    this->_screen->setVisible(true);
    auto region = _countrydb->current();
    assert(region);
    this->setRegionName(region->name());
    _tiles.setCanDraw(true);
    _timeron = true;
    this->startRound();
}

void TimedGameController::endController()
{
    _tiles.setCanDraw(false);
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

TimedGameOverController::TimedGameOverController(const GameControllerConfig& config)
    : GameController{ config, ctrlrname }
{
    auto label = tgui::Label::create();
    label->setWidgetName("gameoverlbl");
    label->setText("Game Over");
    label->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.1));
    label->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc = (_target.getView().getSize().x / 2) - (label->getSize().x / 2);
    const auto yloc = (_target.getView().getSize().y / 3) - (label->getSize().y / 2);
    label->setPosition(xloc, yloc);
    _gui->add(label);

    auto scorelabel = tgui::Label::create();
    scorelabel->setWidgetName("scorelbl");
    scorelabel->setText("Score: 0");
    scorelabel->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.05));
    scorelabel->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc2 = (_target.getView().getSize().x / 2) - (scorelabel->getSize().x / 2);
    const auto yloc2 = yloc + label->getSize().y + 20;
    scorelabel->setPosition(xloc2, yloc2);
    _gui->add(scorelabel);

    auto timelabel = tgui::Label::create();
    timelabel->setWidgetName("timelbl");
    timelabel->setText("Time: 00:00.00");
    timelabel->setTextSize(static_cast<std::uint32_t>(_target.getView().getSize().x * 0.05));
    timelabel->getRenderer()->setTextColor(sf::Color::White);
    const auto xloc3 = (_target.getView().getSize().x / 2) - (timelabel->getSize().x / 2);
    const auto yloc3 = yloc2 + scorelabel->getSize().y + 20;
    timelabel->setPosition(xloc3, yloc3);
    _gui->add(timelabel);
}

PollResult TimedGameOverController::update()
{
    return {};
}

PollResult TimedGameOverController::poll(const sf::Event&  e) 
{ 
    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
            default:
            break;

            case sf::Keyboard::Escape:
            {
                gs::PollResult result;
                result.type = gs::ActionType::CHANGE_SCREEN;
                result.data = SCREEN_GAMESELECT;
                return result;
            }
            break;
        }
    }

    return {}; 
}

void TimedGameOverController::draw() 
{ 
    drawDrawables();
    drawGui();
}

void TimedGameOverController::startController()
{
    this->_screen->setVisible(false);

    auto totalMili = std::accumulate(_scores.begin(), _scores.end(), 0u, 
        [](auto acc, const auto& score) 
        { 
            return acc + static_cast<std::uint32_t>(score.elapsed.count()); 
        });

    const auto minutes = totalMili / 60000;
    const auto seconds = (totalMili % 60000) / 1000;
    const auto tenths = (totalMili % 1000) / 10;
    auto timelabel = _gui->get<tgui::Label>("timelbl");
    timelabel->setText(fmt::format("Time: {:02d}:{:02d}.{}", minutes, seconds, tenths));

    auto totalScore = std::accumulate(_scores.begin(), _scores.end(), 0, 
               [](auto acc, const auto& score) { return acc + score.score; });

    auto scorelabel = _gui->get<tgui::Label>("scorelbl");
    scorelabel->setText(fmt::format("Score: {}", totalScore));
}

void TimedGameOverController::endController()
{
}

}  // namespace gs
