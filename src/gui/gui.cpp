#include "gui.h"
#include "lvgl.h"
#include <chrono>
#include <SDL2/SDL.h>        // macOS/Linux
#include "plog/Log.h"
#include "asset.h"
#include "statusBar.h"
#include <src/tick/lv_tick.h>

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

    /*LVGL init*/
    lv_init();
    lv_fs_stdio_init();
    lv_bmp_init();
    lv_libpng_init();

    // SDL Display initialization
    lv_display_t *disp = lv_sdl_window_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_default(disp);
    SDL_Window* win = SDL_GetWindowFromID(1);
    SDL_Renderer* renderer = SDL_GetRenderer(win);

    // Force logical size (disables auto-scaling)
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

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

void LVGL::createDisplay()
{
    display.reset(new Image(actScr->raw()));
    display->setSrc(ASSET_IMG_BLK_STARTUP_SCR).align(LV_ALIGN_CENTER, -13, 27).setRadius(10);
}

Image &LVGL::getDisplay() noexcept
{
    return dynamic_cast<Image&>(*display.get());
}

Widget &LVGL::getScreen() noexcept
{
    return dynamic_cast<Widget&>(*actScr.get());
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