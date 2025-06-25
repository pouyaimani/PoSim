#include "event.h"
#include "state.h"
#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;
using namespace Events;

void Events::Event::dispatch(StateShPtr state)
{
    auto ev = EvUnqPtr(clone());
    ev->targetState = state;
    Core::instance().raiseEvent(std::move(ev));
}
