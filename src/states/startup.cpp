#include "startup.h"

#define START_UP_SHOW_TIME_MS 4000

static void animCb(void *var, int32_t v)
{
    lv_obj_set_style_img_opa((lv_obj_t*)var, v, 0);
}

void StartUp::enter()
{
    logo = lv_img_create(ui::LVGL::instance().getDisplay());
    lv_img_set_src(logo, ASSET_IMG_POS_LOGO);
    lv_obj_set_align(logo, LV_ALIGN_CENTER);
    // 
    lv_obj_clear_flag(logo, LV_OBJ_FLAG_HIDDEN);

    anim = new lv_anim_t;
    lv_anim_init(anim);
    lv_anim_set_var(anim, logo);
    lv_anim_set_time(anim, START_UP_SHOW_TIME_MS);
    lv_anim_set_values(anim, 0, LV_OPA_100);
    lv_anim_set_exec_cb(anim, animCb);
    lv_anim_start(anim);

    Timer::singleShot(START_UP_SHOW_TIME_MS + 1000, []() {
        Events::TimeOut ev;
        ev.dispatch();
    });
}

void StartUp::handle(Events::TimeOut &ev)
{
    lv_obj_add_flag(logo, LV_OBJ_FLAG_HIDDEN);
    lv_img_set_src(ui::LVGL::instance().getDisplay(), ASSET_IMG_POS_DISP);
}

void StartUp::exit()
{
    lv_obj_del(logo);
    lv_anim_delete(anim, animCb);
}