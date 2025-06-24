#include "state.h"
#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;

State::State(State *parent, const char *name)
{
    this->name = name;
}

void State::enter()
{
    PLOG_INFO << name << " state enter method is not defined.";
}

void State::exit()
{
    PLOG_INFO << name << " state exit method is not defined.";
}

void State::goTo(State *nextState)
{
    Core::instance().goTo(nextState);
}

void State::handle(Events::TimeOut &ev)
{
    PLOG_INFO << name << " state handle(Events::TimeOut &) method is not defined.";
}