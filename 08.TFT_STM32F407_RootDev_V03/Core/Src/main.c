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
#include "i2c.h"
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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t ReInitializingCardRequired = 0;

volatile int DisableTimeScreenAfterMS = 20000;
volatile int DisableTimeScreen = 0;

// ESP wifi Start
volatile int SendTimeUARTCount = 0;
volatile int SendTimeUARTAfterSec = 5;
// ESP

// LoRa Start
LoRa myLoRa;
int LoRa_stat=0;
char TxBuffer[128];
volatile int LoRaIsReceived=0;
char RxBuffer[128];
// LoRa End

// BMP280 Start
BMP280_HandleTypedef bmp280;
uint16_t size;
uint8_t Data[256];

// BMP280 END

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
	int BMP280_CheckTimeAfterMS = 5000;
	int DeviceStatusesRefresh_CheckTimeAfterMS = 1000;

	int DefaultDTSynchAfterMS= 3600000 * 8; // powinna byc 1h
	int SendDataTime_CheckTimeAfterMS = 30000;
	int SendDataTimeSynchronizeAfterMS = DefaultDTSynchAfterMS;

	int TimeNOW = 0;
	int LV_FromTime = 0;
	int BMP280_FromTime = 0;
	int DeviceStatusesRefresh_FromTime = 0;
	int DataTime_FromTime = 0;
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
  MX_SPI1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  DelayInit();

  initTouchScr(); //lcdInit();

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

  lcdSetOrientation(LCD_ORIENTATION_LANDSCAPE);

  //TODOLIST
  // kalibracja temp hum itd (najpierw odczytywana z karty SD
  // wyswietlana na jakies zakladce lub osobnej stronie.
  // wifi jak czesto odświezać ma serwer NTP
  // timer wylaczania ekranu ustawienia z karty SD
  // dodatkowo wycięcie logowania przy WIFI

	if(CardDetectedInSlot)
	{
	 if(Mount_SD("/") == FR_OK)
	  {
		if (Load_INI_File("config.ini", &config) == FR_OK)
		{
			Debug("Plik INI wczytany pomyslnie.");
			HAL_Delay(1000);

			if (Get_INI_Value_As_Int(&config, "DeviceConfig", "ScreenTime", (void*)&DisableTimeScreenAfterMS))
			{
	//			snprintf(debugData, sizeof(debugData),"ScreenTime: %d ms", DisableTimeScreenAfterMS);
	//			Debug(debugData);
			}
			else
			{
				Debug("Nieprawidlowa wartosc ScreenTime.");
				HAL_Delay(2000);
			}

			if (Get_INI_Value_As_Int(&config, "DeviceConfig", "PointerSensivitySizeDefault", &PointerSensivitySizeDefault))
			{
	//			snprintf(debugData, sizeof(debugData),"PointerSensivitySizeDefault: %d ms", PointerSensivitySizeDefault);
	//			Debug(debugData);
			}
			else
			{
				Debug("Nieprawidlowa wartosc PointerSensivitySizeDefault.");
				HAL_Delay(1000);
			}

			memset(WIFI_SSID, 0, sizeof(WIFI_SSID));
			// Pobierz dane Wi-Fi
			if (Get_INI_Value(&config, "WiFi", "SSID", WIFI_SSID, sizeof(WIFI_SSID)))
			{
				Debug("                                         ");
				snprintf(debugData, sizeof(debugData),"SSID: %s", WIFI_SSID);
				Debug(debugData);
				HAL_Delay(1000);
			}

			memset(WIFI_PASSWORD, 0, sizeof(WIFI_PASSWORD));
			if (Get_INI_Value(&config, "WiFi", "Password", WIFI_PASSWORD, sizeof(WIFI_PASSWORD)))
			{
//				Debug("                                         ");
//				snprintf(debugData, sizeof(debugData),"Password: %s", WIFI_PASSWORD);
//				Debug(debugData);
//				HAL_Delay(2000);
			}

		} else
		{
			Debug("Nie udalo sie wczytac pliku INI.");
			HAL_Delay(3000);
		}
	  }
	  Unmount_SD("/");
	}

	ui_init();
	// --------------------------------------------------------------------ESP 8266
	InitESP8266();
	ConnectToWiFi(WIFI_SSID, WIFI_PASSWORD);

	// --------------------------------------------------------------------LORA
	LoRa_stat = InitializeMyLoRa(&myLoRa);
	MakeHandleToLoRa(&myLoRa);
	SetDevID(4705);
	SendMessageRequest(0, Broadcast);

	// --------------------------------------------------------------------BMP280
	bmp280_init_default_params(&bmp280.params);
	bmp280.addr = BMP280_I2C_ADDRESS_0; // GND
	bmp280.i2c = &hi2c1;
	while (!bmp280_init(&bmp280, &bmp280.params))
	{
	//		size = sprintf((char *)Data, "BMP280 initialization failed\n");
		Debug("BMP280 initialization failed...");
		HAL_Delay(2000);
	}
