#pragma once
#include <cstdint>
#include <memory>

#include "GameEngine.h"
#include "PollResult.h"

namespace gs
{

class GameController;
using GameControllerPtr = std::shared_ptr<GameController>;

using GameMode = NewGameSettings::GameType;

GameControllerPtr createGameController(GameMode mode);

class GameController
{

public:
    virtual ~GameController() = default;

    virtual PollResult update() = 0;
    
private:

};

class GameStartController: public GameController
{
public:
    PollResult update() override;

    void setNextController(GameControllerPtr nextController);

    GameControllerPtr nextController() const;

private:
    GameControllerPtr _nextController;
};


class CountdownController : public GameController
{

public:
    enum class State
    {
        RUNNING,
        PAUSED,
        ENDED
    };

    CountdownController(std::uint32_t totalTime);

    std::uint32_t totalTime() const { return _totalTime; }

    PollResult update() override;

private:
    std::uint32_t _totalTime; // in seconds
    State _state { State::RUNNING };

};

class TimedGameController : public GameController
{
public:
    PollResult update() override
    {
        return {};
    }
};

class FreeGameController : public GameController
{
public:
    PollResult update() override
    {
        return {};
    }
};

} // namespace gs

