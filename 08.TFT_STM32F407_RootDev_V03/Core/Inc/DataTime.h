/*
 * DataTime.h
 *
 *  Created on: Jan 12, 2025
 *      Author: wawer
 */
#pragma once
#ifndef INC_DATATIME_H_
#define INC_DATATIME_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <string.h> // Potrzebne do strcmp, strcpy
#include <stdio.h>
#include "ESP8266_WiFi.h"
#include "SPITouchScreen.h"

typedef struct {
	int year, month, day, hour, minute, second;
	char DayName[15];
	bool isWinterTime;
} ActualDate;

extern volatile  ActualDate DateNow;

bool ProcessNTPResponse(const uint8_t *response);

void IncrementLocalTime(ActualDate *date);
const char *GetDayOfWeek(int year, int month, int day);
int IsLeapYear(int year);


#endif /* INC_DATATIME_H_ */
