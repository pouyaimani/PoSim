#include "gui.h"
#include "lvgl.h"
#include <chrono>
#include <SDL2/SDL.h>        // macOS/Linux
#include "plog/Log.h"

using namespace ui;

#define SCREEN_WIDTH    520
#define SCREEN_HEIGHT   840

static uint32_t lv_tick_custom();

void LVGL::init()
{
    /*LVGL init*/
    lv_init();

    // SDL Display initialization
    lv_display_t *disp = lv_sdl_window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_default(disp);

    // SDL Mouse input
    lv_indev_t *indev = lv_sdl_mouse_create();
    lv_indev_set_display(indev, disp);

    lv_tick_set_cb(lv_tick_custom);
}

void LVGL::handle()
{
    uint32_t time_till_next = lv_timer_handler();

   /* Wait forever for events upon LV_NO_TIMER_READY, because there
    * is no reason to call lv_timer_handler sooner. */
    if(time_till_next == LV_NO_TIMER_READY) {
        PLOG_ERROR << "LVGL: No timer is ready.";
    }
}

// Returns current system tick in ms (required by LV_TICK_CUSTOM)
uint32_t lv_tick_custom() 
{
    using namespace std::chrono;
    static const auto start_time = steady_clock::now();
    return static_cast<uint32_t>(
        duration_cast<milliseconds>(steady_clock::now() - start_time).count()
    );
}