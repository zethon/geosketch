#pragma once

#include <boost/any.hpp>

namespace gs
{

constexpr std::uint16_t SCREEN_SPLASH = 100;
constexpr std::uint16_t SCREEN_SETTINGS = 150;
constexpr std::uint16_t SCREEN_MAINMENU = 200;
constexpr std::uint16_t SCREEN_LOADING = 300;
constexpr std::uint16_t SCREEN_GAMESELECT = 400;
constexpr std::uint16_t SCREEN_GAME = 500;
constexpr std::uint16_t SCREEN_GAMEOVER = 600;

enum class ActionType
{
    NONE = 0,
    EXIT_GAME,
    CHANGE_SCREEN,
    NEW_GAME,
    CHANGE_GAME_STATE
};

struct PollResult
{
    ActionType  type { ActionType::NONE };
    boost::any  data;
};

}
