#ifndef WIDGET_H_
#define WIDGET_H_

#include <lvgl.h>
#include <memory>
#include <functional>

namespace ui
{

    template<typename Derived, lv_obj_t* (*CreateFn)(lv_obj_t*)>
    class LvObject {
        #define RET_REF return static_cast<Derived&>(*this);
    public:
        explicit LvObject(lv_obj_t* parent) : obj(CreateFn(parent)) {}
        virtual ~LvObject() {
            if (obj) lv_obj_del(obj);
        }

        lv_obj_t *raw() { return obj; }

        Derived& align(lv_align_t align, int16_t x, int16_t y) {
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


    protected:
        lv_obj_t* obj = nullptr;
    };

    
    class Widget : public LvObject<Widget, lv_obj_create> {
    public:
        Widget(lv_obj_t *parent) : LvObject(parent) {}
    };

    class Image : public LvObject<Image, lv_img_create> {
    public:
        Image(lv_obj_t *parent) : LvObject(parent) {}
        Image &setSrc(const char *src) {
            lv_img_set_src(obj, src);
            return static_cast<Image&>(*this);
        }
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

        Animation& setTime(uint32_t time) {
            lv_anim_set_time(&anim, time);
            return *this;
        }

        Animation& setValue(uint32_t start, uint32_t end) {
            lv_anim_set_values(&anim,start, end);
            return *this;
        }
    };



}

#endif