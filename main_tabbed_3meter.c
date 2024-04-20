#include <stdlib.h>
#include <unistd.h>
#include "lvgl/lvgl.h"

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_meter_indicator_t *obj;
    lv_obj_t *parent;
    int32_t min_value;
    int32_t max_value;
} arc_data_t;

typedef struct {
    arc_data_t part[3];
    int count;
} arcs_data_t;

/**********************
 *      VARIABLES
 **********************/

static lv_style_t style_title;

static lv_coord_t tab_height;

static lv_obj_t *meter_water;
static arcs_data_t arcs_water[2];
static arcs_data_t arcs_fuel[1];
static arcs_data_t arcs_voltage[3];
static lv_obj_t *meter_fuel;
static lv_obj_t *meter_voltage;

static const lv_font_t *font_large;
static const lv_font_t *font_normal;

static lv_obj_t *create_meter_box( lv_obj_t *parent ) {
    lv_obj_set_flex_flow( parent, LV_FLEX_FLOW_ROW_WRAP );

    lv_obj_t *cont = lv_obj_create( parent );
    lv_obj_set_height( cont, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow( cont, 1 );

    lv_obj_t *meter = lv_meter_create( cont );
    lv_obj_remove_style( meter, NULL, LV_PART_MAIN );
    lv_obj_remove_style( meter, NULL, LV_PART_INDICATOR );
    lv_obj_set_width( meter, LV_PCT( 100 ));

    return meter;
}

static void meter_animation_callback( void *var, int32_t value ) {
    arcs_data_t *arcs_data = var;
    for ( int i = 0; i < arcs_data->count; i++ ) {
        arc_data_t *arc = &arcs_data->part[ i ];
        if ( value <= arc->min_value ) {
            lv_meter_set_indicator_end_value( arc->parent, arc->obj, arc->min_value );
        } else if ( value >= arc->max_value ) {
            lv_meter_set_indicator_end_value( arc->parent, arc->obj, arc->max_value );
        } else {
            lv_meter_set_indicator_end_value( arc->parent, arc->obj, value );
        }
    }
//    lv_obj_t * label = lv_obj_get_child(meter3, 0);
//    lv_label_set_text_fmt(label, "%"LV_PRId32, v);
}

static void create_arc( lv_obj_t *meter, lv_meter_scale_t *scale, uint16_t width, lv_color_t color, int16_t r_mod,
                        int32_t min_value, int32_t max_value, arcs_data_t *arcs_data ) {
    lv_meter_indicator_t *arc = lv_meter_add_arc( meter, scale, width, color, r_mod );
    lv_meter_set_indicator_start_value( meter, arc, min_value );
    lv_meter_set_indicator_end_value( meter, arc, max_value );

    arc_data_t *arc_data = &arcs_data->part[ arcs_data->count++ ];
    arc_data->obj = arc;
    arc_data->parent = meter;
    arc_data->min_value = min_value;
    arc_data->max_value = max_value;
}

static void
create_arcs( lv_obj_t *meter, lv_meter_scale_t *scale, uint16_t width, lv_palette_t *palettes,
             int16_t r_mod,
             int32_t *values, arcs_data_t *arcs_data, int arc_count, int mod_count ) {
    for ( int mod = 0; mod < mod_count; mod++ ) {
        for ( int i = 0; i < arc_count; i++ ) {
            create_arc( meter, scale, width, lv_palette_main( palettes[ i ] ), mod * -r_mod,
                        values[ i ], values[ i + 1 ], &arcs_data[ mod ] );
        }
    }

    for ( int i = 0; i < arc_count; i++ ) {
        lv_meter_indicator_t *lines = lv_meter_add_scale_lines( meter, scale,
                                                                lv_palette_darken( palettes[ i ], 3 ),
                                                                lv_palette_darken( palettes[ i ], 3 ),
                                                                true, 0 );
        lv_meter_set_indicator_start_value( meter, lines, values[ i ] );
        lv_meter_set_indicator_end_value( meter, lines, values[ i + 1 ] );
    }
}

static lv_obj_t *create_water_meter( lv_obj_t *parent ) {
    lv_meter_scale_t *scale;

    int32_t min_value = 0;
    int32_t red_limit_value = 20;
    int32_t max_value = 100;
    int16_t arc_width = 10;
    int16_t arc_padding = 3;

    lv_obj_t *meter = create_meter_box( parent );

    // https://docs.lvgl.io/8.0/widgets/extra/meter.html

    scale = lv_meter_add_scale( meter );
    lv_meter_set_scale_range( meter, scale, min_value, max_value, 180, 360 - 180 );
    lv_meter_set_scale_ticks( meter, scale, 11, 2, 2 * arc_width + arc_padding, lv_color_white());
    lv_meter_set_scale_major_ticks( meter, scale, 2, 3,
                                    2 * ( arc_width + arc_padding ) + arc_padding,
                                    lv_color_white(), 15 );

    lv_palette_t palettes[] = { LV_PALETTE_RED, LV_PALETTE_GREEN };
    int32_t values[] = { min_value, red_limit_value, max_value };
    create_arcs( meter, scale, arc_width, palettes, arc_width + arc_padding,
                 values, arcs_water, 2, 2 );

    lv_obj_t *legend_1 = lv_label_create( meter );
    lv_label_set_text( legend_1, "víz bal" );
    lv_obj_add_style( legend_1, &style_title, 0 );

    lv_obj_t *legend_2 = lv_label_create( meter );
    lv_label_set_text( legend_2, "víz jobb" );
    lv_obj_add_style( legend_2, &style_title, 0 );

    lv_obj_update_layout( parent );
    lv_coord_t meter_w = lv_obj_get_width( meter );
    lv_obj_set_height( meter, meter_w );

    // https://docs.lvgl.io/8.0/widgets/obj.html?highlight=lv_obj_align
    lv_obj_align( legend_2, LV_ALIGN_CENTER, 0, -font_normal->line_height / 2 );
    lv_obj_align_to( legend_1, legend_2, LV_ALIGN_TOP_LEFT, 0, -font_normal->line_height * 1.5 );

    return meter;
}

static lv_obj_t *create_fuel_meter( lv_obj_t *parent ) {
    lv_meter_scale_t *scale;

    int32_t min_value = 0;
    int32_t red_limit_value = 20;
    int32_t max_value = 100;
    int16_t arc_width = 10;
    int16_t arc_padding = 3;

    lv_obj_t *meter = create_meter_box( parent );

    scale = lv_meter_add_scale( meter );
    lv_meter_set_scale_range( meter, scale, 0, 100, 180, 360 - 180 );
    lv_meter_set_scale_ticks( meter, scale, 21, 2, arc_width, lv_color_white());
    lv_meter_set_scale_major_ticks( meter, scale, 2, 3,
                                    1 * ( arc_width + arc_padding ) + arc_padding,
                                    lv_color_white(), 15 );

    lv_palette_t palettes[] = { LV_PALETTE_RED, LV_PALETTE_GREEN };
    int32_t values[] = { min_value, red_limit_value, max_value };
    create_arcs( meter, scale, arc_width, palettes, arc_width + arc_padding,
                 values, arcs_fuel, 2, 1 );

    lv_obj_t *legend_1 = lv_label_create( meter );
    lv_label_set_text( legend_1, "üzemanyag" );
    lv_obj_add_style( legend_1, &style_title, 0 );

    lv_obj_update_layout( parent );
    lv_coord_t meter_w = lv_obj_get_width( meter );
    lv_obj_set_height( meter, meter_w );

    lv_obj_align( legend_1, LV_ALIGN_CENTER, 0, -font_normal->line_height / 2 );

    return meter;
}

static void
scale_voltage_label_callback( struct _lv_meter_scale_t *scale, char *buf, size_t buf_size, int32_t value_of_line ) {
    lv_snprintf( buf, buf_size, "%.1f", value_of_line / 10.0 );
}

static lv_obj_t *create_voltage_meter( lv_obj_t *parent ) {
    lv_meter_scale_t *scale;

    lv_obj_t *meter = create_meter_box( parent );

    int32_t min_value = 100;
    int32_t red_limit_value = 113;
    int32_t blue_Limit_Value = 137;
    int32_t max_value = 150;
    int16_t arc_width = 10;
    int16_t arc_padding = 3;
    int minor_per_one = 10;

    scale = lv_meter_add_scale( meter );
    lv_meter_set_scale_range( meter, scale, min_value, max_value, 180, 360 - 180 );
    lv_meter_set_scale_ticks( meter, scale,
                              ( max_value - min_value ) / 10 * minor_per_one + 1,
                              1, 3 * arc_width + 2 * arc_padding, lv_color_white());
    lv_meter_set_scale_major_ticks( meter, scale, minor_per_one, 3,
                                    3 * ( arc_width + arc_padding ) + arc_padding,
                                    lv_color_white(), 15 );
    scale->label_callback = scale_voltage_label_callback;

    lv_palette_t palettes[] = { LV_PALETTE_RED, LV_PALETTE_GREEN, LV_PALETTE_BLUE };
    int32_t values[] = { min_value, red_limit_value, blue_Limit_Value, max_value };
    create_arcs( meter, scale, arc_width, palettes, arc_width + arc_padding,
                 values, arcs_voltage, 3, 3 );

    lv_obj_t *legend_1 = lv_label_create( meter );
    lv_label_set_text( legend_1, "motor" );
    lv_obj_add_style( legend_1, &style_title, 0 );

    lv_obj_t *legend_2 = lv_label_create( meter );
    lv_label_set_text( legend_2, "munka 1" );
    lv_obj_add_style( legend_2, &style_title, 0 );

    lv_obj_t *legend_3 = lv_label_create( meter );
    lv_label_set_text( legend_3, "munka 2" );
    lv_obj_add_style( legend_3, &style_title, 0 );

    lv_obj_update_layout( parent );
    lv_coord_t meter_w = lv_obj_get_width( meter );
    lv_obj_set_height( meter, meter_w );

    // https://docs.lvgl.io/8.0/widgets/obj.html?highlight=lv_obj_align
    lv_obj_align( legend_3, LV_ALIGN_CENTER, 0, -font_normal->line_height / 2 );
    lv_obj_align_to( legend_2, legend_3, LV_ALIGN_TOP_LEFT, 0, -font_normal->line_height * 1.5 );
    lv_obj_align_to( legend_1, legend_2, LV_ALIGN_TOP_LEFT, 0, -font_normal->line_height * 1.5 );

    return meter;
}

void tabbed_init_theme() {
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

void tabbed_create_view( void ) {
    // https://docs.lvgl.io/8.0/widgets/extra/tabview.html
    lv_obj_t *tabView = lv_tabview_create( lv_scr_act(), LV_DIR_TOP, tab_height );
//    lv_tabview_set_act(tabView, 0, LV_ANIM_OFF);
    
    lv_obj_t *tab1 = lv_tabview_add_tab( tabView, "Víz" );
    lv_obj_t *tab2 = lv_tabview_add_tab( tabView, "Üzemanyag" );
    lv_obj_t *tab3 = lv_tabview_add_tab( tabView, "Akku" );
    meter_water = create_water_meter( tab1 );
    meter_fuel = create_fuel_meter( tab2 );
    meter_voltage = create_voltage_meter( tab3 );
}

void tabbed_start_animation() {
    lv_anim_t a;

    lv_anim_init( &a );
    lv_anim_set_values( &a, 0, 100 );
    lv_anim_set_repeat_count( &a, LV_ANIM_REPEAT_INFINITE );
    lv_anim_set_exec_cb( &a, meter_animation_callback );
    lv_anim_set_var( &a, &arcs_water[ 0 ] );
    lv_anim_set_time( &a, 1600 );
    lv_anim_set_playback_time( &a, 400 );
    lv_anim_start( &a );

    lv_anim_init( &a );
    lv_anim_set_values( &a, 0, 100 );
    lv_anim_set_repeat_count( &a, LV_ANIM_REPEAT_INFINITE );
    lv_anim_set_exec_cb( &a, meter_animation_callback );
    lv_anim_set_var( &a, &arcs_fuel[ 0 ] );
    lv_anim_set_time( &a, 1600 );
    lv_anim_set_playback_time( &a, 400 );
    lv_anim_start( &a );

    lv_anim_init( &a );
    lv_anim_set_values( &a, 100, 150 );
    lv_anim_set_repeat_count( &a, LV_ANIM_REPEAT_INFINITE );
    lv_anim_set_exec_cb( &a, meter_animation_callback );
    lv_anim_set_var( &a, &arcs_voltage[ 1 ] );
    lv_anim_set_time( &a, 1600 );
    lv_anim_set_playback_time( &a, 400 );
    lv_anim_start( &a );

}