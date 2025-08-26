/*
 * BATTERYInfo.h
 *
 *  Created on: Aug 26, 2024
 *      Author: wawer
 */

#ifndef INC_BATTERYINFO_H_
#define INC_BATTERYINFO_H_
#include <stdint.h>
#include "stdbool.h"


extern volatile bool WasChargedEnough;

typedef enum
{
    NO_BATTERY = 0,
	DISCHARGED_BATTERY = 1,
    OK_BATTERY_LOW = 2,
	OK_BATTERY_MID = 3,
    OK_BATTERY_HIGH = 4,
	FULL_BATTERY = 5
} eBatteryStatus;

typedef enum
{
    UNKNOWN = 0,
	CHARGING_REQUIRED = 1,
    CHARGING_BATTERY = 2,
	CHARGING_END= 3,
	NOT_CHARGING = 4
} eChargingStatus;

//float ReadDCBattVoltageFromADC(uint32_t *Buffer, int size);
float ReadDCBattVoltageFromADC(uint32_t  *Buffer, uint32_t  size);

eBatteryStatus BatteryInformation(float batteryDischarged, float batteryVoltage);


#endif /* INC_BATTERYINFO_H_ */
