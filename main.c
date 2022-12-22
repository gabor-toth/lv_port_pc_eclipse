
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/sdl/sdl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void meter_create();

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

static disp_size_t disp_size;

static lv_obj_t * tv;

static lv_style_t style_title;
static lv_style_t style_bullet;

static lv_obj_t * meter1;
static lv_obj_t * meter2;
static lv_obj_t * meter3;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

//  lv_example_switch_1();
//  lv_example_calendar_1();
//  lv_example_btnmatrix_2();
//  lv_example_checkbox_1();
//  lv_example_colorwheel_1();
//  lv_example_chart_6();
//  lv_example_table_2();
//  lv_example_scroll_2();
//  lv_example_textarea_1();
//  lv_example_msgbox_1();
//  lv_example_dropdown_2();
//  lv_example_btn_1();
//  lv_example_scroll_1();
//  lv_example_tabview_1();
//  lv_example_tabview_1();
//  lv_example_flex_3();
//  lv_example_label_1();

//    lv_demo_widgets();
  meter_create();

  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(5 * 1000);
  }

  return 0;
}

static lv_obj_t * create_meter_box(lv_obj_t * parent, const char * title, const char * text1, const char * text2,
                                   const char * text3)
{
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_height(cont, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(cont, 1);

//    lv_obj_t * title_label = lv_label_create(cont);
//    lv_label_set_text(title_label, title);
//    lv_obj_add_style(title_label, &style_title, 0);

    lv_obj_t * meter = lv_meter_create(cont);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_set_width(meter, LV_PCT(100));

//    lv_obj_t * bullet1 = lv_obj_create(cont);
//    lv_obj_set_size(bullet1, 13, 13);
//    lv_obj_remove_style(bullet1, NULL, LV_PART_SCROLLBAR);
//    lv_obj_add_style(bullet1, &style_bullet, 0);
//    lv_obj_set_style_bg_color(bullet1, lv_palette_main(LV_PALETTE_RED), 0);
//    lv_obj_t * label1 = lv_label_create(cont);
//    lv_label_set_text(label1, text1);
//
//    lv_obj_t * bullet2 = lv_obj_create(cont);
//    lv_obj_set_size(bullet2, 13, 13);
//    lv_obj_remove_style(bullet2, NULL, LV_PART_SCROLLBAR);
//    lv_obj_add_style(bullet2, &style_bullet, 0);
//    lv_obj_set_style_bg_color(bullet2, lv_palette_main(LV_PALETTE_BLUE), 0);
//    lv_obj_t * label2 = lv_label_create(cont);
//    lv_label_set_text(label2, text2);
//
//    lv_obj_t * bullet3 = lv_obj_create(cont);
//    lv_obj_set_size(bullet3, 13, 13);
//    lv_obj_remove_style(bullet3,  NULL, LV_PART_SCROLLBAR);
//    lv_obj_add_style(bullet3, &style_bullet, 0);
//    lv_obj_set_style_bg_color(bullet3, lv_palette_main(LV_PALETTE_GREEN), 0);
//    lv_obj_t * label3 = lv_label_create(cont);
//    lv_label_set_text(label3, text3);

    if(disp_size == DISP_MEDIUM) {
        static lv_coord_t grid_col_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_FR(8), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t grid_row_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_set_grid_dsc_array(cont, grid_col_dsc, grid_row_dsc);
//        lv_obj_set_grid_cell(title_label, LV_GRID_ALIGN_START, 0, 4, LV_GRID_ALIGN_START, 0, 1);
        lv_obj_set_grid_cell(meter, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 1, 3);
//        lv_obj_set_grid_cell(bullet1, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 2, 1);
//        lv_obj_set_grid_cell(bullet2, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 3, 1);
//        lv_obj_set_grid_cell(bullet3, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 4, 1);
//        lv_obj_set_grid_cell(label1, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 2, 1);
//        lv_obj_set_grid_cell(label2, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 3, 1);
//        lv_obj_set_grid_cell(label3, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 4, 1);
    }
    else {
        static lv_coord_t grid_col_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static lv_coord_t grid_row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(cont, grid_col_dsc, grid_row_dsc);
//        lv_obj_set_grid_cell(title_label, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_START, 0, 1);
        lv_obj_set_grid_cell(meter, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_START, 1, 1);
//        lv_obj_set_grid_cell(bullet1, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 2, 1);
//        lv_obj_set_grid_cell(bullet2, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 3, 1);
//        lv_obj_set_grid_cell(bullet3, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 4, 1);
//        lv_obj_set_grid_cell(label1, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 2, 1);
//        lv_obj_set_grid_cell(label2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 3, 1);
//        lv_obj_set_grid_cell(label3, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 4, 1);
    }


    return meter;

}

static void meter_create() {
    lv_meter_scale_t * scale;
    lv_meter_indicator_t * arc;
    lv_meter_indicator_t * indic;

    if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
    else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
    else disp_size = DISP_LARGE;

    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    lv_coord_t tab_h;
    if(disp_size == DISP_LARGE) {
        tab_h = 70;
#if LV_FONT_MONTSERRAT_24
        font_large     = &lv_font_montserrat_24;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_24 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_16
        font_normal    = &lv_font_montserrat_16;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_16 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else if(disp_size == DISP_MEDIUM) {
        tab_h = 45;
#if LV_FONT_MONTSERRAT_20
        font_large     = &lv_font_montserrat_20;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_20 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_14
        font_normal    = &lv_font_montserrat_14;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_14 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else {   /* disp_size == DISP_SMALL */
        tab_h = 45;
#if LV_FONT_MONTSERRAT_18
        font_large     = &lv_font_montserrat_18;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_12
        font_normal    = &lv_font_montserrat_12;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }

#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);
#endif

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);

    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, 0);
    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);

    lv_obj_set_style_text_font(lv_scr_act(), font_normal, 0);

    lv_obj_t * parent = lv_scr_act();
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);

    meter3 = create_meter_box(parent, "Network Speed", "Low speed", "Normal Speed", "High Speed");
    if(disp_size < DISP_LARGE) {
    	lv_obj_add_flag(lv_obj_get_parent(meter3), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    /*Add a special circle to the needle's pivot*/
//    lv_obj_set_style_pad_hor(meter3, 10, 0);
//    lv_obj_set_style_size(meter3, 10, LV_PART_INDICATOR);
//    lv_obj_set_style_radius(meter3, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
//    lv_obj_set_style_bg_opa(meter3, LV_OPA_COVER, LV_PART_INDICATOR);
//    lv_obj_set_style_bg_color(meter3, lv_palette_darken(LV_PALETTE_GREY, 4), LV_PART_INDICATOR);
//    lv_obj_set_style_outline_color(meter3, lv_color_white(), LV_PART_INDICATOR);
//    lv_obj_set_style_outline_width(meter3, 3, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(meter3, lv_palette_darken(LV_PALETTE_GREY, 1), LV_PART_TICKS);

    // https://docs.lvgl.io/8.0/widgets/extra/meter.html

    scale = lv_meter_add_scale(meter3);
    lv_meter_set_scale_range(meter3, scale, 0, 100, 180, 360 - 180);
    lv_meter_set_scale_ticks(meter3, scale, 21, 3, 17, lv_color_white());
    lv_meter_set_scale_major_ticks(meter3, scale, 2, 4, 22, lv_color_white(), 15);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter3, arc, 0);
    lv_meter_set_indicator_end_value(meter3, arc, 20);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(meter3, arc, 20);
    lv_meter_set_indicator_end_value(meter3, arc, 40);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_meter_set_indicator_start_value(meter3, arc, 40);
    lv_meter_set_indicator_end_value(meter3, arc, 100);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_RED), -15);
    lv_meter_set_indicator_start_value(meter3, arc, 0);
    lv_meter_set_indicator_end_value(meter3, arc, 20);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_BLUE), -15);
    lv_meter_set_indicator_start_value(meter3, arc, 20);
    lv_meter_set_indicator_end_value(meter3, arc, 40);

    arc = lv_meter_add_arc(meter3, scale, 10, lv_palette_main(LV_PALETTE_GREEN), -15);
    lv_meter_set_indicator_start_value(meter3, arc, 40);
    lv_meter_set_indicator_end_value(meter3, arc, 100);

    indic = lv_meter_add_scale_lines(meter3, scale,
                                     lv_palette_darken(LV_PALETTE_RED, 3),
                                     lv_palette_darken(LV_PALETTE_RED, 3),
                                     true,
                                     0);
    lv_meter_set_indicator_start_value(meter3, indic, 0);
    lv_meter_set_indicator_end_value(meter3, indic, 20);


    indic = lv_meter_add_scale_lines(meter3, scale, lv_palette_darken(LV_PALETTE_BLUE, 3),
                                     lv_palette_darken(LV_PALETTE_BLUE, 3), true, 0);
    lv_meter_set_indicator_start_value(meter3, indic, 20);
    lv_meter_set_indicator_end_value(meter3, indic, 40);


    indic = lv_meter_add_scale_lines(meter3, scale, lv_palette_darken(LV_PALETTE_GREEN, 3),
                                     lv_palette_darken(LV_PALETTE_GREEN, 3), true, 0);
    lv_meter_set_indicator_start_value(meter3, indic, 40);
    lv_meter_set_indicator_end_value(meter3, indic, 100);

