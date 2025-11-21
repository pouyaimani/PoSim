#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "widget.h"

namespace ui
{
    static const char * btnMap[] = { "1", "2", "3", "\n",
                                    "4", "5", "6", "\n",
                                    "7", "8", "9", "\n",
                                    "clear", "0", "enter", ""};

    class Keypad : public ButtonMatrix {
        public:
        static Keypad &instance() {
            static Keypad keypad(LVGL::instance().getDisplay().raw());
            return keypad;
        }

        Keypad &show() {
            PLOG_DEBUG << "keypad height = " << getHeight();
            slideUp.setValue(getHeight(), 0).start();
            return *this;
        };

        Keypad &hide() {
            slideUp.setValue(0, getHeight()).start();
            return *this;
        };

        private:
        Keypad(lv_obj_t *parent) : ButtonMatrix(parent) {
            setMap(btnMap).setButtonWidth(12, 2).center().
                setSize(LV_PCT(100), LV_PCT(40)).setBgColor(0).
                    setBorderOpa(LV_OPA_50).setBorderColor(0).
                        setBgOpa(LV_OPA_50);
            slideUp.setTime(300).
            setExec(raw(), animCbSlideUp);
            lv_obj_update_layout(lv_scr_act());

        }

        ui::Animation slideUp;

        static void animCbSlideUp(void *var, int32_t v) {
            lv_obj_align((lv_obj_t*)var, LV_ALIGN_BOTTOM_MID, 0, v);
        }

    };

}

#endif