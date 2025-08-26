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
