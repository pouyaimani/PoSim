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
        // Make Core Class Non-Copyable
        Core() = default;
        Core(const Core&) = delete;
        Core& operator=(const Core&) = delete;
        static Core &instance();
        void init(State *state) noexcept;
        void exec();
        void runCycle();
        void registerCb(Callback cb) noexcept;
        friend class Events::Event;
        friend class State;

    private:
        State *currentState;
        State *nextState;
        std::vector<std::unique_ptr<Events::Event>> evq;
        std::mutex qmtx;
        std::mutex cbMtc;
        std::vector<Callback> callback;
        void raiseEvent(std::unique_ptr<Events::Event> ev);
        void goTo(State *state);
        void onExit();
        void onEntry();
        void onEvent();
    };
}

#endif // CORE_H