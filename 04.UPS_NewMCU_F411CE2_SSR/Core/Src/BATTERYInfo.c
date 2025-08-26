/*
 * BATTERYInfo.c
 *
 *  Created on: Aug 26, 2024
 *      Author: wawer
 */
#include "BATTERYInfo.h"

volatile bool WasChargedEnough = false;

float ReadDCBattVoltageFromADC(uint32_t  *Buffer, uint32_t  size)// VoltageOfSensor can be 3.3 or 5
{
    if (!Buffer || size == 0) {
        return 0.0f;
    }

    // uśrednienie ADC
    uint32_t sum = 0;
    for (uint32_t i = 0; i < size; ++i) {
        sum += Buffer[i];
    }
    float avg = (float)sum / size;

	//voltage =(float)readValue/4095*(37.5/7.5*VoltageOfSensor); 37,5 oraz 7.5 to rezystory na płytce PCB
	//voltage =(float)readValue/4095*5*VoltageOfPowerSensor;
	//voltage =(float)readValue/4095*(37.5/7.5*VoltageOfSensor); 37,5 oraz 7.5 to rezystory na płytce PCB
    float voltage =(float)avg/4095*16.5; //(dla 3.3 volta)
//	    voltage =(float)readValue/4095*25; //(dla 5.0 volta)
	if(voltage <= 0.9)
	    	voltage = 0;
	return voltage;

}


eBatteryStatus BatteryInformation(float batteryDischarged, float batteryVoltage)
{
	//float batteryDischarged = 11.2;
	eBatteryStatus batStatus = NO_BATTERY;

	if(batteryVoltage <= batteryDischarged)
		batStatus = DISCHARGED_BATTERY;
	else if(batteryVoltage > batteryDischarged && batteryVoltage <= 12.6)
		batStatus = OK_BATTERY_LOW;
	else if(batteryVoltage > 12.6 && batteryVoltage <= 13.4)
		batStatus = OK_BATTERY_MID;
	else if(batteryVoltage > 13.4 && batteryVoltage <= 13.8)
		batStatus = OK_BATTERY_HIGH;
	else if(batteryVoltage >= 14.0)
		batStatus = FULL_BATTERY;

	return batStatus;
}
