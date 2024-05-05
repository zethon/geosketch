#pragma once

#include "GameController.h"

namespace gs
{

class GameStartController: public GameController
{
public:
    static constexpr auto ctrlrname = "GameStartController";

    GameStartController(const GameControllerConfig& config);
    PollResult update() override;
    PollResult poll(const sf::Event&) override;
    void draw() override;

    void start() override;

private:
    std::uint8_t _countdown = 4;
    chrono::time_point<chrono::steady_clock> _start = chrono::steady_clock::now();
};

} // namespace gs
