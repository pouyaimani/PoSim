#include "event.h"
#include "state.h"
#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;
using namespace Events;

void Events::Event::dispatch(State *state)
{
    auto ev = std::unique_ptr<Events::Event>(clone());
    ev->targetState = state;
    Core::instance().raiseEvent(std::move(ev));
}
