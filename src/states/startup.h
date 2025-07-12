#ifndef STARTUP_H
#define STARTUP_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "core/timer/timer.h"
#include "gui/gui.h"
#include "asset.h"


using namespace StateMachine;

class StartUp : public State {
public:
    StartUp(StateShPtr parent) : State(parent, "startUp") {}
    void enter() override;
    void exit() override;
    void handle(Events::TimeOut &ev) override;

private:
    lv_obj_t *logo;
    lv_anim_t *anim;

};

#endif