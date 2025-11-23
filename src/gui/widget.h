#ifndef WIDGET_H_
#define WIDGET_H_

#include <lvgl.h>
#include <memory>
#include <functional>
#include "plog/Log.h"
#include "common.h"

namespace ui
{

    inline void eventTrampoline(lv_event_t * e)
    {
        auto cb = static_cast<std::function<void(lv_event_t*)>*>(lv_event_get_user_data(e));
        if (cb && *cb)
            (*cb)(e);
    }

    enum class Layout{
        FLEX_HOR,
        FLEX_VER,
        FLEX_HOR_WRAP,
        FLEX_VER_WRAP,
        GRID
    };
    
    template<typename Derived, lv_obj_t* (*CreateFn)(lv_obj_t*)>
    class object {
        #define RET_REF return static_cast<Derived&>(*this);
    public:
        explicit object(lv_obj_t* parent) : obj(CreateFn(parent)) {}
        virtual ~object() {
            if (obj) lv_obj_del(obj);
        }

        lv_obj_t *raw() { return obj; }

        Derived& align(lv_align_t align, int16_t x = 0, int16_t y = 0) {
            lv_obj_align(obj, align, x, y); RET_REF
        }

        Derived& setSize(int w, int h) {
            lv_obj_set_size(obj, w, h); RET_REF
        }

        Derived& center() {
            lv_obj_center(obj); RET_REF
        }

        Derived& setRadius(int radius) {
            lv_obj_set_style_radius(obj, radius, LV_PART_MAIN); RET_REF
        }

        Derived& show() {
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN); RET_REF
        }

