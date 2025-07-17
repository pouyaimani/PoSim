#ifndef GUI_H
#define GUI_H

#include <lvgl.h>
#include "widget.h"

namespace ui
{
    class LVGL {
    public:
        static LVGL& instance();
        void init();
        void handle();
        Image &getDisplay() noexcept;
    private:
        LVGL() = default;
        std::unique_ptr<Image> display;
        std::unique_ptr<Widget>  actScr;
        std::unique_ptr<Image>  scrImg;
        void createMainScr();
        void createDisplay();
    };
}

#endif