#ifndef GUI_H
#define GUI_H

#include <lvgl.h>
#include "widget.h"
#include "statusBar.h"
#include "core/timer/timer.h"

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
        Widget &getScreen() noexcept;
        Widget &getBlkScreen() noexcept;
        Widget &getWidgetScreen() noexcept;
        void showStatusBar();
        void hideStatusBar();
        void showBlkScr();
        void hideBlkScr();
    private:
        LVGL() = default;
        std::unique_ptr<Image> display;
        std::unique_ptr<Widget> actScr;
        std::unique_ptr<Widget> blkScr;
        std::unique_ptr<Widget> widgetScr;
        std::unique_ptr<Image> scrImg;
        std::unique_ptr<StatusBar> statusBar;
        Timer lvTimer;
        void createMainScr();
        void createDisplay();
        void createStatusBar();
        ui::Animation blkScrAnim;
    };
}

#endif