#pragma once
#include "GameController.h"

namespace gs
{

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
    chrono::time_point<std::chrono::steady_clock> _start2;
    tgui::Label::Ptr    _timer;
    bool                _timeron { false };
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
