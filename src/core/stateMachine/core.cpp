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
void Core::init(StateShPtr state) noexcept
{
    PLOG_INFO << "Core::init called.";
    currentState = state;
    if (auto locked = currentState.lock()) {
        locked->innerState = State::InnerState::ENTRY;
    }
}

void Core::registerCb(Callback cb) noexcept
{
    PLOG_INFO << "Core::registerCb called.";
    LockGuard lock(cbMtc);
    callback.push_back(std::move(cb));
}

void Core::exec()
{
    PLOG_INFO << "Core::exec started.";
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
            LockGuard lock(cbMtc);
            cbs = callback; // safely copy the list
        }

        for (auto& cb : cbs) {
            cb();  // call without holding the lock
        }
    }
}

void Core::raiseEvent(EvUnqPtr &&ev)
{
    PLOG_INFO << "Core::raiseEvent called.";
    if (ev->targetState.expired()) {
        ev->targetState = currentState;
    }
    LockGuard lock(qmtx);
    evq.push_back(std::move(ev));
}

void Core::runCycle()
{
    if (auto locked = currentState.lock()) {
        switch (locked->innerState) {
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
}

void Core::onEvent()
{
    // PLOG_INFO << "Core::onEvent called.";÷
    LockGuard lock(qmtx);
    for (auto it = evq.begin() ; it != evq.end();) {
        if (auto target = (*it)->targetState.lock(); target == currentState.lock()) {
            PLOG_INFO << "Core::onEvent() -> event come to " << target->getName() << " state.";
            (*it)->dispatchTo(target);
            it = evq.erase(it);
        } else {
            ++it;
        }
    }
}

void Core::goTo(StateShPtr state)
{
    PLOG_INFO << "Core::goTo called.";
    if (auto lock = currentState.lock()) {
        lock->innerState = State::InnerState::EXIT;
        nextState = state;
    }
}

void Core::onEntry()
{
    PLOG_INFO << "Core::onEntry called.";
    if (auto state = currentState.lock()) {
        PLOG_INFO << "Core::onEntry() -> enter to " << state->getName() << " state.";
        state->enter();
        state->innerState = State::InnerState::EVENT;
    }
}

void Core::onExit()
{
    PLOG_INFO << "Core::onExit called.";
    if (auto state = currentState.lock()) {
        state->exit();
        PLOG_INFO << "Core::onExit() -> exit from " << state->getName() << " state.";
        LockGuard lock(qmtx);
        evq.erase(std::remove_if(evq.begin(), evq.end(), [this](EvUnqPtr &ev) {
            return ev->targetState.lock() == this->currentState.lock();
        }), evq.end());
        state->innerState = State::InnerState::ENTRY;
        if (auto next = nextState.lock()) {
            currentState = next;
        }
    }
}