//    bool bme280p = bmp280.id == BME280_CHIP_ID;
//	size = sprintf((char *)Data, "BMP280: found %s\n", bme280p ? "BME280" : "BMP280");
	bmp280_read_float(&bmp280,&BMP280_temperature, &BMP280_pressure, &BMP280_humidity);
	SetLocalTempHumPressureLVGL();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	ConnectToWiFi(WIFI_SSID, WIFI_PASSWORD);
	LoRaReceiveMode();
	DisableTimeScreen = HAL_GetTick();
	TSInterrupt = 1;


	// init time
	DateNow.isWinterTime = false;

	while (1)
	{

		TimeNOW = HAL_GetTick();

		if(((uint32_t)TimeNOW - DisableTimeScreen) >= DisableTimeScreenAfterMS)
		{
			DisableTimeScreen = TimeNOW;
			LCD_BL_OFF();
			TSInterrupt = 0;
		}

		if ((uint32_t)(TimeNOW - BMP280_FromTime) >= BMP280_CheckTimeAfterMS)
		{
			BMP280_FromTime = TimeNOW; // Ustaw następny czas startu na bieżący

			bmp280_read_float(&bmp280,&BMP280_temperature, &BMP280_pressure, &BMP280_humidity); // or altitude
			SetLocalTempHumPressureLVGL();
		}

		if ((uint32_t)(TimeNOW - DeviceStatusesRefresh_FromTime) >= DeviceStatusesRefresh_CheckTimeAfterMS)
		{
			DeviceStatusesRefresh_FromTime = TimeNOW; // Ustaw następny czas startu na bieżący
			SetLoRaStatusFlagsLVGL();
			SetWifiStatusFlagsLVGL();
		}

		TimeNOW = HAL_GetTick();


		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if ((uint32_t)(TimeNOW - LV_FromTime) >= LV_CheckTimeAfterMS)
		{
			LV_FromTime = TimeNOW; // Ustaw następny czas startu na bieżący
			lv_timer_handler();
			ui_tick();
		}

		if(LoRaIsReceived == 1)// received somethig
		{
			if(UnpackMessage(RxBuffer) != 1)
			{
			}
			LoRaIsReceived = 0;
			memset(RxBuffer, 0, sizeof RxBuffer);
		}

		if(ReInitializingCardRequired == 1 && CardDetectedInSlot == true)
		{
			ReInitializingCardRequired = 0;
			InitializeSDCard();
		}

		if ((uint32_t)(TimeNOW - DataTime_FromTime) >= SendDataTime_CheckTimeAfterMS)
		{
			DataTime_FromTime = TimeNOW; // Ustaw następny czas startu na bieżący
			if(GetTimeFromNTP())
			{
				if(procesNTPTime == true)
				{
					SendDataTime_CheckTimeAfterMS = SendDataTimeSynchronizeAfterMS;

					if(TimerInit == false)
					{
						if (HAL_TIM_Base_Start_IT(TIMERSEC) == HAL_OK)
						{
						  TimerInit = true;
						}
						else
						{
						  TimerInit = false;
						}
					}
				}
			}
			else
				SendDataTime_CheckTimeAfterMS = 60000;
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
		case DIO0_Pin:

		LoRa_receive(&myLoRa, (uint8_t *) RxBuffer, 128);
		LoRaIsReceived=1;
		break;

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

}

// Callback obsługi przerwania UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // Zapisz odebrany bajt do bufora cyklicznego
    	if(rxWriteIndex == 0 && (rxData == '\n' || rxData == '\r')) // hmm filtrowanie smieci ?
    	{
    		rxData = 0;
    	}
    	else
    	{
    		BufferTable[ActualBuffer].rxBuffer[rxWriteIndex]=rxData;

    		rxWriteIndex++;
    	}


    	if(WaitForNTPResp == true && rxData == '+') // zacznij zczytywać wiadomość NTP
    	{
    		StartCollectingNTPRespons = true;
    	}


		if (rxWriteIndex >= NTPResponsLength && StartCollectingNTPRespons == true) // dlugosc wiadomosci 56
        {
//			char dbData[300];
//			sprintf(dbData, "Bufor %d do przetworzenia: %s ;index %d \r\n",ActualBuffer, BufferTable[ActualBuffer].rxBuffer, rxWriteIndex);
//			Debug(dbData);

			WaitForNTPResp = false;
			StartCollectingNTPRespons = false;

            if (rxWriteIndex < RX_BUFFER_SIZE)
            {
            	BufferTable[ActualBuffer].rxBuffer[rxWriteIndex] = '\0';
            	BufferTable[ActualBuffer].rxWriteLength = rxWriteIndex;
            }
            else
            	BufferTable[ActualBuffer].rxWriteLength = rxWriteIndex -1;

            BufferTable[ActualBuffer].rxWasReaded = false;

            for(int i = 0; i < BufferTableSize; i++)
            {
            	if(BufferTable[i].rxWasReaded == true)
            	{
            		ActualBuffer = i;
            		BufferTable[i].rxWasReaded = false;
            		break;
            	}
            }

            rxWriteIndex = 0;
        }
		else if ((rxWriteIndex >= RX_BUFFER_SIZE - 1 || rxData == '\n') && StartCollectingNTPRespons == false)// && commandReady == 0)
        {
            if (rxWriteIndex < RX_BUFFER_SIZE)
            {
            	BufferTable[ActualBuffer].rxBuffer[rxWriteIndex] = '\0';
            	BufferTable[ActualBuffer].rxWriteLength = rxWriteIndex;
            }
            else
            	BufferTable[ActualBuffer].rxWriteLength = rxWriteIndex -1;

            BufferTable[ActualBuffer].rxWasReaded = false;

//            char dbData[500];
//            snprintf(dbData, sizeof(dbData), "Bufor %d do przetworzenia: %s\r\n",ActualBuffer, BufferTable[ActualBuffer].rxBuffer);
//			Debug(dbData);

            for(int i = 0; i < BufferTableSize; i++)
            {
            	if(BufferTable[i].rxWasReaded == true)
            	{
            		ActualBuffer = i;
            		BufferTable[i].rxWasReaded = false;
            		break;
            	}
            }
            rxWriteIndex = 0;
        }

//		Wznów odbiór danych w trybie przerwań // przeniesione do ProcessReceivedData();
		HAL_UART_Receive_IT(ESP_USART, (uint8_t *)&rxData, 1);

		//HAL_UART_AbortReceive_IT(ESP_USART);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) // Sprawdź, czy to Twój UART
    {
        isTransmissionComplete = true;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		SendTimeUARTCount++;
		IncrementLocalTime((void*)&DateNow);
//		char debugData[60];
//		sprintf(debugData,"%04d/%02d/%02d %02d:%02d:%02d", DateNow.year, DateNow.month, DateNow.day, DateNow.hour, DateNow.minute, DateNow.second);
//		PrintDateTime(debugData);
		SetDateTimeLVGL();
		if(SendTimeUARTCount >= SendTimeUARTAfterSec)
		{
			SendTimeUARTCount= 0;
//		    char debugData[200];
//		    sprintf(debugData,"LOCAL Date and Time: %04d-%02d-%02d %02d:%02d:%02d\r\n", DateNow.year, DateNow.month, DateNow.day, DateNow.hour, DateNow.minute, DateNow.second);
//			Debug(debugData);
		}

	}
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
