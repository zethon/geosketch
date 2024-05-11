#pragma once
#include "GameController.h"

namespace gs
{

class Country
{

public:
    Country(const std::string& name)
        : _name{name}
    {
        // nothing to do
    }

    std::string name() const { return _name; }

private:
    std::string _name;
};

class CountryDB
{
    
public:
    CountryDB(const std::vector<std::string>& data);

    void addCountry(const std::string& name);

    Country* current() 
    {
        if (_index >= _countries.size()) return nullptr;
        return &(_countries[_index]);
    }

    Country* next() 
    {
        _index++;
        if (_index > _countries.size()) return nullptr;
        return &(_countries[_index-1]);
    }

private:
    std::vector<Country> _countries;
    std::size_t _index { 0 };
};

using CountryDBPtr = std::unique_ptr<CountryDB>;

struct TiledScore
{
    using ElapsedTime = std::chrono::milliseconds;

    Country         country;
    std::uint32_t   score;
    ElapsedTime     elapsed;
};

using TiledScores = std::vector<TiledScore>;

class TimedGameController : public GameController
{
public:
    using SteadyClock = std::chrono::time_point<std::chrono::steady_clock>;
    using OptionalTimePoint = std::optional<SteadyClock>;

    static constexpr auto ctrlrname = "TimedGameController";

    TimedGameController(const GameControllerConfig& config);
    ~TimedGameController() = default;

    PollResult update() override;
    PollResult poll(const sf::Event&) override;
    void draw() override;

    void start() override;

private:
    void updateTimer();
    void setCountryName(const std::string& name);
    void calculateScore();

    void startRound();
    void endRound();

    OptionalTimePoint _start;
    SteadyClock       _roundStart;
    
    tgui::Label::Ptr    _timer;
    bool                _timeron { false };
    tgui::Label::Ptr    _country_name;
    CountryDBPtr        _countrydb;

    TileManager&        _tiles;
    TiledScores         _scores;
};

class TimedGameOverController : public GameController
{
public:
    static constexpr auto ctrlrname = "TimedGameOverController";

    TimedGameOverController(const GameControllerConfig& config)
        : GameController{config, ctrlrname}
    {
        // nothing to do
    }

    PollResult update() override
    {
        return {};
    }

    PollResult poll(const sf::Event&) override { return {}; }
    void draw() override { };
};

} // namespace gs
