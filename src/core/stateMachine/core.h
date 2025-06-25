#ifndef CORE_H
#define CORE_H

#include "state.h"
#include "event.h"
#include <mutex>
#include <vector>

namespace StateMachine 
{
    using Callback = std::function<void()>;
    class Core {
        DEFINE_ALIASES
    public:
        // Make Core Class Non-Copyable
        Core() = default;
        Core(const Core&) = delete;
        Core& operator=(const Core&) = delete;
        static Core &instance();
        void init(StateShPtr state) noexcept;
        void exec();
        void runCycle();
        void registerCb(Callback cb) noexcept;
        friend class Events::Event;
        friend class State;

    private:
        StateWkPtr currentState;
        StateWkPtr nextState;
        std::vector<EvUnqPtr> evq;
        std::mutex qmtx;
        std::mutex cbMtc;
        std::vector<Callback> callback;
        void raiseEvent(EvUnqPtr ev);
        void goTo(StateShPtr);
        void onExit();
        void onEntry();
        void onEvent();
    };
}

#endif // CORE_H