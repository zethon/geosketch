#include "GameController.h"

namespace gs
{

CountdownController::CountdownController(std::uint32_t totalTime)
    : _totalTime{totalTime}
{ 
    // nothing to do
}

PollResult CountdownController::update()
{
    return {};
}

} // namespace gs
