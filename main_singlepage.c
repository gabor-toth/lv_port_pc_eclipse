#include <stdlib.h>
#include <unistd.h>
#include "lvgl/lvgl.h"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

static lv_style_t style_title;

static lv_coord_t tab_height;

static lv_obj_t *meter_water;
static lv_obj_t *meter_fuel;
static lv_obj_t *meter_voltage;

static const lv_font_t *font_large;
static const lv_font_t *font_normal;

void singlepage_init_theme() {
    font_large = &rubik_18_subpx;
    font_normal = &rubik_12_subpx;
    
    tab_height = font_normal->line_height * 2.5;
    
    // color palette:
    // https://docs.lvgl.io/master/overview/color.html#palette -> https://vuetifyjs.com/en/styles/colors/#material-colorss
    lv_theme_default_init(NULL,
                          lv_palette_main( LV_PALETTE_LIGHT_GREEN ),
                          lv_palette_main( LV_PALETTE_LIME ),
                          1,
                          font_normal );
    
    lv_style_init( &style_title );
    lv_style_set_text_font( &style_title, font_large );
    
    lv_obj_set_style_text_font( lv_scr_act(), font_normal, 0 );
}

static void set_temp(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

void lv_example_bar_3(void)
{
    static lv_style_t style_indic;
    
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);
    
    lv_obj_t * bar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 20, 200);
    lv_obj_center(bar);
    lv_bar_set_range(bar, -20, 40);
    
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 3000);
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, -20, 40);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void singlepage_create_view( void ) {
    lv_example_bar_3();
}

void singlepage_start_animation() {
}