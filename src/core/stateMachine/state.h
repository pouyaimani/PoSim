#ifndef STATE_H
#define STATE_H

#include <cxxtimer.h>
#include <queue>
#include <string>
#include <mutex>

#define DEFINE_ALIASES \
    using StateShPtr = std::shared_ptr<State>;          \
    using EvUnqPtr = std::unique_ptr<Events::Event>;    \
    using StateWkPtr = std::weak_ptr<State>;            \
    using LockGuard = std::lock_guard<std::mutex>;      

namespace StateMachine 
{
    class Core;
    namespace Events {
        class Event;
        class TimeOut;
    }
    class State {
    public:
        DEFINE_ALIASES
        State(StateShPtr parent, const char *name);
        enum class InnerState {
            ENTRY,
            EVENT,
            EXIT
        };
        const std::string &getName() {
            return name;
        }
        template<class T>
        static std::shared_ptr<T> create(StateShPtr parent) {
            return std::make_shared<T>(parent);
        }
        friend class StateMachine::Core;
        friend class StateMachine::Events::Event;

    private:
        uint32_t timeOut;
        InnerState innerState {InnerState::ENTRY};
        virtual void handle(Events::TimeOut &ev);
        std::string name;
        virtual void enter();
        virtual void exit();
    
    protected:
        void goTo(StateShPtr state);
        StateWkPtr parent;


    // DECLARE_HANDLER(TimeOutEvent)
};

}
#endif