/*
 * waweroBasicFunction.c
 *
 *  Created on: Feb 4, 2024
 *      Author: wawer
 */
#include <stdio.h>
#include "main.h"
#include "waweroBasicFunction.h"
#include "File_Handling.h"
#include "spi.h"

void ToggleLedOn()
{
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);  1 jako ON 0 jako OFF

	  //HAL_GPIO_TogglePin(LED_Pill_GPIO_Port, LED_Pill_Pin);
}

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

//ralated to  LoRa ------------------------------------------------- START

int InitializeMyLoRa(LoRa *myLoRadev)
{

	*myLoRadev = newLoRa();

	myLoRadev->CS_port         = NSS_GPIO_Port;
	myLoRadev->CS_pin          = NSS_Pin;
	myLoRadev->reset_port      = RST_GPIO_Port;
	myLoRadev->reset_pin       = RST_Pin;
	myLoRadev->DIO0_port       = DIO0_GPIO_Port;
	myLoRadev->DIO0_pin        = DIO0_Pin;
	myLoRadev->hSPIx           = &hspi1;

	myLoRadev->frequency       = 457;							  // default = 433 MHz
	myLoRadev->spredingFactor  = SF_7;							// default = SF_7 // rekomendowany SF_9
	myLoRadev->bandWidth		 = BW_125KHz;				  // default = BW_125KHz
	myLoRadev->crcRate         = CR_4_5;						// default = CR_4_5
	myLoRadev->power	         = POWER_17db;				// default = 20db
	myLoRadev->overCurrentProtection = 100; 							// default = 100 mA
	myLoRadev->preamble	     = 8;		  					// default = 8;

	LoRa_reset(myLoRadev);
	int ret = LoRa_init(myLoRadev);
	if(ret== LORA_OK)
	{
		LoRa_isWorking = true;
		return LORA_OK;
	}
	else
		return ret ;
}

  //ralated to  LoRa ------------------------------------------------- END

void PrintImage (int index)
{

	//FRESULT result = FR_INT_ERR;
	char *imageFilePath = malloc(20*sizeof(char));
	sprintf (imageFilePath, "Image%d.bin", index);
	if(Mount_SD("/") == FR_OK)
	{
		//lcdSetOrientation(LCD_ORIENTATION_PORTRAIT);
	  	if (Read_And_Display_JPEG(imageFilePath, 0, 0, 240, 320, 16) != FR_OK)
	  		Send_ToLCDDisplay("JPEG file read failed");
		Unmount_SD("/");
	}
	free(imageFilePath);
}
