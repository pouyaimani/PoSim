#ifndef GUI_H
#define GUI_H

#include <lvgl.h>

namespace ui
{
    class LVGL {
    public:
        static LVGL& instance();
        void init();
        void handle();
        lv_obj_t *getDisplay() noexcept;
    private:
        LVGL() = default;
        lv_obj_t *display;
        lv_obj_t *actScr;
        lv_obj_t *scrImg;
        void createMainScr();
        void createDisplay();
    };
}

#endif