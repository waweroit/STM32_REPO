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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "ZMPT101BHeader.h"
#include "BATTERYInfo.h"
#include "waweroBasicFunction.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_SIZE 2
#define Channel0_BUF_SIZE 100
#define Channel1_BUF_SIZE 400

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile bool calculateBattVoltage = false;  // Global flag // co 1 sek ok.
volatile bool adc_ready = false;
volatile bool DMA_AC_AdjustSensivity = false;

int relayUpsStatus =0;
int relayBatteryCharging =0;
//int relayLineNStatus =0;
//int relayLineLStatus =0;


volatile bool isMainACSelected = true;
volatile bool acAvailable = false;


int relayLineMainPowerStatus =0;
int relayLineInverterPowerStatus =0;

int Buffer_DC_TableCount = 0;
int Buffer_AC_TableCount = 0;

bool DMA_DC_Was_Filled = false;
bool DMA_AC_Was_Filled = false;

float VoltageDCBattery;

eBatteryStatus BatteryStatus = NO_BATTERY;
eChargingStatus ChargingStatus = UNKNOWN;

bool BatteryCharging = false;

uint32_t BatteryChargingStartTime = 0;
int BatteryChargingTime = 0;
int BatteryChargingTimeLONG =43200; // 12h
int BatteryChargingTimeSHORT =21600; // 6h
uint32_t inrementOneSecTim=0;
int TimeNOW=0;

//bool WorkingOnBattery = false;
//bool WasDischarded = false;

float VoltageAC =230;
float VoltageACTemp =220;
int VoltageDrop = 190;


float batteryDischarged = 11.0;
int MSDelayBetweenACPower = 1500;

uint32_t ADC_DC_BATTERY_TABLE[Channel0_BUF_SIZE];
uint32_t ADC_AC_POWER_TABLE[Channel1_BUF_SIZE];
uint32_t adc_buffer[ADC_BUF_SIZE];

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUF_SIZE);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim10);

  DelayInit();

  relayUpsStatus = ReadStatusRelay1_UPS();
  relayBatteryCharging = ReadStatusRelay2_BATTERY_CHARGE();
//  relayLineNStatus = ReadStatusRelay3_LineN();
//  relayLineLStatus = ReadStatusRelay4_LineL();
  relayLineMainPowerStatus = ReadStatusRelay3_MainPower();
  relayLineInverterPowerStatus = ReadStatusRelay4_InverterPower();

  Initialize_ZMPT101B(&hadc1);


  //BatteryCharging = true;
