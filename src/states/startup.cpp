#include "startup.h"
#include "plog/Log.h"
#include "core/timer/timer.h"
#include "asset.h"
#include "gui/gui.h"

#define START_UP_SHOW_TIME_MS 4000

static void animCb(void *var, int32_t v)
{
    lv_obj_set_style_img_opa((lv_obj_t*)var, v, 0);
}

void StartUp::enter()
{
    // logo = lv_img_create(ui::LVGL::instance().getDisplay());
    logo.reset(new ui::Image(ui::LVGL::instance().getDisplay().raw()));
    logo->setSrc(ASSET_IMG_POS_LOGO).center().show();

    anim.reset(new ui::Animation);
    anim->setTime(START_UP_SHOW_TIME_MS).setValue(0, LV_OPA_100).setExec(logo.get()->raw(), animCb).start();

    Timer::singleShot(START_UP_SHOW_TIME_MS + 1000, []() {
        Events::TimeOut ev;
        ev.dispatch();
    });
}

void StartUp::handle(Events::TimeOut &ev)
{
    logo->hide();
    ui::LVGL::instance().getDisplay().setSrc(ASSET_IMG_POS_DISP);
}

void StartUp::exit()
{
    logo.release();
    anim.release();
}