/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// pliki naglowkowe musza tu byc z racji na to ze ich deklaracja jest zalezna od includow z main
#include "File_Handling.h"
#include "INI_FileHandling.h"
#include "actions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint8_t ReInitializingCardRequired = 0;

volatile int DisableTimeScreenAfterMS = 20000;
volatile int DisableTimeScreen = 0;
volatile int DisableTimeScreenNOW = 0;

volatile int SendTimeUARTCount = 0;
volatile int SendTimeUARTAfterSec = 5;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	int PointerSensivitySizeDefault = 200;

	int LV_CheckTimeAfterMS = 5;

	int TimeNOW = 0;
	int SendFromTime = 0;
	int LV_FromTime = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  DelayInit();

  //lcdInit();
  initTouchScr();

  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  LCD_BL_ON();

  if(HAL_GPIO_ReadPin(GPIOC, CardDetectPin_Pin) == GPIO_PIN_RESET)
  {
	CardDetectedInSlot = true;
	InitializeSDCard();
  }

  IniFile config;

  lcdFillRGB(COLOR_BLACK);

  lcdSetOrientation(LCD_ORIENTATION_PORTRAIT);
	if(CardDetectedInSlot)
	{
	 if(Mount_SD("/") == FR_OK)
	  {
		if (Load_INI_File("config.ini", &config) == FR_OK)
		{
			Debug("Plik INI wczytany pomyslnie.");
			HAL_Delay(2000);

			if (Get_INI_Value_As_Int(&config, "DeviceConfig", "ScreenTime", (void*)&DisableTimeScreenAfterMS))
			{
	//			snprintf(debugData, sizeof(debugData),"ScreenTime: %d ms", DisableTimeScreenAfterMS);
	//			Debug(debugData);
			} else
			{
				Debug("Nieprawidlowa wartosc ScreenTime.");
				HAL_Delay(2000);
			}

			if (Get_INI_Value_As_Int(&config, "DeviceConfig", "PointerSensivitySizeDefault", &PointerSensivitySizeDefault))
			{
	//			snprintf(debugData, sizeof(debugData),"PointerSensivitySizeDefault: %d ms", PointerSensivitySizeDefault);
	//			Debug(debugData);
			} else
			{
				Debug("Nieprawidlowa wartosc PointerSensivitySizeDefault.");
				HAL_Delay(2000);
			}

		} else
		{
			Debug("Nie udalo sie wczytac pliku INI.");
			HAL_Delay(3000);
		}

	//	 char *fileData;
	//	  result = Read_File("Test.txt", &fileData);
	//	  if (result == FR_OK)
	//	  {
	//		  lcdDisplayTextWithScrollPage(fileData, 2000, 0,0, COLOR_WHITE, COLOR_BLACK);
	//		  free(fileData);
	//		  fileData = NULL;
	//	  }
	//	  else if (result != FR_OK)
	//	  {
	//		  Send_ToLCDDisplay("ERROR: Failed to read txt file.");
	//		  free(fileData);
	//		  fileData = NULL;
	//	  }
	  }
	  Unmount_SD("/");
	}

  DisableTimeScreen = HAL_GetTick();
  TSInterrupt = 0;

  lcdSetOrientation(LCD_ORIENTATION_LANDSCAPE);
  ui_init();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  DisableTimeScreenNOW = TimeNOW = HAL_GetTick();

	  if(((uint32_t)DisableTimeScreenNOW - DisableTimeScreen) >= DisableTimeScreenAfterMS)
	  {
		  DisableTimeScreen = DisableTimeScreenNOW;
		  LCD_BL_OFF();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //DelayUS(1000);
	if ((uint32_t)(TimeNOW - LV_FromTime) >= LV_CheckTimeAfterMS)
	{
		LV_FromTime = TimeNOW; // Ustaw następny czas startu na bieżący
		lv_timer_handler();
		ui_tick();
	}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 14;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case TOUCH_INTERRUPT_Pin: //EXTI9_10_IRQHandler(void) {
//			if (TSInterrupt == 0)
//			{
				//HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); // Zablokuj przerwanie EXTI na czas obsługi
				LCD_BL_ON();
				DisableTimeScreen = HAL_GetTick();
				TSInterrupt = 1;
//			}
				//__HAL_GPIO_EXTI_CLEAR_IT(TOUCH_INTERRUPT_Pin);
			break;

		case CardDetectPin_Pin: //EXTI15_10_IRQHandler(void)

			if (HAL_GPIO_ReadPin(GPIOC, CardDetectPin_Pin) == GPIO_PIN_RESET)
			{

				CardDetectedInSlot = true;
				LCD_BL_ON();

				char *buf;
				size_t bufSize;
				// Karta SD została włożona
				char tekst[] ="Karta SD wlozona";
				// Oblicz wymagany rozmiar bufora
	        	bufSize = snprintf(NULL, 0, "%s", tekst) + 1;
	        	buf = malloc(bufSize);
        	    snprintf(buf, bufSize, "%s", tekst);

				ReInitializingCardRequired = 1;

				int cx = lcdGetWidth()  / 2;
				int cy = lcdGetHeight() / 2;
				int tekstLength =strlen(buf);
				lcdSetCursor(cx- ((lcdGetTextFont()->Width*tekstLength )/2 -1), cy);
				lcdSetTextColor(COLOR_WHITE, COLOR_BLACK);
				lcdPrintf(buf);
	    	    free(buf);

				DisableTimeScreen = HAL_GetTick();
			}

			else if (HAL_GPIO_ReadPin(GPIOC, CardDetectPin_Pin) == GPIO_PIN_SET)
			{
				CardDetectedInSlot = false;
				LCD_BL_ON();

				char *buf;
				size_t bufSize;
				// Karta SD została włożona
				char tekst[] ="Karta SD wyjeta";
				// Oblicz wymagany rozmiar bufora
	        	bufSize = snprintf(NULL, 0, "%s", tekst) + 1;
	        	buf = malloc(bufSize);
        	    snprintf(buf, bufSize, "%s", tekst);

				ReInitializingCardRequired = 0;

				int cx = lcdGetWidth()  / 2;
				int cy = lcdGetHeight() / 2;
				int tekstLength =strlen(buf);
				lcdSetCursor(cx- ((lcdGetTextFont()->Width*tekstLength )/2 -1), cy);
				lcdSetTextColor(COLOR_WHITE, COLOR_BLACK);
				lcdPrintf(buf);
	    	    free(buf);

				DisableTimeScreen = HAL_GetTick();
			}

			//__HAL_GPIO_EXTI_CLEAR_FLAG(CardDetectPin_Pin);
    	    break;

	}


//    if (GPIO_Pin == TOUCH_INTERRUPT_Pin)
//    {
//		if (TSInterrupt == 0)
//		{
//			//HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); // Zablokuj przerwanie EXTI na czas obsługi
//			TSInterrupt = 1;
//			LCD_BL_ON();
//			DisableTimeScreen = HAL_GetTick();
//		}
//    }

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