//  SetRelay1_UPS(1);
//  WorkingOnBattery = true;
  SetRelaySSR_4_InverterPower(0);
  DelayUS(2500);
  SetRelaySSR_3_MainPower(1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  bool acAvailableLastStatus = true;
  bool isFirstRun = true;

  DelayUS(2000000);
  while (1)
  {
	 if(adc_ready == true)
	 {

		 // odczyt z ADC jakim napieciem dysponuje nasz Akumulator
		if(Buffer_DC_TableCount < Channel0_BUF_SIZE && DMA_DC_Was_Filled == false)
		{
			ADC_DC_BATTERY_TABLE[Buffer_DC_TableCount] = adc_buffer[0];
			Buffer_DC_TableCount++;
		}
		else
		{
			DMA_DC_Was_Filled = true;
			Buffer_DC_TableCount = 0;
		}

		// odczyt z ADC jakie jest napiecie w sieci
		if(Buffer_AC_TableCount < Channel1_BUF_SIZE && DMA_AC_Was_Filled == false)
		{
			ADC_AC_POWER_TABLE[Buffer_AC_TableCount] = adc_buffer[1];
			Buffer_AC_TableCount++;

		}
		else
		{
			DMA_AC_Was_Filled = true;
			Buffer_AC_TableCount = 0;
			//calibrateLowNoisePointFromDMA(ADC_AC_POWER_TABLE, Channel1_BUF_SIZE); jak jest uruchomione bardzo skacze wynik
		}

		adc_ready = false;
		HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUF_SIZE);
	 }




     if (DMA_AC_Was_Filled)
     {
         DMA_AC_Was_Filled = false;

         // Odczyt statusów przekaźników
         relayUpsStatus               = ReadStatusRelay1_UPS();
         relayLineMainPowerStatus     = ReadStatusRelay3_MainPower();
         relayLineInverterPowerStatus = ReadStatusRelay4_InverterPower();

         // Pomiar napięcia sieci
         calibrateZeroPointWithTolerance(ADC_AC_POWER_TABLE, Channel1_BUF_SIZE, 50);
         VoltageAC = getVoltageACFromDMABuff(ADC_AC_POWER_TABLE, Channel1_BUF_SIZE, DMA_AC_AdjustSensivity);
         acAvailable = (VoltageAC > VoltageDrop);

		if(acAvailable != acAvailableLastStatus)
		{
			if (acAvailable && !isMainACSelected)
			{
				SetRelaySSR_4_InverterPower(0);
				DelayUS(MSDelayBetweenACPower);
				SetRelaySSR_3_MainPower(1);
				ResetADC();
				isMainACSelected = true;

				if(BatteryStatus < OK_BATTERY_HIGH && relayUpsStatus == 1)
					SetRelay1_UPS(0);

				acAvailableLastStatus = acAvailable;// jeżeli warunki przełączenia nie są spełnione, lastStatus NIE JEST aktualizowany
			}
			 //else if (!acAvailable && isMainACSelected && WasChargedEnough)
			else if (!acAvailable && isMainACSelected)
			 {
				if(BatteryStatus > DISCHARGED_BATTERY && relayUpsStatus == 0)
					SetRelay1_UPS(1);

				SetRelaySSR_3_MainPower(0);
				DelayUS(MSDelayBetweenACPower);
				SetRelaySSR_4_InverterPower(1);

				// ustabilizowanie sie napiecia baterii po zalaczeniu USP
				if(relayUpsStatus == 0)
					DelayUS(5000000);

				ResetADC();
				isMainACSelected = false;


				acAvailableLastStatus = acAvailable; // jeżeli warunki przełączenia nie są spełnione, lastStatus NIE JEST aktualizowany

			 }



		}
     }

	 if (DMA_DC_Was_Filled && calculateBattVoltage)
	 {
	     DMA_DC_Was_Filled    = false;
	     calculateBattVoltage = false;

	     VoltageDCBattery = ReadDCBattVoltageFromADC(ADC_DC_BATTERY_TABLE, Channel0_BUF_SIZE);
	     BatteryStatus    = BatteryInformation(batteryDischarged, VoltageDCBattery);

		  
		switch (ChargingStatus)
	    {
			case UNKNOWN:
				ChargingStatus = CHARGING_REQUIRED;
			break;

			case CHARGING_REQUIRED:
				if (acAvailable)
				{
					if(BatteryStatus <= OK_BATTERY_MID)
					{
						ChargingStatus = CHARGING_BATTERY;
					}
					else if(isFirstRun)
					{
						isFirstRun = false;
						ChargingStatus = CHARGING_BATTERY;
					}
				}
				else
				{
					ChargingStatus = NOT_CHARGING;
				}
			break;			
			
			case CHARGING_BATTERY:
				if (acAvailable)
				{
					SetRelay2_BATTERY_CHARGE(1);
					if(BatteryStatus == FULL_BATTERY)
					{
						ChargingStatus = CHARGING_END;
					}
				}
				else
				{
					ChargingStatus = NOT_CHARGING;
				}
			break;
			
			case CHARGING_END:
				if (acAvailable)
				{
					ChargingStatus = NOT_CHARGING;
				}
				else
				{
					ChargingStatus = CHARGING_REQUIRED;
				}
				
			break;
			
			case NOT_CHARGING:
				SetRelay2_BATTERY_CHARGE(0);

				if(BatteryStatus <= OK_BATTERY_HIGH)
				{
					ChargingStatus = CHARGING_REQUIRED;
				}
			break;
	    }

		switch (BatteryStatus)
		{
			case NO_BATTERY:
			case DISCHARGED_BATTERY:
				SetRelay1_UPS(0);
				break;

			case OK_BATTERY_LOW:
					if(acAvailableLastStatus == true && isMainACSelected == false)
					SetRelay1_UPS(0);
				break;

			case OK_BATTERY_MID:
				break;

			case OK_BATTERY_HIGH:
			case FULL_BATTERY:
				// bateria wystarczająco naładowana → inwerter ON
				SetRelay1_UPS(1);
				break;
		}
	 }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void ResetADC()
{
	HAL_ADC_Stop_DMA(&hadc1);
	memset( ADC_AC_POWER_TABLE, 0, Channel1_BUF_SIZE * sizeof(uint32_t) );
	memset( adc_buffer, 0, ADC_BUF_SIZE * sizeof(uint32_t) );

	adc_ready = false;
	DMA_AC_Was_Filled = false;
	DMA_DC_Was_Filled = false;
	Buffer_AC_TableCount = 0;
	//DelayUS(1000);//100milisec
	HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUF_SIZE);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//UNUSED(hadc); // nie wiem czemu tego nie wyremować ??
	HAL_ADC_Stop_DMA(&hadc1); // jesli chcial bym wylaczyc timer
	adc_ready = true;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM10) // jest odpalany co 1 sek.
	{
		ToggleLedOnBlackPill();
		calculateBattVoltage = true;  // Global flag
		inrementOneSecTim++;
//
//		if(DMA_AC_AdjustSensivity == false)
//			DMA_AC_AdjustSensivity = true;
//		else
//			DMA_AC_AdjustSensivity = false;
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
#ifdef USE_FULL_ASSERT
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
