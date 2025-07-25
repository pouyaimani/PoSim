#ifndef GUI_H
#define GUI_H

#include <lvgl.h>
#include "widget.h"
#include "statusBar.h"

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   1536
#define DISPLAY_WIDTH   470
#define DISPLAY_HEIGHT  840

namespace ui
{
    class LVGL {
    public:
        static LVGL& instance();
        void init();
        void handle();
        Image &getDisplay() noexcept;
        void showStatusBar();
        void hideStatusBar();
    private:
        LVGL() = default;
        std::unique_ptr<Image> display;
        std::unique_ptr<Widget> actScr;
        std::unique_ptr<Image> scrImg;
        std::unique_ptr<StatusBar> statusBar;
        void createMainScr();
        void createDisplay();
        void createStatusBar();
    };
}

#endif