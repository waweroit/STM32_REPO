/*
 * ZMPT101BHeader.h
 *
 *  Created on: Aug 22, 2024
 *      Author: waweroIT
 */

#ifndef INC_ZMPT101BHEADER_H_
#define INC_ZMPT101BHEADER_H_

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define ADC_SCALE 4095.0  // ADC w STM32 ma rozdzielczość 12-bitowąc
#define VREF 2.6        // Zakładam VREF = 5.0V  lub 3.3V
#define ZEROPOINT 2048    // Środek zakresu dla 12-bitowego ADC

//#define ADC_SCALE 2540.0  // ADC w STM32 ma rozdzielczość 12-bitową
//#define VREF 3.3        // Zakładam VREF = 5.0V  lub 3.3V
//#define ZEROPOINT 1835    // Środek zakresu dla 12-bitowego ADC

#define FREQUENCY 50

void Initialize_ZMPT101B(ADC_HandleTypeDef* _hadc);
int calibrate();
int calibrateLive();
float calibrateVoltage();
void calibrateLowNoiseAndZeroPoint();
void calibrateLowNoisePointFromDMA(uint32_t *Buffer, int size);
void calibrateAverageZeroPointFromDMA(uint32_t *Buffer, int size, int tolerance);
void calibrateZeroPointWithTolerance(uint32_t *Buffer, int size, int tolerance);

float getVoltageDC();
float getVoltageAC(uint16_t frequency);
float getVoltageACFromDMABuff(uint32_t *Buffer, int size, bool adjSensivity);

int checkADCSaturationTest(uint32_t* Buffer, int size);

uint16_t analogRead();
uint32_t micros();
void delayMicroseconds(uint32_t us);

#endif /* INC_ZMPT101BHEADER_H_ */
