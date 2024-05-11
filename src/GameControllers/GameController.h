#pragma once
#include <cstdint>
#include <memory>

#include <boost/describe/class.hpp>

#include "../GameEngine.h"
#include "../PollResult.h"
#include "../GeoSketchLogger.h"

namespace chrono = std::chrono;

namespace gs
{

class GameController;
using GameControllerPtr = std::shared_ptr<GameController>;
using GameMode = NewGameSettings::GameType;

class GameScreen;

struct GameControllerConfig
{
    sf::RenderTarget& target;
    ResourceManager& resources;
    NewGameSettings settings;
    GameScreen* screen = nullptr;
};

class GameResults
{

};

class IGameResultCollector
{
public:

};

GameControllerPtr createGameController(const GameControllerConfig& config);

class GameController
{
public:
    GameController(const GameControllerConfig& config, const std::string& name)
        : _target{ config.target },
          _resources{ config.resources },
          _settings{ config.settings },
          _screen{ config.screen },
          _gui{ std::make_unique<tgui::Gui>(config.target) },
          _logger{ log::initializeLogger(name) }
    {
        // nothing to do
    }
    virtual ~GameController() = default;

    virtual PollResult update() = 0;
    virtual PollResult poll(const sf::Event&) = 0;
    virtual void draw() = 0;
    
    virtual void start() {}

    GameControllerPtr nextController() const;
    GameControllerPtr setNextController(GameControllerPtr nextController);

    void drawDrawables();
    void drawGui();

protected:
    sf::RenderTarget& _target;
    ResourceManager& _resources;
    NewGameSettings _settings;
    GameScreen* _screen = nullptr;
    std::unique_ptr<tgui::Gui> _gui;
    log::SpdLogPtr _logger;

    std::vector<DrawablePtr> _drawables;

private:
    GameControllerPtr _nextController;
};

class NullGameController : public GameController
{
public:
    static constexpr auto ctrlrname = "NullGameController";

    NullGameController(const GameControllerConfig& config)
        : GameController{config, ctrlrname}
    {
        // nothing to do
    }

    PollResult update() override { return {}; }
    PollResult poll(const sf::Event&) override { return {}; }
    void draw() override { };
};


// ************************************************************************************************

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

// ************************************************************************************************

class FreeGameController : public GameController
{
public:
    static constexpr auto ctrlrname = "FreeGameController";

    FreeGameController(const GameControllerConfig& config)
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

class FreeGameOverController : public GameController
{
public:
    static constexpr auto ctrlrname = "FreeGameOverController";

    FreeGameOverController(const GameControllerConfig& config)
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

