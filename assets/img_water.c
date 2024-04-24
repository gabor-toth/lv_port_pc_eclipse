
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_DUST
#define LV_ATTRIBUTE_IMG_DUST
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_DUST
uint8_t img_water_map[] = {
        
        0xff,0xff,0xff,0x0a,0xff,0xff,0xff,0xf6,

    0x00,0x10,0x00,
    0x01,0xff,0x00,
    0x00,0x10,0x00,
    0x00,0x3c,0x00,
    0x00,0x7c,0x00,
    0x00,0x7c,0x00,
    0x70,0xfe,0x00,
    0x73,0xff,0xc0,
    0x73,0xff,0xf0,
    0x73,0xff,0xf8,
    0x73,0xff,0xf8,
    0x73,0xff,0xfc,
    0x70,0x00,0x7c,
    0x00,0x00,0x7c,
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    0x00,0x00,0x18,
    0x00,0x00,0x38,
    0x00,0x00,0x7c,
    0x00,0x00,0x7e,
    0x00,0x00,0x7e,
    0x00,0x00,0x7e,
    0x00,0x00,0x7c,
    0x00,0x00,0x38,

};

const lv_image_dsc_t img_water = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_I1,
  .header.flags = 0,
  .header.w = 24,
  .header.h = 24,
  .header.stride = 3,
  .data_size = sizeof(img_water_map),
  .data = img_water_map,
};
