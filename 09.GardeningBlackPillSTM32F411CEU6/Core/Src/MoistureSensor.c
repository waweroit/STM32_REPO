/*
 * MoistureSensor.c
 *
 *  Created on: Jul 14, 2025
 *      Author: wawer
 */


#include "MoistureSensor.h"



int ScaleADC_Light_To_Percentage(const uint32_t *adc_buffer, int size, int adc_dark, int adc_bright)
{
    if (adc_buffer == NULL || size <= 0 || adc_bright <= adc_dark)
        return 0;

    uint32_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += adc_buffer[i];
    }

    int avg = sum / size;

    // Ogranicz średnią do zakresu [adc_dark, adc_bright]
    if (avg < adc_dark) avg = adc_dark;
    if (avg > adc_bright) avg = adc_bright;

    int range = adc_bright - adc_dark;
    int delta = avg - adc_dark;

    int percent = (delta * 100) / range;

    // Zabezpieczenie
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;

    return percent;
}

//dray- 2900/2500
//wet 1790/1700
int ScaleADC_To_Percent_Inverted_Ranged(const uint32_t *adc_buffer, int size, int dry_value, int wet_value)
{
    if (adc_buffer == NULL || size <= 0 )// || dry_value <= wet_value)
        return 0;

    uint32_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += adc_buffer[i];
    }

    int avg = sum / size;

    // Ogranicz średnią do zakresu [wet_value, dry_value]
//    if (avg > dry_value)
//    	avg = dry_value;
//    if (avg < wet_value)
//    	avg = wet_value;

    int range = dry_value - wet_value;
    int delta = avg - wet_value;

    int percent = 100 - ((delta * 100) / range);

    // Graniczne zabezpieczenie
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;

    return percent;
}