//    indic = lv_meter_add_needle_line(meter3, scale, 4, lv_palette_darken(LV_PALETTE_GREY, 4), -25);

//    lv_obj_t * mbps_label = lv_label_create(meter3);
//    lv_label_set_text(mbps_label, "-");
//    lv_obj_add_style(mbps_label, &style_title, 0);

//    lv_obj_t * mbps_unit_label = lv_label_create(meter3);
//    lv_label_set_text(mbps_unit_label, "Mbps");

//    lv_anim_init(&a);
//    lv_anim_set_values(&a, 10, 60);
//    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//    lv_anim_set_exec_cb(&a, meter3_anim_cb);
//    lv_anim_set_var(&a, indic);
//    lv_anim_set_time(&a, 4100);
//    lv_anim_set_playback_time(&a, 800);
//    lv_anim_start(&a);

    lv_obj_update_layout(parent);
    if(disp_size == DISP_MEDIUM) {
//        lv_obj_set_size(meter1, 200, 200);
//        lv_obj_set_size(meter2, 200, 200);
        lv_obj_set_size(meter3, 200, 200);
    }
    else {
        lv_coord_t meter_w = lv_obj_get_width(meter3);
//        lv_obj_set_height(meter1, meter_w);
//        lv_obj_set_height(meter2, meter_w);
        lv_obj_set_height(meter3, meter_w);
//        lv_obj_set_size(meter3, 300, 240);
    }

//    lv_obj_align(mbps_label, LV_ALIGN_TOP_MID, 10, lv_pct(55));
//    lv_obj_align_to(mbps_unit_label, mbps_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_disp_draw_buf_t disp_buf1;
  static lv_color_t buf1_1[SDL_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, SDL_HOR_RES * 100);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;

  lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

  lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
  lv_disp_set_theme(disp, th);

  lv_group_t * g = lv_group_create();
  lv_group_set_default(g);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  static lv_indev_drv_t indev_drv_1;
  lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
  indev_drv_1.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  indev_drv_1.read_cb = sdl_mouse_read;
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

  static lv_indev_drv_t indev_drv_2;
  lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
  indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_2.read_cb = sdl_keyboard_read;
  lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
  lv_indev_set_group(kb_indev, g);

  static lv_indev_drv_t indev_drv_3;
  lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
  indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
  indev_drv_3.read_cb = sdl_mousewheel_read;
  lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
  lv_indev_set_group(enc_indev, g);

  /*Set a cursor for the mouse*/
  LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/
}
