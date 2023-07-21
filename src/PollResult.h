#include <boost/any.hpp>

namespace gs
{

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
