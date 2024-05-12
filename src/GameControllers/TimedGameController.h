#pragma once
#include "../Data/RegionDatabase.h"
#include "GameController.h"

namespace gs
{ 

struct TiledScore
{
    using ElapsedTime = std::chrono::milliseconds;

    Region          region;
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

    void startController() override;
    void endController() override;

private:
    void updateTimer();
    void setRegionName(const std::string& name);

    void startRound();
    void endRound();

    OptionalTimePoint _start;
    SteadyClock       _roundStart;
    
    tgui::Label::Ptr    _timer;
    bool                _timeron { false };
    tgui::Label::Ptr    _country_name;
    RegionDBPtr        _countrydb;

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
