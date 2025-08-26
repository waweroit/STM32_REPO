#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_car_start_stop_engine_button_pressed(lv_event_t * e);
extern void action_lo_ra_reset_button_pressed(lv_event_t * e);
extern void action_wi_fi_reset_button_pressed(lv_event_t * e);
extern void action_wi_fi_disable_button_pressed(lv_event_t * e);
extern void action_car_auto_start_switch_pressed(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/