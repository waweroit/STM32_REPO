/*
 * actions.c
 *
 *  Created on: Jan 19, 2025
 *      Author: wawer
 */
#include "ui.h"
#include "actions.h"
#include "screens.h"
#include "vars.h"
#include "ili9341.h"
#include "File_Handling.h"
#include <stdbool.h>
#include "BMP280.h"

void action_car_start_stop_engine_button_pressed(lv_event_t * e)
{
	SendMessageRequest(4707, ReqRunServoIgnition); // powinno zwrocic ReqCarStatus czy uruchomiony
}

void action_lo_ra_reset_button_pressed(lv_event_t * e)
{

}

void action_wi_fi_reset_button_pressed(lv_event_t * e)
{

}

void action_wi_fi_disable_button_pressed(lv_event_t * e)
{

}

void action_car_auto_start_switch_pressed(lv_event_t * e)
{

}

void SetWifiStatusFlagsLVGL()
{
	if(ESP8266_isConnected)
	{
        lv_led_set_color(objects.wi_fi_connect_led, lv_color_hex(0xf08ff000));//#08ff00
	}
	else
	{
        lv_led_set_color(objects.wi_fi_connect_led, lv_color_hex(0xffff0000));
	}
}

void SetLoRaStatusFlagsLVGL()
{
	if(LoRa_isWorking)
	{
        lv_led_set_color(objects.lora_connect_led, lv_color_hex(0xf08ff000));//#08ff00
	}
	else
	{
        lv_led_set_color(objects.lora_connect_led, lv_color_hex(0xffff0000));
	}
}

void SetDateTimeLVGL()
{
	char DateTimeText[60];
	sprintf(DateTimeText,"%04d-%02d-%02d %02d:%02d", DateNow.year, DateNow.month, DateNow.day, DateNow.hour, DateNow.minute);
    lv_label_set_text(objects.date_time_text_label, DateTimeText);
}

void SetLocalTempHumPressureLVGL()
{
	char Data[20];
	sprintf(Data,"%3.1f", BMP280_temperature + tempCalibration);
    lv_label_set_text(objects.temperature_variable_label, Data);
    memset(Data, 0, sizeof(Data));

	sprintf(Data,"%.2f", (BMP280_pressure / 100.0f ) + pressureCalibration);
    lv_label_set_text(objects.pressure_variable_label, Data);
}
