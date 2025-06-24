#ifndef CORE_H
#define CORE_H

#include "state.h"
#include "event.h"
#include <mutex>
#include <vector>

namespace StateMachine 
{
    class Core {
    using Callback = std::function<void()>;
    public:
        static Core &instance();
        void init(State *state);
        void exec();
        void runCycle();
        void registerCb(Callback cb) noexcept;
        friend class Events::Event;
        friend class State;

    private:
        State *currentState;
        State *nextState;
        std::vector<std::unique_ptr<Events::Event>> evq;
        std::recursive_mutex qmtx;
        std::recursive_mutex cbMtc;
        std::vector<Callback> callback;
        void raiseEvent(std::unique_ptr<Events::Event> ev);
        void goTo(State *state);
        void doExitTask();
        void doEntryTask();
        void checkEvents();
    };
}

#endif // CORE_H