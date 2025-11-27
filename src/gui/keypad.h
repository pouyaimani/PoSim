#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "widget.h"
#include "core/stateMachine/core.h"
#include "core/timer/timer.h"

namespace ui {

    // Button labels for the keypad
    static constexpr const char* btnMap[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        "clear", "0", "enter", ""
    };

    class Keypad : public Widget {
    public:
        enum class KeypadMode {
            PlainNumeric,
            AmountFormatted,
            HiddenPassword
        };
        // Singleton instance
        static Keypad& instance() {
            static Keypad keypad(LVGL::instance().getDisplay().raw());
            return keypad;
        }

        // Show/hide animations
        Keypad& show() {
            Widget::show();
            slideUp.setValue(getHeight(), 0).start();
            return *this;
        }

        Keypad& hide() {
            slideUp.setValue(0, getHeight()).start();
            Timer::singleShot(300, [this](){
                Widget::hide();
            });
            return *this;
        }

        // Set title text
        Keypad& setTitle(const char* txt) {
            title->setText(txt);
            return *this;
        }

        Keypad& setMode(KeypadMode mode) {
            this->mode = mode;
            if (mode == KeypadMode::HiddenPassword) {
                textArea->hide();
                passBox->show();
            } else {
                passBox->hide();
                textArea->show();
            }
            return *this;
        }

    private:
        // Private constructor for singleton
        explicit Keypad(lv_obj_t* parent) : Widget(parent) {

            handler = {
                {KeypadMode::PlainNumeric,   [this](const std::string& key, int id){ handleNormal(key, id); }},
                {KeypadMode::AmountFormatted,   [this](const std::string& key, int id){ handleAmount(key, id); }},
                {KeypadMode::HiddenPassword, [this](const std::string& key, int id){ handlePassword(key, id); }}
            };

            const auto parentWidth  = LVGL::instance().getDisplay().getWidth();
            const auto parentHeight = LVGL::instance().getDisplay().getHeight();

            // Configure base widget
            setScrollEnable(false)
                .align(LV_ALIGN_BOTTOM_MID)
                .setSize(parentWidth, parentHeight)
                .setBgColor(0)
                .setBorderOpa(LV_OPA_0)
                .setBorderColor(0)
                .setBgOpa(LV_OPA_0);

            // Title box
            title = std::make_unique<TextBox>(raw());
            title->setBgOpa(LV_OPA_0)
                 .setBorderOpa(LV_OPA_0)
                 .autoSize()
                 .align(LV_ALIGN_TOP_MID, 0, 100)
                 .setFont(&lv_font_montserrat_24)
                 .setTextColor(0);

            // Text area
            textArea = std::make_unique<TextArea>(raw());
            textArea->setBgColor(0)
                    .setBgOpa(LV_OPA_0)
                    .setBorderOpa(LV_OPA_0)
                    .setSize(LV_PCT(60), LV_PCT(15))
                    .setRadius(20)
                    .align(LV_ALIGN_TOP_MID, 0, 200)
                    .setFont(&lv_font_montserrat_24)
                    .setTextColor(0)
                    .setTextAlign(LV_TEXT_ALIGN_CENTER);

            // PasswordBox
            passBox = std::make_unique<PasswordBox>(4, raw());
            passBox->setBgOpa(LV_OPA_0)
                    .setBorderOpa(LV_OPA_0)
                    .setSize(LV_PCT(60), LV_PCT(15))
                    .align(LV_ALIGN_TOP_MID, 0, 200);

            // Button matrix
            btnMat = std::make_unique<ButtonMatrix>(raw());
            btnMat->setMap(btnMap)
                  .setButtonWidth(12, 2)
                  .center()
                  .setSize(parentWidth, LV_PCT(40))
                  .setBgColor(0)
                  .setBorderOpa(LV_OPA_0)
                  .setBorderColor(0)
                  .setBgOpa(LV_OPA_20)
                  .align(LV_ALIGN_BOTTOM_MID);

            // Slide-up animation
            slideUp.setTime(300).setExec(raw(), animCbSlideUp);

            // Event handling
            btnMat->addEvent([this](lv_event_t* e) {
                if (lv_event_get_code(e) != LV_EVENT_VALUE_CHANGED) return;
                lv_obj_t* obj = lv_event_get_target_obj(e);
                uint32_t id = lv_buttonmatrix_get_selected_button(obj);
                std::string key = lv_buttonmatrix_get_button_text(obj, id);
                handler[mode](key, id);

            }, LV_EVENT_ALL);

            // Style button matrix items
            lv_obj_set_style_text_color(btnMat->raw(), lv_color_hex(0xffffff), LV_PART_ITEMS);
            lv_obj_set_style_bg_color(btnMat->raw(), lv_color_hex(0xffffff), LV_PART_ITEMS);
            lv_obj_set_style_bg_opa(btnMat->raw(), 10, LV_PART_ITEMS);

            setMode(KeypadMode::PlainNumeric);

            lv_obj_update_layout(lv_scr_act());
        }

        // Member variables
        ui::Animation slideUp;
        std::unique_ptr<ui::ButtonMatrix> btnMat;
        std::unique_ptr<ui::TextArea> textArea;
        std::unique_ptr<ui::PasswordBox> passBox;
        std::unique_ptr<ui::TextBox> title;
        std::string text;
        KeypadMode mode = KeypadMode::PlainNumeric;
        std::unordered_map<KeypadMode, std::function<void(const std::string&, int)>> handler;


        // Animation callback
        static void animCbSlideUp(void* var, int32_t v) {
            lv_obj_align(static_cast<lv_obj_t*>(var), LV_ALIGN_BOTTOM_MID, 0, v);
        }

        // --- Handlers ---
        void handleNormal(const std::string& key, int id) {

                if (id == 9) { // clear
                    if (!text.empty()) text.pop_back();
                    textArea->setText(text.c_str());
                } else if (id == 11) { // enter
                    auto ev = std::make_unique<StateMachine::Events::Keypad>();
                    ev->key = static_cast<StateMachine::Events::Keypad::Key>(id + 1);
                    ev->keyStr = key;
                    ev->dispatch();
                } else {
                    text.append(key);
                    textArea->setText(text.c_str());
                }
        }

        void handleAmount(const std::string& key, int id) {
            handleNormal(key, id);
            textArea->setText(formatAmount(text).c_str());
        }

        void handlePassword(const std::string& key, int id) {
            if (id == 9) { // clear
                passBox->popBack();
            } else if (id == 11) { // enter
                    auto ev = std::make_unique<StateMachine::Events::Keypad>();
                    ev->key = static_cast<StateMachine::Events::Keypad::Key>(id + 1);
                    ev->keyStr = key;
                    ev->dispatch();
            } else {
                passBox->addKey(key.back());
            }
        }

        std::string formatAmount(const std::string& digits) {
            if (digits.empty()) return "";

            std::string result;
            int count = 0;

            for (int i = digits.size() - 1; i >= 0; --i) {
                result.push_back(digits[i]);
                count++;

                if (count == 3 && i != 0) {
                    result.push_back(',');
                    count = 0;
                }
            }

            std::reverse(result.begin(), result.end());
            return result;
        }
    };

} // namespace ui

#endif // KEYPAD_H_