        Derived& hide() {
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN); RET_REF
        }

        Derived& setBgOpa(lv_opa_t opa) {
            lv_obj_set_style_bg_opa(obj, opa, LV_PART_MAIN); RET_REF
        }

        Derived& setBorderOpa(lv_opa_t opa) {
            lv_obj_set_style_border_opa(obj, opa, LV_PART_MAIN); RET_REF
        }

        Derived& setBgColor(uint32_t color) {
            lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_MAIN); RET_REF
        }

        Derived& setBorderColor(uint32_t color) {
            lv_obj_set_style_border_color(obj, lv_color_hex(color), LV_PART_MAIN); RET_REF
        }

        Derived& setBorderWidth(uint16_t width) {
            lv_obj_set_style_border_width(obj, width, LV_PART_MAIN); RET_REF
        }

        Derived& hideBorder(lv_border_side_t border) {
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL & ~border, LV_PART_MAIN); RET_REF
        }

        Derived& showBorder(lv_border_side_t border) {
            lv_obj_set_style_border_side(obj, border, LV_PART_MAIN); RET_REF
        }

        Derived& setScrollEnable(bool en) {
            lv_obj_set_scrollbar_mode(obj, en ? LV_SCROLLBAR_MODE_ACTIVE : LV_SCROLLBAR_MODE_OFF); RET_REF
        }

        Derived& setLayout(lv_layout_t layout) {
            lv_obj_set_layout(obj, layout); RET_REF
        }

        Derived& setFlexFlow(lv_flex_flow_t flow) {
            lv_obj_set_flex_flow(obj, flow); RET_REF
        }

        Derived& setFlexAlign(lv_flex_align_t mainPlace, lv_flex_align_t crossPlace, lv_flex_align_t trackCrossPlace) {
            lv_obj_set_flex_align(obj, mainPlace, crossPlace, trackCrossPlace); RET_REF
        }

        Derived& addFlag(lv_obj_flag_t flag) {
            lv_obj_add_flag(obj, flag); RET_REF
        }

        Derived& removeFlag(lv_obj_flag_t flag) {
            lv_obj_remove_flag(obj, flag); RET_REF
        }

        Derived& setPadAll(int pad) {
            lv_obj_set_style_pad_all(obj, pad, LV_PART_MAIN); RET_REF
        }

        Derived& setPadTop(int pad) {
            lv_obj_set_style_pad_top(obj, pad, LV_PART_MAIN); RET_REF
        }

        Derived& setPadBottom(int pad) {
            lv_obj_set_style_pad_bottom(obj, pad, LV_PART_MAIN); RET_REF
        }

        Derived& setPadRight(int pad) {
            lv_obj_set_style_pad_right(obj, pad, LV_PART_MAIN); RET_REF
        }

        Derived& setPadLeft(int pad) {
            lv_obj_set_style_pad_left(obj, pad, LV_PART_MAIN); RET_REF
        }

        Derived& setRowPad(uint16_t width) {
            lv_obj_set_style_pad_row(obj, width, 0); RET_REF
        }

        Derived& setColumnPad(uint16_t width) {
            lv_obj_set_style_pad_column(obj, width, 0); RET_REF
        }

        Derived& setGradColor(uint32_t color, lv_grad_dir_t dir) {
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(color), 0);
            lv_obj_set_style_bg_grad_dir(obj, dir, 0);
            RET_REF
        }

        Derived& autoHeight(){
            lv_obj_set_height(obj, LV_SIZE_CONTENT); RET_REF
        }

        Derived& autoWidth() {
            lv_obj_set_width(obj, LV_SIZE_CONTENT); RET_REF
        }

        Derived& autoSize() {
            autoHeight().autoWidth(); RET_REF
        }

        uint32_t getHeight() {
            return lv_obj_get_height(obj);
        }

        uint32_t getWidth() {
            return lv_obj_get_width(obj);
        }

        void addEvent(std::function<void(lv_event_t*)> cb, lv_event_code_t code = LV_EVENT_ALL) {
            auto *cbPtr = new std::function<void(lv_event_t*)>(std::move(cb));
            lv_obj_add_event_cb(obj, eventTrampoline, code, cbPtr);
            lv_obj_add_event_cb(this->raw(), [](lv_event_t * e) {
            delete static_cast<std::function<void(lv_event_t*)>*>(lv_event_get_user_data(e));
            }, LV_EVENT_DELETE, cbPtr);
        }

    protected:
        lv_obj_t* obj = nullptr;
    };

    
    class Widget : public object<Widget, lv_obj_create> {
    public:
        Widget(lv_obj_t *parent) : object(parent) {}
    };

    class Image : public object<Image, lv_img_create> {
    public:
        Image(lv_obj_t *parent) : object(parent) {}
        Image &setSrc(std::string_view src) {
            lv_img_set_src(obj, src.data());
            return static_cast<Image&>(*this);
        }
    };

    class TextBox : public object<TextBox, lv_label_create> {
    public:
        TextBox(lv_obj_t *parent) : object(parent) {}
        TextBox &setText(std::string_view text) {
            lv_label_set_text(obj, text.data());
            lv_obj_set_height(obj, LV_SIZE_CONTENT);
            return static_cast<TextBox&>(*this);
        }

        TextBox &setFont(const lv_font_t *font) {
            lv_obj_set_style_text_font(obj, font, 0);
            return static_cast<TextBox&>(*this);
        }

        TextBox &setTextColor(uint32_t color) {
            lv_obj_set_style_text_color(obj, lv_color_hex(color), 0);
            return static_cast<TextBox&>(*this);
        }

        TextBox &setTextBold(bool en) {
            return static_cast<TextBox&>(*this);
        }

        TextBox &setTextAlign(lv_text_align_t align) {
            lv_obj_set_style_text_align(obj, align, 0);
            return static_cast<TextBox&>(*this);
        }

        TextBox &setLongMode(lv_label_long_mode_t mode) {
            lv_label_set_long_mode(obj, mode);
            return static_cast<TextBox&>(*this);
        }

        TextBox &clearText() {
            lv_label_set_text(obj, "");
            return static_cast<TextBox&>(*this);
        }
    };

    class Button : public object<Button, lv_button_create> {
    public:
        Button(lv_obj_t *parent) : object(parent) {
            textbox.reset(new TextBox(this->raw()));
            textbox->align(LV_ALIGN_CENTER);
        }

        TextBox &textBox() {
            return static_cast<TextBox&>(*this->textbox.get());
        }

    private:
        std::unique_ptr<TextBox> textbox;
    };

    class ButtonMatrix : public object<ButtonMatrix, lv_buttonmatrix_create> {
    public:
        ButtonMatrix(lv_obj_t *parent) : object(parent) {}

        ButtonMatrix &setMap(const char * const map[]) {
            lv_buttonmatrix_set_map(obj, map);
            return static_cast<ButtonMatrix&>(*this);
        }

        ButtonMatrix &setCtrl(uint32_t id, lv_buttonmatrix_ctrl_t ctrl) {
            lv_buttonmatrix_set_button_ctrl(obj, id, ctrl);
            return static_cast<ButtonMatrix&>(*this);
        }

        ButtonMatrix &setButtonWidth(uint32_t id, uint32_t width) {
            lv_buttonmatrix_set_button_width(obj, id, width);
            return static_cast<ButtonMatrix&>(*this);
        }
    };

    class SimpleItem : public Button {
        public:
        SimpleItem(lv_obj_t *parent) : Button(parent){}

    };

    class Menu : public object<Menu, lv_obj_create> {
    public:
        Menu(lv_obj_t *parent) : object(parent) {
            setFlexFlow(LV_FLEX_FLOW_COLUMN);
            setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            setPadRight(0).setPadLeft(0);
            setScrollEnable(true);
        }

        Button &appendTextItem(const char *text) {
            auto item = std::unique_ptr<Button>(new Button(raw()));
            item->textBox().setText(text).setTextColor(0x000000);
            item->setSize(LV_PCT(90), LV_PCT(7)).setColumnPad(0).setBgColor(0xf68830).
                setGradColor(0x43cece, LV_GRAD_DIR_RADIAL).setRadius(MENU_RADIUS);
            items.emplace_back(std::move(item));
            return static_cast<Button&>(*items.back().get());
        }

    private:
        std::vector<std::unique_ptr<Button>> items;

    };

    class TextArea : public object<TextArea, lv_textarea_create> {
    public:
        TextArea(lv_obj_t *parent) : object(parent) {
            lv_obj_add_state(obj, LV_STATE_FOCUSED);
            setBgColor(0).setBorderColor(0).
                setBgOpa(LV_OPA_0).setBorderOpa(LV_OPA_100);
            lv_obj_set_style_border_side(obj, (lv_border_side_t)(LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT),
                              LV_PART_MAIN); 
        }

        TextArea &addText(const char *text) {
            lv_textarea_add_text(obj, text);
            return static_cast<TextArea&>(*this);
        }

        TextArea &setText(const char *text) {
            lv_textarea_set_text(obj, text);
            return static_cast<TextArea&>(*this);
        }

        TextArea &setFont(const lv_font_t *font) {
            lv_obj_set_style_text_font(obj, font, 0);
            return static_cast<TextArea&>(*this);
        }

        TextArea &setTextColor(uint32_t color) {
            lv_obj_set_style_text_color(obj, lv_color_hex(color), 0);
            return static_cast<TextArea&>(*this);
        }

        TextArea &setTextAlign(lv_text_align_t align) {
            lv_obj_set_style_text_align(obj, align, 0);
            return static_cast<TextArea&>(*this);
        }

    private:

    };

    class Animation {
        lv_anim_t anim;
    public:
        Animation() { lv_anim_init(&anim); }
        Animation& setExec(lv_obj_t* obj, lv_anim_exec_xcb_t exec_cb) {
            lv_anim_set_exec_cb(&anim, exec_cb);
            lv_anim_set_var(&anim, obj);
            return *this;
        }
        void start() {
            lv_anim_start(&anim);
        }

        void pause() {
            lv_anim_pause(&anim);
        }

        void resume() {
            lv_anim_resume(&anim);
        }

        void del() {
            lv_anim_delete(anim.var, anim.exec_cb);
        }

        Animation& setTime(uint32_t time) {
            lv_anim_set_time(&anim, time);
            return *this;
        }

        Animation& setValue(uint32_t start, uint32_t end) {
            lv_anim_set_values(&anim,start, end);
            return *this;
        }

        Animation& setDuration(uint32_t duration) {
            lv_anim_set_duration(&anim, duration);
            return *this;
        }

        Animation& setRepeatCount(uint32_t count) {
            lv_anim_set_repeat_count(&anim, count);
            return *this;
        }

        Animation& setRepeatDelay(uint32_t delay) {
            lv_anim_set_repeat_delay(&anim, delay);
            return *this;
        }

        Animation& setDelay(uint32_t delay) {
            lv_anim_set_delay(&anim, delay);
            return *this;
        }

        Animation& setEarlyApply(bool state) {
            lv_anim_set_early_apply(&anim, state);
            return *this;
        }
    };





}

#endif