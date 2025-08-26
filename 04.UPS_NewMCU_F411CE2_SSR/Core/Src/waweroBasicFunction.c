/*
 * waweroBasicFunction.c
 *
 *  Created on: Feb 4, 2024
 *      Author: wawer
 */
#include <stdio.h>
#include "main.h"
//#include "adc.h"
#include "waweroBasicFunction.h"

void ToggleLedOnBlackPill()
{
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);  1 jako ON 0 jako OFF

	  HAL_GPIO_TogglePin(LED_Pill_GPIO_Port, LED_Pill_Pin);
}

void ToggleLedOnBlackPill_X_Times(int x)
{
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);  1 jako ON 0 jako OFF

	if(x != 0)
	{
		for(int i = x; i > 0 ;i--)
		{
			HAL_GPIO_TogglePin(LED_Pill_GPIO_Port, LED_Pill_Pin);
			HAL_Delay(700);
		}
	}else
	{
		HAL_GPIO_TogglePin(LED_Pill_GPIO_Port, LED_Pill_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(LED_Pill_GPIO_Port, LED_Pill_Pin);
		HAL_Delay(100);
	}


}

int ReadStatusRelay1_UPS()
{
	GPIO_PinState state = HAL_GPIO_ReadPin(Relay1_UPS_GPIO_Port, Relay1_UPS_Pin);
	if(state == GPIO_PIN_RESET)
	{
		return 1;
	}
	else if(state == GPIO_PIN_SET)
	{
		return 0;
	}
	else
		return 0;

}

int ReadStatusRelay2_BATTERY_CHARGE()
{
	GPIO_PinState state = HAL_GPIO_ReadPin(Relay2_BATT_GPIO_Port, Relay2_BATT_Pin);
	if(state == GPIO_PIN_RESET)
	{
		return 1;
	}
	else if(state == GPIO_PIN_SET)
	{
		return 0;
	}
	else
		return 0;
}

//int ReadStatusRelay3_LineN()
//{
//	GPIO_PinState state = HAL_GPIO_ReadPin(Relay3_LineN_GPIO_Port, Relay3_LineN_Pin);
//	if(state == GPIO_PIN_RESET)
//	{
//		return 1;
//	}
//	else if(state == GPIO_PIN_SET)
//	{
//		return 0;
//	}
//	else
//		return 0;
//}
//
//int ReadStatusRelay4_LineL()
//{
//	GPIO_PinState state = HAL_GPIO_ReadPin(Relay4_LineL_GPIO_Port, Relay4_LineL_Pin);
//	if(state == GPIO_PIN_RESET)
//	{
//		return 1;
//	}
//	else if(state == GPIO_PIN_SET)
//	{
//		return 0;
//	}
//	else
//		return 0;
//}


