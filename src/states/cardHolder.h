#ifndef CARD_HOLDER_H
#define CARD_HOLDER_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "gui/widget.h"

using namespace StateMachine;

class CardHolder : public State {
public:
    CardHolder(StateShPtr parent) : State(parent, "card holder") {
        menu.reset(new Menu(LVGL::instance().getDisplay().raw()));
        menu->setSize(LV_PCT(80), LV_PCT(80)).center().setBgOpa(LV_OPA_80).setRadius(10);
        menu->enableRootBack(true).append("Sale").append("Balance").create();
    }
    void enter() override {
        PLOG_DEBUG << "enter to Cardholder.";
        menu->show();
    }
    void exit() override {
        menu->hide();
    }
    void handle(Events::TimeOut &ev) override {

    }

private:
    std::unique_ptr<ui::Menu> menu;
};

#endif