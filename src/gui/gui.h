#ifndef GUI_H
#define GUI_H

namespace ui
{
    class Gui {
    public:
        Gui() = default;
        virtual void init() = 0;
        virtual void handle() = 0;
    };

    class LVGL : public Gui {
    public:
        LVGL() = default;
        void init() override;
        void handle() override;
        void createMainScr();
    };
}

#endif