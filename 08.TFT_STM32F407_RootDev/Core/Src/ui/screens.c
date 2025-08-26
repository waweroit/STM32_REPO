#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // TopContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.top_container = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 24);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // WiFiLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.wi_fi_label = obj;
                    lv_obj_set_pos(obj, 269, 8);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "WiFi");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // WiFiConnectLed
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    objects.wi_fi_connect_led = obj;
                    lv_obj_set_pos(obj, 301, 10);
                    lv_obj_set_size(obj, 6, 6);
                    lv_led_set_color(obj, lv_color_hex(0xffff0000));
                    lv_led_set_brightness(obj, 255);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // DateTimeTextLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.date_time_text_label = obj;
                    lv_obj_set_pos(obj, 6, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "0000-00-00 00:00");
                    lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // LoRaiLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lo_rai_label = obj;
                    lv_obj_set_pos(obj, 216, 8);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "LoRa");
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // LoraConnectLed
                    lv_obj_t *obj = lv_led_create(parent_obj);
                    objects.lora_connect_led = obj;
                    lv_obj_set_pos(obj, 251, 10);
                    lv_obj_set_size(obj, 6, 6);
                    lv_led_set_color(obj, lv_color_hex(0xffff0000));
                    lv_led_set_brightness(obj, 255);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // BottomContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bottom_container = obj;
            lv_obj_set_pos(obj, 0, 24);
            lv_obj_set_size(obj, 320, 216);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // TabView
                    lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 32);
                    objects.tab_view = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 320, 322);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // InfoTab1
                            lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Info");
                            objects.info_tab1 = obj;
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ONE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_ON);
                            lv_obj_set_scroll_snap_x(obj, LV_DIR_VER);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // BMP280_InsideReadPanel
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.bmp280_inside_read_panel = obj;
                                    lv_obj_set_pos(obj, -7, -4);
                                    lv_obj_set_size(obj, 301, 114);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // BazaLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.baza_label = obj;
                                            lv_obj_set_pos(obj, 84, -6);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Home readings");
                                        }
                                        {
                                            // TemperaturaLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperatura_label = obj;
                                            lv_obj_set_pos(obj, 7, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Temperature:");
                                        }
                                        {
                                            // PressureLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_label = obj;
                                            lv_obj_set_pos(obj, 7, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Pressure:");
                                        }
                                        {
                                            // AltitudeLabeL
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_labe_l = obj;
                                            lv_obj_set_pos(obj, 7, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Altitude:");
                                        }
                                        {
                                            // TemperatureVariableLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperature_variable_label = obj;
                                            lv_obj_set_pos(obj, 179, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // PressureVariableLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_variable_label = obj;
                                            lv_obj_set_pos(obj, 179, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // AltitudeVariableLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_variable_label = obj;
                                            lv_obj_set_pos(obj, 179, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // TemperatureLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperature_label_1 = obj;
                                            lv_obj_set_pos(obj, 253, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "*C");
                                        }
                                        {
                                            // PressureLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_label_1 = obj;
                                            lv_obj_set_pos(obj, 253, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Pa");
                                        }
                                        {
                                            // AltitudeLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_label_1 = obj;
                                            lv_obj_set_pos(obj, 253, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "m");
                                        }
                                    }
                                }
                                {
                                    // BMP280_CarReadPanel
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.bmp280_car_read_panel = obj;
                                    lv_obj_set_pos(obj, -7, 114);
                                    lv_obj_set_size(obj, 301, 114);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // CarLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.car_label_1 = obj;
                                            lv_obj_set_pos(obj, 84, -6);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Car readings");
                                        }
                                        {
                                            // TemperaturaLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperatura_label_1 = obj;
                                            lv_obj_set_pos(obj, 7, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Temperature:");
                                        }
                                        {
                                            // PressureLabel_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_label_2 = obj;
                                            lv_obj_set_pos(obj, 7, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Pressure:");
                                        }
                                        {
                                            // AltitudeLabeL_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_labe_l_1 = obj;
                                            lv_obj_set_pos(obj, 7, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Altitude:");
                                        }
                                        {
                                            // TemperatureVariableLabel_Car
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperature_variable_label_car = obj;
                                            lv_obj_set_pos(obj, 179, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // PressureVariableLabel_Car
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_variable_label_car = obj;
                                            lv_obj_set_pos(obj, 179, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // AltitudeVariableLabel_Car
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_variable_label_car = obj;
                                            lv_obj_set_pos(obj, 179, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "00");
                                        }
                                        {
                                            // TemperatureLabel_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.temperature_label_2 = obj;
                                            lv_obj_set_pos(obj, 253, 22);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "*C");
                                        }
                                        {
                                            // PressureLabel_3
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.pressure_label_3 = obj;
                                            lv_obj_set_pos(obj, 253, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Pa");
                                        }
                                        {
                                            // AltitudeLabel_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.altitude_label_2 = obj;
                                            lv_obj_set_pos(obj, 253, 70);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "m");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // CarSettingTab
                            lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Car");
                            objects.car_setting_tab = obj;
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // CarPanel02
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.car_panel02 = obj;
                                    lv_obj_set_pos(obj, -9, -8);
                                    lv_obj_set_size(obj, 300, 115);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // CarAutoStartSwitch
                                            lv_obj_t *obj = lv_switch_create(parent_obj);
                                            objects.car_auto_start_switch = obj;
                                            lv_obj_set_pos(obj, 181, -1);
                                            lv_obj_set_size(obj, 50, 25);
                                            lv_obj_add_event_cb(obj, action_car_auto_start_switch_pressed, LV_EVENT_RELEASED, (void *)0);
                                        }
                                        {
                                            // CarAutoStartLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.car_auto_start_label = obj;
                                            lv_obj_set_pos(obj, -2, 1);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Auto engine start:");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, 186, 30);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "STOP");
                                        }
                                        {
                                            // CarStartLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.car_start_label = obj;
                                            lv_obj_set_pos(obj, -2, 30);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Car engine status:");
                                        }
                                        {
                                            // CarEngineStartStopButton
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.car_engine_start_stop_button = obj;
                                            lv_obj_set_pos(obj, 13, 53);
                                            lv_obj_set_size(obj, 99, 34);
                                            lv_obj_add_event_cb(obj, action_car_start_stop_engine_button_pressed, LV_EVENT_PRESSED, (void *)0);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text(obj, "Start/Stop");
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                    }
                                }
                                {
                                    // CarPanel01
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.car_panel01 = obj;
                                    lv_obj_set_pos(obj, -9, 110);
                                    lv_obj_set_size(obj, 300, 138);
                                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // AutoStartEngineTimeLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.auto_start_engine_time_label = obj;
                                            lv_obj_set_pos(obj, 0, 5);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Start engine at:");
                                        }
                                        {
                                            // HourLabel
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.hour_label = obj;
                                            lv_obj_set_pos(obj, 174, 5);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "H");
                                        }
                                        {
                                            // MinuteLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.minute_label_1 = obj;
                                            lv_obj_set_pos(obj, 240, 5);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "MIN");
                                        }
                                        {
                                            // DropDownCarHourStartEngineHour
                                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                            objects.drop_down_car_hour_start_engine_hour = obj;
                                            lv_obj_set_pos(obj, 118, -5);
                                            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
                                            lv_dropdown_set_options(obj, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23");
                                            lv_dropdown_set_selected(obj, 7);
                                        }
                                        {
                                            // DropDownCarHourStartEngineMinutes
                                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                            objects.drop_down_car_hour_start_engine_minutes = obj;
                                            lv_obj_set_pos(obj, 188, -5);
                                            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
                                            lv_dropdown_set_options(obj, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n");
                                            lv_dropdown_set_selected(obj, 15);
                                        }
                                        {
                                            // AutoStartEngineTimeLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.auto_start_engine_time_label_1 = obj;
                                            lv_obj_set_pos(obj, 0, 46);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Temp below:");
                                        }
                                        {
                                            // DropDownCarTempBelow
                                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                            objects.drop_down_car_temp_below = obj;
                                            lv_obj_set_pos(obj, 118, 36);
                                            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
                                            lv_dropdown_set_options(obj, "5\n4\n3\n2\n1\n0\n-1\n-2\n-3\n-4\n-5\n-6\n-7\n-8\n-9\n-10");
                                            lv_dropdown_set_selected(obj, 5);
                                        }
                                        {
                                            // AutoStartEngineTimeLabel_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.auto_start_engine_time_label_2 = obj;
                                            lv_obj_set_pos(obj, 0, 89);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "Temp above:");
                                        }
                                        {
                                            // DropDownCarTempAbove
                                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                                            objects.drop_down_car_temp_above = obj;
                                            lv_obj_set_pos(obj, 118, 79);
                                            lv_obj_set_size(obj, 52, LV_SIZE_CONTENT);
                                            lv_dropdown_set_options(obj, "15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30");
                                            lv_dropdown_set_selected(obj, 10);
                                        }
                                        {
                                            // DegreesLabel_1
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.degrees_label_1 = obj;
                                            lv_obj_set_pos(obj, 174, 45);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "*C");
                                        }
                                        {
                                            // DegreesLabel_2
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.degrees_label_2 = obj;
                                            lv_obj_set_pos(obj, 174, 89);
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_text(obj, "*C");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // LoraSettingTab
                            lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "LoRa");
                            objects.lora_setting_tab = obj;
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // LoraPanel
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.lora_panel = obj;
                                    lv_obj_set_pos(obj, -7, -8);
                                    lv_obj_set_size(obj, 301, 175);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // LoRaDevReset
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.lo_ra_dev_reset = obj;
                                            lv_obj_set_pos(obj, 88, 49);
                                            lv_obj_set_size(obj, 100, 50);
                                            lv_obj_add_event_cb(obj, action_lo_ra_reset_button_pressed, LV_EVENT_PRESSED, (void *)0);
                                            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // LoRaDevResetLabel
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.lo_ra_dev_reset_label = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text(obj, "Reset");
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // WifiSettingTab
                            lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "WiFi");
                            objects.wifi_setting_tab = obj;
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // WiFiPanel_1
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.wi_fi_panel_1 = obj;
                                    lv_obj_set_pos(obj, -7, -8);
                                    lv_obj_set_size(obj, 301, 175);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // WifiReset
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.wifi_reset = obj;
                                            lv_obj_set_pos(obj, 94, 20);
                                            lv_obj_set_size(obj, 100, 50);
                                            lv_obj_add_event_cb(obj, action_wi_fi_reset_button_pressed, LV_EVENT_PRESSED, (void *)0);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // WifiDevResetLabel
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.wifi_dev_reset_label = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text(obj, "Reset");
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                        {
                                            // WifiDisable
                                            lv_obj_t *obj = lv_btn_create(parent_obj);
                                            objects.wifi_disable = obj;
                                            lv_obj_set_pos(obj, 94, 77);
                                            lv_obj_set_size(obj, 100, 50);
                                            lv_obj_add_event_cb(obj, action_wi_fi_disable_button_pressed, LV_EVENT_PRESSED, (void *)0);
                                            {
                                                lv_obj_t *parent_obj = obj;
                                                {
                                                    // WifiDevDisableLabel
                                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                                    objects.wifi_dev_disable_label = obj;
                                                    lv_obj_set_pos(obj, 0, 0);
                                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                                    lv_label_set_text(obj, "Disable");
                                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void tick_screen_main() {
}


void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
