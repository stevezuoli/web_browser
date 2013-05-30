#ifndef __COMMON_IMAGEMANAGER_H__
#define __COMMON_IMAGEMANAGER_H__

#include <vector>
#include <string>

namespace ui
{
#define IMAGE_LIST(MACRO_NAME) \
    MACRO_NAME(BACK, "back@kt.png", "back@kp.png")           \
    MACRO_NAME(BACK_GREY, "back_grey@kt.png", "back_grey@kp.png")      \
    MACRO_NAME(BROWSER, "browser@kt.png", "browser@kp.png") \
    MACRO_NAME(BROWSER_GREY, "browser_grey@kt.png", "browser_grey@kp.png")   \
    MACRO_NAME(FORWARD, "forward@kt.png", "forward@kp.png")   \
    MACRO_NAME(FORWARD_GREY, "forward_grey@kt.png", "forward@kp.png")   \
    MACRO_NAME(HOMEPAGE, "homepage@kt.png", "homepage@kp.png")   \
    MACRO_NAME(MENU, "menu@kt.png", "menu@kp.png")   \
    MACRO_NAME(NORMAL_IME_KEY, "ime_key_50@kt.png", "ime_key_64@kp.png")   \
    MACRO_NAME(NORMAL_IME_KEY_PRESSED, "ime_key_50_pressed@kt.png", "ime_key_64_pressed@kp.png")   \
    MACRO_NAME(MID_IME_KEY, "ime_key_79@kt.png", "ime_key_96@kp.png")   \
    MACRO_NAME(MID_IME_KEY_PRESSED, "ime_key_79_pressed@kt.png", "ime_key_96_pressed@kp.png")   \
    MACRO_NAME(IME_KEY_DEL, "ime_key_del@kt.png", "ime_key_del@kp.png")   \
    MACRO_NAME(IME_KEY_DEL_PRESSED, "ime_key_del_pressed@kt.png", "ime_key_del_pressed@kp.png")   \
    MACRO_NAME(IME_KEY_ENTER, "ime_key_enter@kt.png", "ime_key_enter@kp.png")   \
    MACRO_NAME(IME_KEY_ENTER_PRESSED, "ime_key_enter_pressed@kt.png", "ime_key_enter_pressed@kp.png")   \
    MACRO_NAME(IME_KEY_LANG_CN, "ime_key_lang_cn@kt.png", "ime_key_lang_cn@kp.png")   \
    MACRO_NAME(IME_KEY_LANG_CN_PRESSED, "ime_key_lang_cn_pressed@kt.png", "ime_key_lang_cn_pressed@kp.png")   \
    MACRO_NAME(IME_KEY_LANG_EN, "ime_key_lang_en@kt.png", "ime_key_lang_en@kp.png")   \
    MACRO_NAME(IME_KEY_LANG_EN_PRESSED, "ime_key_lang_en_pressed@kt.png", "ime_key_lang_en_pressed@kp.png")   \
    MACRO_NAME(IME_KEY_SPACE, "ime_key_space@kt.png", "ime_key_space@kp.png")   \
    MACRO_NAME(IME_KEY_SPACE_PRESSED, "ime_key_space_pressed@kt.png", "ime_key_space_pressed@kp.png")   \

enum ImageIndex
{
#define IMAGEINDEX(CommandId, srcKt, srcKp) \
    IMAGE_##CommandId,
    IMAGE_LIST(IMAGEINDEX)
#undef IMAGEINDEX
    ImageCount
};//ImageIndex

class ImageManger
{
public:
    static const char* GetImagePath(ImageIndex index);
};//ImageManger
}//ui
#endif//__COMMON_IMAGEMANAGER_H__
