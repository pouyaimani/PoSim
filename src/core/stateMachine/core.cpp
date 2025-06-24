#include "core.h"
#include "plog/Log.h"

using namespace StateMachine;
using namespace Events;

Core &Core::instance()
{
    static Core instance;
    return instance;
}

/* Initializes the state machine with an initial state.
 * Should be called once before exec().
 */
void Core::init(State *state) noexcept
{
    currentState = state;
    currentState->innerState = State::InnerState::ENTRY;
}

void Core::registerCb(Callback cb) noexcept
{
    std::lock_guard<std::mutex> lock(cbMtc);
    callback.push_back(std::move(cb));
}

void Core::exec()
{
    while (true) {
        runCycle();
        /*
         * Copy callbacks then run them cause calling external 
         * functions (like cb()) while holding a mutex can lead to:
         * 
         * 1- Deadlocks: If a callback tries to:
         * Acquire the same cbMtc mutex again (e.g. it calls registerCb()),
         * or acquire another mutex that's being held elsewhere in the system,
         * you risk a deadlock.
         * 
         * 2- Reduced Concurrency
         * holding a mutex during the execution of arbitrary callbacks:
         * blocks other threads that want to register or update callbacks.
         * makes the callback system a performance bottleneck, especially if callbacks are long-running.
         * 
         * 3- Unclear Ownership and Debugging
         * it's hard to trace where a lock is being held.
         * errors or exceptions thrown inside cb() may leave the mutex in an inconsistent 
         * state (in older C++ versions without RAII or on custom mutex wrappers).
        */
        std::vector<Callback> cbs;
        {
            std::lock_guard<std::mutex> lock(cbMtc);
            cbs = callback; // safely copy the list
        }

        for (auto& cb : cbs) {
            cb();  // call without holding the lock
        }
    }
}

void Core::raiseEvent(std::unique_ptr<Events::Event> ev)
{
    ev->targetState = ev->targetState == nullptr ? currentState : ev->targetState;
    std::lock_guard<std::mutex> lock(qmtx);
    evq.push_back(std::move(ev));
}

void Core::runCycle()
{
    switch (currentState->innerState) {
    case State::InnerState::ENTRY:
        onEntry();
        break;
    case State::InnerState::EVENT:
        onEvent();
        break;
    case State::InnerState::EXIT:
        onExit();
        break;
    default:
        break;
    }
}

void Core::onEvent()
{
    std::lock_guard<std::mutex> lock(qmtx);
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

void Core::onEntry()
{
    currentState->enter();
    currentState->innerState = State::InnerState::EVENT;
}

void Core::onExit()
{
    currentState->exit();
    std::lock_guard<std::mutex> lock(qmtx);
    evq.erase(std::remove_if(evq.begin(), evq.end(), [this](std::unique_ptr<Events::Event> &ev) {
        return ev->targetState == this->currentState;
    }), evq.end());
    currentState->innerState = State::InnerState::ENTRY;
    currentState = nextState;
}