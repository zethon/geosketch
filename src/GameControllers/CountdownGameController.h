#pragma once
#include "GameController.h"

namespace gs
{

class CountdownGameController : public GameController
{
public:
    constexpr static auto ctrlrname = "CountdownGameController";

    CountdownGameController(const GameControllerConfig& config)
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

class CountdownGameOverController : public GameController
{
public:
    constexpr static auto ctrlrname = "CountdownGameOverController";

    CountdownGameOverController(const GameControllerConfig& config)
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
