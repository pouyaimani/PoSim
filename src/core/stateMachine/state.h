#ifndef STATE_H
#define STATE_H

#include <cxxtimer.h>
#include <queue>
#include <string>

namespace StateMachine 
{
    #define DECLARE_HANDLER(EventType) \
    virtual void handle(EventType&) {}
    class Core;
    namespace Events {
        class Event;
        class TimeOut;
    }

    class State {

    public:
        State(State *parent, const char *name);
        enum class InnerState {
            ENTRY,
            EVENT,
            EXIT
        };
        virtual void enter();
        virtual void exit();
        const std::string &getName() {
            return name;
        }
        friend class StateMachine::Core;
        friend class StateMachine::Events::Event;

    private:
        uint32_t timeOut;
        InnerState innerState {InnerState::ENTRY};
        virtual void handle(Events::TimeOut &ev);
        std::string name;
    
    protected:
        void goTo(State *nextState);

    // DECLARE_HANDLER(TimeOutEvent)
};

}
#endif