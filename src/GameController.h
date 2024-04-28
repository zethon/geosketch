#pragma once
#include <cstdint>

#include "PollResult.h"

namespace gs
{

class GameController
{

public:
    virtual ~GameController() = default;

    virtual PollResult update() = 0;
    
private:

};

class CountdownController : public GameController
{

public:
    enum class State
    {
        STARTING,
        RUNNING,
        PAUSED
    };

    CountdownController(std::uint32_t totalTime);

    std::uint32_t totalTime() const { return _totalTime; }

    PollResult update() override;

private:
    std::uint32_t _totalTime; // in seconds
    State _state { State::STARTING };

};

} // namespace gs
