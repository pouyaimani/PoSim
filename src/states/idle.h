#ifndef IDLE_H
#define IDLE_H

#include "core/stateMachine/state.h"
#include "core/stateMachine/event.h"
#include "asset.h"
#include "gui/gui.h"
#include "plog/Log.h"

#define ARROW_ANIM_RANGE            300
#define SWIPE_BUTTON_OFFSET         70
#define SWIPE_BUTTON_WIDTH          200
#define SWIPE_BUTTON_HEIGHT         50

using namespace StateMachine;
using namespace ui;

std::unique_ptr<ui::Animation> alignAnim;
std::unique_ptr<ui::Animation> opaAnim;

static void animCbAlign(void *var, int32_t v)
{
    lv_obj_align((lv_obj_t*)var, LV_ALIGN_RIGHT_MID, 0, v);
    lv_obj_set_style_img_opa((lv_obj_t*)var, LV_OPA_100, 0);
    if (v == ARROW_ANIM_RANGE) {
        opaAnim->start();
    }
}

static void animCbOpa(void *var, int32_t v)
{
    lv_obj_set_style_img_opa((lv_obj_t*)var, v, 0);
    if (v == 0) {
        Timer::singleShot(3000, []() {
            alignAnim->start();
        });
    }
}

class Idle : public State {
public:
    Idle(StateShPtr parent) : State(parent, "idle") {
        swipeCardImg.reset(new Image(LVGL::instance().getDisplay().raw()));
        swipeButton.reset(new Button(LVGL::instance().getScreen().raw()));
        swipeButton->align(LV_ALIGN_BOTTOM_MID, 0, SWIPE_BUTTON_OFFSET).
            setSize(SWIPE_BUTTON_WIDTH, SWIPE_BUTTON_HEIGHT).setBgOpa(LV_OPA_0)
                .setBorderOpa(LV_OPA_0).removeFlag(LV_OBJ_FLAG_PRESS_LOCK).addFlag(LV_OBJ_FLAG_CLICKABLE);
        swipeButton->textBox().setSize(SWIPE_BUTTON_WIDTH, SWIPE_BUTTON_HEIGHT)
            .setText("press to swipe")
                .setFont(&lv_font_montserrat_24).setBorderOpa(LV_OPA_0)
                    .setBgOpa(LV_OPA_0).setTextAlign(LV_TEXT_ALIGN_CENTER);
        swipeButton->addEvent([this](lv_event_t* e) {
            goTo(StateFactory::get(StateId::Cardholder));
            }, LV_EVENT_CLICKED);
        swipeCardImg->setSrc(ASSET_IMG_ARROW).align(LV_ALIGN_RIGHT_MID, 0, -ARROW_ANIM_RANGE);
        alignAnim.reset(new Animation);
        opaAnim.reset(new Animation);
    }
    void enter() override {
        LVGL::instance().hideBlkScr();
        alignAnim->setTime(600).setValue(-ARROW_ANIM_RANGE, ARROW_ANIM_RANGE).
        setExec(swipeCardImg.get()->raw(), animCbAlign).start();
        opaAnim->setTime(600).setValue(LV_OPA_100, LV_OPA_0).
            setExec(swipeCardImg.get()->raw(), animCbOpa);
        swipeCardImg->show();
        swipeButton->show();
    }
    void exit() override {
        swipeCardImg->hide();
        swipeButton->hide();
        LVGL::instance().getBlkScreen().show();
        LVGL::instance().showBlkScr();
    }
    void handle(Events::TimeOut &ev) override {

    }

private:
    std::unique_ptr<ui::Image> swipeCardImg;
    std::unique_ptr<ui::Button> swipeButton;
};

#endif