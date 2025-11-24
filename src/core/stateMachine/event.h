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
        DEFINE_ALIASES
        Event(StateShPtr targetState = CURRENT_STATE) {
            this->targetState = targetState;
        }
        virtual ~Event() = default;
        void dispatch(StateShPtr state = nullptr);
        friend class StateMachine::Core;

    private:
        StateWkPtr targetState;
        virtual void dispatchTo(StateShPtr) = 0;
        virtual Event * clone() = 0;
    
    protected:
        template<class T>
        void passToState(T &t, StateShPtr state) {
            state->handle(t);
        }
    };

    class TimeOut : public Event {
    public:
        TimeOut() {}
    private:
        void dispatchTo(StateShPtr state) override {
            passToState(*this, state);
        }

        Event * clone() override {
            return new TimeOut(*this);
        }
    };

    class Keypad : public Event {
    public:
        enum class Key {
            KEY_1_ = 1,
            KEY_2_,
            KEY_3_,
            KEY_4_,
            KEY_5_,
            KEY_6_,
            KEY_7_,
            KEY_8_,
            KEY_9_,
            KEY_CLEAR_,
            KEY_0_,
            KEY_ENTER_
        };
        Keypad() {}
        Key key;
        std::string keyStr;
    private:
        void dispatchTo(StateShPtr state) override {
            passToState(*this, state);
        }

        Event * clone() override {
            return new Keypad(*this);
        }

    };

}

}
#endif