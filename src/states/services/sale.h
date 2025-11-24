#ifndef SALE_H_
#define SALE_H_

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "gui/widget.h"
#include "common.h"
#include "gui/keypad.h"

using namespace StateMachine;

namespace Sale
{

    enum class SubStates {
        enterAmount,
        enterPass
    };

static std::unordered_map<SubStates, std::shared_ptr<State>> SubState;

class EnterPass : public State {
public:
    EnterPass(StateShPtr parent) : State(parent, "get password") {
    }

    static void animCbOpa(void *var, int32_t v) {
        lv_obj_set_style_bg_opa((lv_obj_t*)var, v, 0);
    }

    void enter() override {
        Keypad::instance().setTitle("Enter Password")
            .setMode(Keypad::KeypadMode::HiddenPassword).show();
    }
    void exit() override {
    }
    void handle(Events::TimeOut &ev) override {

    }

    void handle(Events::Keypad &ev) override {
    }

private:
    ui::Animation appearMenu;
};

class EnterAmount : public State {
public:
    EnterAmount(StateShPtr parent) : State(parent, "Enter amount") {
        
    }

    static void animCbOpa(void *var, int32_t v) {
        lv_obj_set_style_bg_opa((lv_obj_t*)var, v, 0);
    }

    void enter() override {

        Keypad::instance().setTitle("Enter Amount")
            .setMode(Keypad::KeypadMode::AmountFormatted).show();
    }
    void exit() override {
    }
    void handle(Events::TimeOut &ev) override {

    }

    void handle(Events::Keypad &ev) override {
        if (ev.key == Events::Keypad::Key::KEY_ENTER_) {
            goTo(getPass);
        }
    }

private:
    ui::Animation appearMenu;
    StateShPtr getPass;
};

class Service : public State {
public:
    Service(StateShPtr parent) : State(parent, "sale service") {
        
    }

    void enter() override {
        static std::once_flag flag;
        std::call_once(flag, [=]() {
            SubState[SubStates::enterAmount] = std::make_shared<EnterAmount>(shared_from_this());
            SubState[SubStates::enterPass] = std::make_shared<EnterPass>(shared_from_this());
        });
        goTo(SubState[SubStates::enterAmount]);
    }
};

}
#endif