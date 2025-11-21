#include "gui.h"
#include "lvgl.h"
#include <chrono>
#include <SDL2/SDL.h>        // macOS/Linux
#include "plog/Log.h"
#include "asset.h"
#include "statusBar.h"
#include <src/tick/lv_tick.h>

// Detect platform
#if defined(__APPLE__) && defined(__MACH__)
    #define IS_MACOS true
#else
    #define IS_MACOS false
#endif

using namespace ui;

static uint32_t lv_tick_custom();

LVGL& LVGL::instance()
{
    static LVGL instance;
    return instance;
}

void LVGL::init()
{
    // Disables SDL's DPI scaling behavior
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");


    /*LVGL init*/
    lv_init();
    lv_fs_stdio_init();
    lv_bmp_init();
    lv_libpng_init();

    // SDL Display initialization
    lv_display_t *disp = lv_sdl_window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_default(disp);

    if (IS_MACOS) {
        SDL_Window *window = SDL_GetWindowFromID(1);
        if (window) {
            float scale;
            SDL_GetDisplayDPI(0, NULL, NULL, &scale);
            int window_w, window_h, drawable_w, drawable_h;
            SDL_GetWindowSize(window, &window_w, &window_h);
            SDL_GL_GetDrawableSize(window, &drawable_w, &drawable_h);
            float scale_x = (float)drawable_w / window_w;
            float scale_y = (float)drawable_h / window_h;
            lv_display_set_resolution(disp, drawable_w, drawable_h);
        }
    }
    // SDL Mouse input
    lv_indev_t *indev = lv_sdl_mouse_create();
    lv_indev_set_display(indev, disp);

    lvTimer.start(1, []() {
        lv_tick_inc(1);
    });

    createMainScr();
    createDisplay();
    createStatusBar();
}

void LVGL::createMainScr()
{
    /*Create main screen*/
    actScr.reset(new Widget(NULL));
    lv_screen_load(actScr->raw());

    scrImg.reset(new Image(actScr->raw()));
    scrImg->setSrc(ASSET_IMG_MAIN_SCR);
    scrImg->center();
}

static void animCbBlkScr(void *var, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t*)var, v, LV_PART_MAIN);
}

void LVGL::createDisplay()
{
    display.reset(new Image(scrImg->raw()));
    display->setSize(470, 890);
    display->setSrc(ASSET_IMG_BLK_STARTUP_SCR).align(LV_ALIGN_CENTER, -13, 27).setRadius(10);
    lv_obj_update_layout(lv_scr_act());
    blkScr.reset(new Widget(display->raw()));
    blkScr->setSize(LV_PCT(100),LV_PCT(95)).setBgColor(0).
        setBorderOpa(0).setBgOpa(LV_OPA_0).setScrollEnable(false).
        align(LV_ALIGN_BOTTOM_MID, 0, -20).hide();
    blkScrAnim.setTime(100).setExec(blkScr->raw(), animCbBlkScr);
    lv_obj_update_layout(lv_scr_act());
    widgetScr.reset(new Widget(display->raw()));
    widgetScr->setSize(LV_PCT(100), LV_PCT(80)).setBgOpa(LV_OPA_0).
        setBorderOpa(LV_OPA_0).align(LV_ALIGN_BOTTOM_MID).
            setScrollEnable(false);
}

Image &LVGL::getDisplay() noexcept
{
    return dynamic_cast<Image&>(*display.get());
}

Widget &LVGL::getScreen() noexcept
{
    return dynamic_cast<Widget&>(*actScr.get());
}

Widget &LVGL::getBlkScreen() noexcept
{
    return dynamic_cast<Widget&>(*blkScr.get());
}

Widget &LVGL::getWidgetScreen() noexcept
{
    return dynamic_cast<Widget&>(*widgetScr.get());
}

void LVGL::showBlkScr() 
{
    blkScrAnim.setValue(LV_OPA_0, LV_OPA_50).start();
}

void LVGL::hideBlkScr() 
{
    blkScrAnim.setValue(LV_OPA_50, LV_OPA_0).start();
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

void LVGL::createStatusBar()
{
    statusBar.reset(new StatusBar);
}

void LVGL::showStatusBar()
{
    statusBar->show();
}

void LVGL::hideStatusBar()
{
    statusBar->hide();
}