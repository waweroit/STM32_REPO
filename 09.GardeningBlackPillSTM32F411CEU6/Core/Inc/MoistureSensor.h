/*
 * MoistureSensor.h
 *
 *  Created on: Jul 14, 2025
 *      Author: wawer
 */

#ifndef INC_MOISTURESENSOR_H_
#define INC_MOISTURESENSOR_H_

#include "main.h"

#define ADC_SCALE 4095.0  // ADC w STM32 ma rozdzielczość 12-bitowąc
#define VREF 2.6        // Zakładam VREF = 5.0V  lub 3.3V
#define ZEROPOINT 2048    // Środek zakresu dla 12-bitowego ADC


int ScaleADC_Light_To_Percentage(const uint32_t *adc_buffer, int size, int adc_dark, int adc_bright);
int ScaleADC_To_Percent_Inverted_Ranged(const uint32_t *adc_buffer, int size, int dry_value, int wet_value);
#endif /* INC_MOISTURESENSOR_H_ */
