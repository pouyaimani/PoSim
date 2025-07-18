#ifndef IDLE_H
#define IDLE_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"

using namespace StateMachine;

class Idle : public State {
public:
    Idle(StateShPtr parent) : State(parent, "idle") {}
    void enter() override {
        PLOG_DEBUG << "Enter to Idle state";
    }
    void exit() override {

    }
    void handle(Events::TimeOut &ev) override {

    }

private:
    std::unique_ptr<ui::Image> logo;
    std::unique_ptr<ui::Animation> anim;

};

#endif