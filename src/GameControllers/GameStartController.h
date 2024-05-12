#pragma once

#include "GameController.h"

namespace gs
{
    
class GameStartController: public GameController
{
public:
    static constexpr auto ctrlrname = "GameStartController";
    static constexpr auto COUNTDOWN = 3;

    GameStartController(const GameControllerConfig& config);
    PollResult update() override;
    PollResult poll(const sf::Event&) override;
    void draw() override;

    void startController() override;

private:
    std::int8_t     _countdown { COUNTDOWN };
    std::uint32_t   _stepdelay { 1000 };

    chrono::time_point<chrono::steady_clock> _start = chrono::steady_clock::now();
};

} // namespace gs
