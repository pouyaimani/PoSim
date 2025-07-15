#include "gui.h"
#include "lvgl.h"
#include <chrono>
#include <SDL2/SDL.h>        // macOS/Linux
#include "plog/Log.h"
#include "asset.h"

using namespace ui;

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   1536

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

    lv_tick_set_cb(lv_tick_custom);

    createMainScr();
    createDisplay();
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