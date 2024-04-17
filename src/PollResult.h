#pragma once

#include <boost/any.hpp>

namespace gs
{

constexpr std::uint16_t SCREEN_SPLASH = 10;
constexpr std::uint16_t SCREEN_SETTINGS = 15;
constexpr std::uint16_t SCREEN_MAINMENU = 20;
constexpr std::uint16_t SCREEN_LOADING = 30;
constexpr std::uint16_t SCREEN_GAME = 40;
constexpr std::uint16_t SCREEN_GAMEOVER = 50;

enum class ActionType
{
    NONE = 0,
    EXIT_GAME,
    CHANGE_SCREEN,
};

struct PollResult
{
    ActionType  type { ActionType::NONE };
    boost::any  data;
};

}
