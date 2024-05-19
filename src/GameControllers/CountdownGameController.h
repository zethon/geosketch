#pragma once
#include "GameController.h"

namespace gs
{

class CountdownGameController : public GameController
{
public:
    constexpr static auto ctrlrname = "CountdownGameController";

    using SteadyClock = std::chrono::time_point<std::chrono::steady_clock>;
    using OptionalTimePoint = std::optional<SteadyClock>;

    CountdownGameController(const GameControllerConfig& config);
    ~CountdownGameController() override = default;

    PollResult update() override;

    PollResult poll(const sf::Event&) override;
    void draw() override;

    void startController() override;
    void endController() override;

private:
    void updateTimer();

    OptionalTimePoint _start;
    SteadyClock       _roundStart;

    tgui::Label::Ptr    _timer;
    bool                _timeron{ false };
    std::int32_t        _timeRemaingMili{ 1000 * 5 };
    bool                _finished{ false };


};

class CountdownGameOverController : public GameController
{
public:
    constexpr static auto ctrlrname = "CountdownGameOverController";

    CountdownGameOverController(const GameControllerConfig& config);
    ~CountdownGameOverController() override = default;

    PollResult update() override;

    PollResult poll(const sf::Event&) override;
    void draw() override;

    void startController() override;
    void endController() override;
};

} // namespace gs
