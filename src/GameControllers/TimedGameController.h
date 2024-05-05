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

class TimedGameController : public GameController
{
public:
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

    using OptionalTimePoint = std::optional<std::chrono::time_point<std::chrono::steady_clock>>;
    OptionalTimePoint _start;
    
    tgui::Label::Ptr    _timer;
    bool                _timeron { false };
    tgui::Label::Ptr    _country_name;
    CountryDBPtr        _countrydb;
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
