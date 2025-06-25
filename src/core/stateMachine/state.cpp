#include "state.h"
#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;

State::State(StateShPtr parent, const char *name)
{
    this->parent = parent;
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

void State::goTo(StateShPtr state)
{
    Core::instance().goTo(state);
}

void State::handle(Events::TimeOut &ev)
{
    PLOG_INFO << name << " state handle(Events::TimeOut &) method is not defined.";
}