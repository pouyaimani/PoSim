#ifndef STARTUP_H
#define STARTUP_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "core/timer/timer.h"

using namespace StateMachine;

class StartUp : public State {
public:
    StartUp(StateShPtr parent) : State(parent, "startUp") {}
    void enter() override {
        PLOG_DEBUG << "startup enter()";
        Timer::singleShot(2000, []() {
            Events::TimeOut ev;
            ev.dispatch();
        });
    }
    void exit() override {
        PLOG_DEBUG << "startup exit()";
    }
    void handle(Events::TimeOut &ev) override {
        PLOG_DEBUG << "startup handle(Events::TimeOut &)";
    }

private:

};

#endif