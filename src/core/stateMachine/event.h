#ifndef EVENT_H
#define EVENT_H

#include <queue>
#include <mutex>
#include "state.h"

#define CURRENT_STATE nullptr

namespace StateMachine 
{
    class State;
    class Core;
namespace Events
{

    class Event {
    public:
        Event(State *targetState = CURRENT_STATE) {
            this->targetState = targetState;
        }
        virtual ~Event() = default;
        void dispatch(State *state = nullptr);
        friend class StateMachine::Core;

    private:
        State *targetState;
        virtual void dispatchTo(State *state) = 0;
        virtual Event * clone() = 0;
    
    protected:
        template<class T>
        void passToState(T &t, State* state) {
            state->handle(t);
        }
    };

    class TimeOut : public Event {
    public:
        TimeOut() {}
    private:
        void dispatchTo(State *state) override {
            passToState(*this, state);
        }

        Event * clone() override {
            return new TimeOut(*this);
        }
    };

}

}
#endif