void SetRelay1_UPS(int value)
{
	if(value == 1)
		HAL_GPIO_WritePin(Relay1_UPS_GPIO_Port, Relay1_UPS_Pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(Relay1_UPS_GPIO_Port, Relay1_UPS_Pin, GPIO_PIN_SET);
}

void SetRelay2_BATTERY_CHARGE(int value)
{
	if(value == 1)
		HAL_GPIO_WritePin(Relay2_BATT_GPIO_Port, Relay2_BATT_Pin, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(Relay2_BATT_GPIO_Port, Relay2_BATT_Pin, GPIO_PIN_SET);
}

//void SetRelay3_LineN(int value)
//{
//	if(value == 1)
//		HAL_GPIO_WritePin(Relay3_MainPower_GPIO_Port, Relay3_LineN_Pin, GPIO_PIN_RESET);
//	else
//		HAL_GPIO_WritePin(Relay3_LineN_GPIO_Port, Relay3_LineN_Pin, GPIO_PIN_SET);
//}
//
//void SetRelay4_LineL(int value)
//{
//	if(value == 1)
//		HAL_GPIO_WritePin(Relay4_LineL_GPIO_Port, Relay4_LineL_Pin, GPIO_PIN_RESET);
//	else
//		HAL_GPIO_WritePin(Relay4_LineL_GPIO_Port, Relay4_LineL_Pin, GPIO_PIN_SET);
//}


// =============================================================================================

int ReadStatusRelay3_MainPower()
{
	GPIO_PinState state = HAL_GPIO_ReadPin(Relay3_MainPower_GPIO_Port, Relay3_MainPower_Pin);
	if(state == GPIO_PIN_RESET)
	{
		return 0;
	}
	else if(state == GPIO_PIN_SET)
	{
		return 1;
	}
	else

		return 1;
}

int ReadStatusRelay4_InverterPower()
{
	GPIO_PinState state = HAL_GPIO_ReadPin(Relay4_InverterPower_GPIO_Port, Relay4_InverterPower_Pin);
	if(state == GPIO_PIN_RESET)
	{
		return 0;
	}
	else if(state == GPIO_PIN_SET)
	{
		return 1;
	}
	else

		return 1;
}

void SetRelaySSR_3_MainPower(int value)
{
	if(value == 1)
		HAL_GPIO_WritePin(Relay3_MainPower_GPIO_Port, Relay3_MainPower_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Relay3_MainPower_GPIO_Port, Relay3_MainPower_Pin, GPIO_PIN_RESET);

}

void SetRelaySSR_4_InverterPower(int value)
{
//	if(value == 1)
//		HAL_GPIO_WritePin(Relay4_InverterPower_GPIO_Port, Relay4_InverterPower_Pin, GPIO_PIN_RESET);
//	else
//		HAL_GPIO_WritePin(Relay4_InverterPower_GPIO_Port, Relay4_InverterPower_Pin, GPIO_PIN_SET);

	if(value == 1)
		HAL_GPIO_WritePin(Relay4_InverterPower_GPIO_Port, Relay4_InverterPower_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Relay4_InverterPower_GPIO_Port, Relay4_InverterPower_Pin, GPIO_PIN_RESET);
}

// w celu odczytu napięcia
//float ReadVoltageFromADC(float VoltageOfPowerSensor)// VoltageOfSensor can be 3.3 or 5
//{
//	uint32_t readValue=0;
//	float voltage =0;
//	HAL_ADC_Start(&hadc1);
//	HAL_ADC_PollForConversion(&hadc1,1000);
//	readValue = HAL_ADC_GetValue(&hadc1);
//	HAL_ADC_Stop(&hadc1);
//	//voltage =(float)readValue/4095*(37.5/7.5*VoltageOfSensor); 37,5 oraz 7.5 to rezystory na płytce PCB
//	voltage =(float)readValue/4095*5*VoltageOfPowerSensor;
//	//voltage =(float)readValue/4095*(37.5/7.5*VoltageOfSensor); 37,5 oraz 7.5 to rezystory na płytce PCB
////	    voltage =(float)readValue/4095*16.5; (dla 3.3 volta)
////	    voltage =(float)readValue/4095*25; (dla 5.0 volta)
//	if(voltage <= 0.9)
//	    	voltage = 0;
//	return voltage;
//
//}

//ralated to  LoRa ------------------------------------------------- START

//int InitializeMyLoRa(LoRa *myLoRadev)
//{
//
//	*myLoRadev = newLoRa();
//
//	myLoRadev->CS_port         = NSS_GPIO_Port;
//	myLoRadev->CS_pin          = NSS_Pin;
//	myLoRadev->reset_port      = RST_GPIO_Port;
//	myLoRadev->reset_pin       = RST_Pin;
//	myLoRadev->DIO0_port       = DIO0_EXTI_GPIO_Port;
//	myLoRadev->DIO0_pin        = DIO0_EXTI_Pin;
//	myLoRadev->hSPIx           = &hspi1;
//
//	myLoRadev->frequency       = 457;							  // default = 433 MHz
//	myLoRadev->spredingFactor  = SF_7;							// default = SF_7
//	myLoRadev->bandWidth		 = BW_125KHz;				  // default = BW_125KHz
//	myLoRadev->crcRate         = CR_4_5;						// default = CR_4_5
//	myLoRadev->power	         = POWER_20db;				// default = 20db
//	myLoRadev->overCurrentProtection = 100; 							// default = 100 mA
//	myLoRadev->preamble	     = 10;		  					// default = 8;
//
//	int ret = LoRa_init(myLoRadev);
//	if(ret== LORA_OK)
//	{
//		return LORA_OK;
//	}
//	else
//		return ret ;
//}

  //ralated to  LoRa ------------------------------------------------- END


//ralated to  delayUS ------------------------------------------------- START
void DelayInit(void)
{
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk;

  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

  DWT->CYCCNT = 0;

  /* 3 NO OPERATION instructions */
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");
  __ASM volatile ("NOP");
}

void DelayUS(uint32_t us) {
  uint32_t cycles = (SystemCoreClock/1000000L)*us;
  uint32_t start = DWT->CYCCNT;
  volatile uint32_t cnt;

  do
  {
    cnt = DWT->CYCCNT - start;
  } while(cnt < cycles);
}

//ralated to  delayUS ------------------------------------------------- END
