#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *top_container;
    lv_obj_t *wi_fi_label;
    lv_obj_t *wi_fi_connect_led;
    lv_obj_t *date_time_text_label;
    lv_obj_t *lo_rai_label;
    lv_obj_t *lora_connect_led;
    lv_obj_t *bottom_container;
    lv_obj_t *tab_view;
    lv_obj_t *info_tab1;
    lv_obj_t *bmp280_inside_read_panel;
    lv_obj_t *baza_label;
    lv_obj_t *temperatura_label;
    lv_obj_t *pressure_label;
    lv_obj_t *altitude_labe_l;
    lv_obj_t *temperature_variable_label;
    lv_obj_t *pressure_variable_label;
    lv_obj_t *altitude_variable_label;
    lv_obj_t *temperature_label_1;
    lv_obj_t *pressure_label_1;
    lv_obj_t *altitude_label_1;
    lv_obj_t *bmp280_car_read_panel;
    lv_obj_t *car_label_1;
    lv_obj_t *temperatura_label_1;
    lv_obj_t *pressure_label_2;
    lv_obj_t *altitude_labe_l_1;
    lv_obj_t *temperature_variable_label_car;
    lv_obj_t *pressure_variable_label_car;
    lv_obj_t *altitude_variable_label_car;
    lv_obj_t *temperature_label_2;
    lv_obj_t *pressure_label_3;
    lv_obj_t *altitude_label_2;
    lv_obj_t *car_setting_tab;
    lv_obj_t *car_panel02;
    lv_obj_t *car_auto_start_switch;
    lv_obj_t *car_auto_start_label;
    lv_obj_t *car_start_label;
    lv_obj_t *car_engine_start_stop_button;
    lv_obj_t *car_panel01;
    lv_obj_t *auto_start_engine_time_label;
    lv_obj_t *hour_label;
    lv_obj_t *minute_label_1;
    lv_obj_t *drop_down_car_hour_start_engine_hour;
    lv_obj_t *drop_down_car_hour_start_engine_minutes;
    lv_obj_t *auto_start_engine_time_label_1;
    lv_obj_t *drop_down_car_temp_below;
    lv_obj_t *auto_start_engine_time_label_2;
    lv_obj_t *drop_down_car_temp_above;
    lv_obj_t *degrees_label_1;
    lv_obj_t *degrees_label_2;
    lv_obj_t *lora_setting_tab;
    lv_obj_t *lora_panel;
    lv_obj_t *lo_ra_dev_reset;
    lv_obj_t *lo_ra_dev_reset_label;
    lv_obj_t *wifi_setting_tab;
    lv_obj_t *wi_fi_panel_1;
    lv_obj_t *wifi_reset;
    lv_obj_t *wifi_dev_reset_label;
    lv_obj_t *wifi_disable;
    lv_obj_t *wifi_dev_disable_label;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/