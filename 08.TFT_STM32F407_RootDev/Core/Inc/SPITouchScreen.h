/*
 * SPITouchScreen.h
 *
 *  Created on: Nov 17, 2024
 *      Author: wawer
 */
#pragma once
#ifndef INC_SPITOUCHSCREEN_H_
#define INC_SPITOUCHSCREEN_H_
#include "main.h"
#include <stdbool.h>

#define NUM_COORDINATES 14

#define TouchScreen_ON() HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET)
#define TouchScreen_OFF() HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET)

#define XPT2046_CMD_X 0xD0  // Komenda odczytu X org
#define XPT2046_CMD_Y 0x90  // Komenda odczytu Y org

#define TouchCS_Port TOUCH_CS_GPIO_Port
#define TouchCS_Pin TOUCH_CS_Pin
// nie wiem dokladnie jakie (?) 511 3792 .. 960 3072 to zależy od predkosci jaka sie ustawi...
#define TouchPointXStart 290
#define TouchPointXEnd 3100
#define TouchPointYStart 300
#define TouchPointYEnd 3100
#define XPT2046_ScrHigh 320
#define XPT2046_ScrWidth 240

extern volatile uint8_t TSInterrupt;
extern volatile int IsReceivedNewCoords;
extern volatile uint16_t touch_x;
extern volatile uint16_t touch_y;

void initTouchScr();
uint16_t ReadTouchCoordinate(uint8_t command);
void Touch_ReadCoordinates(uint16_t* x, uint16_t* y);
void XPT2046_ReadCoordinates(uint16_t* x, uint16_t* y);


bool CheckCoordinatesWithinRadius(int FromPointX, int FromPointY, int InRadius, int PointXInRange, int PointYInRange);
bool CheckCoordinatesWithinRectangle(int FromPointX, int FromPointY, int RectangleWidth, int RectangleHeight, int PointXInRange, int PointYInRange);
int scaleBetween(int value, int oldMin, int oldMax, int newMin, int newMax);
void sortArray(uint16_t arr[], uint16_t size);
void TouchInterrupt(volatile uint16_t *touch_x, volatile uint16_t *touch_y, volatile int *IsReceivedNewCoords);

void DrawRectangle(uint16_t color,int cx, int cy, int width, int hight);

void testRectsWawero(uint16_t color, int width, int hight, int pointerSensivitySize);
void testCircleWawero(uint16_t color, int radious, int pointerSensivitySize);

#endif /* INC_SPITOUCHSCREEN_H_ */
