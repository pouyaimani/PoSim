#ifndef STARTUP_H
#define STARTUP_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "gui/widget.h"

using namespace StateMachine;

class StartUp : public State {
public:
    StartUp(StateShPtr parent) : State(parent, "startUp") {}
    void enter() override;
    void exit() override;
    void handle(Events::TimeOut &ev) override;

private:
    std::unique_ptr<ui::Image> logo;
    std::unique_ptr<ui::Animation> anim;

};

#endif