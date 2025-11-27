#ifndef CARD_HOLDER_H
#define CARD_HOLDER_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "plog/Log.h"
#include "gui/widget.h"
#include "common.h"
#include "gui/keypad.h"

using namespace StateMachine;

class CardHolder : public State {
public:
    CardHolder(StateShPtr parent) : State(parent, "card holder") {
        menu.reset(new Menu(LVGL::instance().getWidgetScreen().raw()));
        menu->setSize(LVGL::instance().getWidgetScreen().getWidth(),
             LVGL::instance().getWidgetScreen().getHeight()).center().setRadius(MENU_RADIUS).
            setBorderOpa(0).setBgOpa(0);
        menu->appendTextItem("Sale").addEvent([this](lv_event_t* e) {
            goTo(StateFactory::get(StateId::ServiceSale));
        }, LV_EVENT_CLICKED);
        menu->appendTextItem("Balance").addEvent([](lv_event_t* e) {
        }, LV_EVENT_CLICKED);;
        menu->hide();
    }

    static void animCbOpa(void *var, int32_t v) {
        lv_obj_set_style_bg_opa((lv_obj_t*)var, v, 0);
    }

    void enter() override {
        menu->setBgOpa(LV_OPA_0).show();
        appearMenu.setTime(600).setValue(LV_OPA_0, LV_OPA_0).
        setExec(menu->raw(), animCbOpa).start();
    }
    void exit() override {
        menu->hide();
    }
    void handle(Events::TimeOut &ev) override {

    }

    void handle(Events::Keypad &ev) override {
        PLOG_DEBUG << "key = " << (int)ev.key;
    }

private:
    std::unique_ptr<ui::Menu> menu;
    ui::Animation appearMenu;
};

#endif