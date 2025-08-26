/*
 * waweroBasicFunction.h
 *
 *  Created on: Feb 4, 2024
 *      Author: wawer
 */
#pragma once
#ifndef INC_WAWEROBASICFUNCTION_H_
#define INC_WAWEROBASICFUNCTION_H_

//#include "LoRa.h"

void ToggleLedOnBlackPill();
void ToggleLedOnBlackPill_X_Times(int x);
int ReadStatusRelay1_UPS();
int ReadStatusRelay2_BATTERY_CHARGE();
int ReadStatusRelay3_LineN();
int ReadStatusRelay4_LineL();
void SetRelay1_UPS(int value);
void SetRelay2_BATTERY_CHARGE(int value);
void SetRelay3_LineN(int value);
void SetRelay4_LineL(int value);

int ReadStatusRelay3_MainPower();
int ReadStatusRelay4_InverterPower();
void SetRelaySSR_3_MainPower(int value);
void SetRelaySSR_4_InverterPower(int value);

//float ReadVoltageFromADC(float VoltageOfPowerSensor);// VoltageOfSensor can be 3.3 or 5

//ralated to  LoRa ------------------------------------------------- START
//int InitializeMyLoRa(LoRa *myLoRadev);

//ralated to  delayUS ------------------------------------------------- START
void DelayInit(void);
void DelayUS(uint32_t us);

//ralated to  WarningBeep ------------------------------------------------- START
void WarningBeep(int count, int delay);
#endif /* INC_WAWEROBASICFUNCTION_H_ */
