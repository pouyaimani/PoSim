#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;
using namespace Events;

Core &Core::instance()
{
    static Core instance;
    return instance;
}

void Core::init(State *state)
{
    currentState = state;
    currentState->innerState = State::InnerState::ENTRY;
}

void Core::registerCb(Callback cb) noexcept
{
    std::lock_guard<std::recursive_mutex> lock(cbMtc);
    callback.push_back(std::move(cb));
}

void Core::exec()
{
    while (true) {
        runCycle();
        std::lock_guard<std::recursive_mutex> lock(cbMtc);
        for (auto& ch : callback) {
            ch();  // Call each registered callback
        }
    }
}

void Core::raiseEvent(std::unique_ptr<Events::Event> ev)
{
    ev->targetState = ev->targetState == nullptr ? currentState : ev->targetState;
    std::lock_guard<std::recursive_mutex> lock(qmtx);
    evq.push_back(std::move(ev));
    auto it = evq.begin();
}

void Core::runCycle()
{
    switch (currentState->innerState) {
    case State::InnerState::ENTRY:
        doEntryTask();
        break;
    case State::InnerState::HANDLE_EVENT:
        checkEvents();
        break;
    case State::InnerState::EXIT:
        doExitTask();
        break;
    default:
        break;
    }
}

void Core::checkEvents()
{
    std::lock_guard<std::recursive_mutex> lock(qmtx);
    for (auto it = evq.begin() ; it != evq.end();) {
        if ((*it)->targetState == currentState) {
            (*it)->dispatchTo(currentState);
            it = evq.erase(it);
        } else {
            it++;
        }
    }
}

void Core::goTo(State *state)
{
    currentState->innerState = State::InnerState::EXIT;
    nextState = state;
}

void Core::doEntryTask()
{
    currentState->enter();
    currentState->innerState = State::InnerState::HANDLE_EVENT;
}

void Core::doExitTask()
{
    currentState->exit();
    std::lock_guard<std::recursive_mutex> lock(qmtx);
    evq.erase(std::remove_if(evq.begin(), evq.end(), [this](std::unique_ptr<Events::Event> &ev) {
        return ev->targetState == this->currentState;
    }), evq.end());
    currentState->innerState = State::InnerState::ENTRY;
    currentState = nextState;
}