/*
 * lvgl_Controller.h
 *
 *  Created on: Jan 18, 2025
 *      Author: wawer
 */

#ifndef INC_LVGL_CONTROLLER_H_
#define INC_LVGL_CONTROLLER_H_


#include "ili9341.h"
#include "SPITouchScreen.h"

/**
 * @file lv_port_disp_templ.h
 *
 */

/*Copy this file as "lv_port_disp.h" and set this value to "1" to enable content*/


/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/* Initialize low level display driver */
void lv_port_disp_init(void);

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void);

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void);

//added by waweroIT
void DrawRGBImage(uint16_t width, uint16_t height, const lv_color_t *color_p);



//======================================================================================================================================== Touch Screen obsluga


void lv_port_indev_init(void);




#endif /* INC_LVGL_CONTROLLER_H_ */
