/*
 * waweroBasicFunction.h
 *
 *  Created on: october 27, 2024
 *      Author: wawer
 */
#pragma once
#ifndef INC_WAWEROBASICFUNCTION_H_
#define INC_WAWEROBASICFUNCTION_H_
#include "LoRa.h"

void ToggleLed();

//ralated to  delayUS ------------------------------------------------- START
void DelayInit(void);
void DelayUS(uint32_t us);

int InitializeMyLoRa(LoRa *myLoRadev);

void PrintImage (int index);
#endif /* INC_WAWEROBASICFUNCTION_H_ */
