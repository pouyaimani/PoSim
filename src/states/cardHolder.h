#ifndef CARD_HOLDER_H
#define CARD_HOLDER_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "gui/widget.h"
#include "common.h"

using namespace StateMachine;

class CardHolder : public State {
public:
    CardHolder(StateShPtr parent) : State(parent, "card holder") {
        menu.reset(new Menu(LVGL::instance().getDisplay().raw()));
        menu->setSize(LV_PCT(80), LV_PCT(80)).center().setRadius(MENU_RADIUS).
            setBgColor(0).setBorderColor(0).setBorderOpa(0);
        menu->appendTextItem("Sale").addEvent([](lv_event_t* e) {
            PLOG_DEBUG << "Sale is chosen.";
        }, LV_EVENT_CLICKED);
        menu->appendTextItem("Balance").addEvent([](lv_event_t* e) {
            PLOG_DEBUG << "Balance is chosen.";
        }, LV_EVENT_CLICKED);;
        menu->hide();
    }

    static void animCbOpa(void *var, int32_t v) {
        lv_obj_set_style_bg_opa((lv_obj_t*)var, v, 0);
    }

    void enter() override {
        PLOG_DEBUG << "enter to Cardholder.";
        menu->setBgOpa(LV_OPA_0).show();
        appearMenu.setTime(600).setValue(LV_OPA_0, LV_OPA_50).
        setExec(menu->raw(), animCbOpa).start();
    }
    void exit() override {
        menu->hide();
    }
    void handle(Events::TimeOut &ev) override {

    }

private:
    std::unique_ptr<ui::Menu> menu;
    ui::Animation appearMenu;
};

#endif