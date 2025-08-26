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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define ADC_BUF_SIZE 3
#define Channel_2_BUF_SIZE 10
#define Channel_3_BUF_SIZE 10
#define Channel_4_BUF_SIZE 10

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool DMA_Channel_2_Was_Filled = false;
bool DMA_Channel_3_Was_Filled = false;
bool DMA_Channel_4_Was_Filled = false;

int Buffer2_TableCount = 0;
int Buffer3_TableCount = 0;
int Buffer4_TableCount = 0;

volatile bool adc_ready = false;
uint32_t ADC_MoistureSensor_Dev_1[Channel_2_BUF_SIZE];
uint32_t ADC_MoistureSensor_Dev_2[Channel_3_BUF_SIZE];
uint32_t ADC_LightSensor_Dev[Channel_4_BUF_SIZE];
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
	typedef enum {
		DARK =20,
		NIGHT_OR_MORNING = 90,
		BRIGHT = 99
	} Light_State;

  uint32_t TimeNOW = 0;

  int GetMoistureAvg_TimeLenghtDefault = 10000; // 10 sek
  int GetMoistureAvg_TimeLenghtFast = 1000; // 1sek
  int GetMoistureAvg_TimeLenght = GetMoistureAvg_TimeLenghtDefault;
  uint32_t GetSoilMoisture_FromTime = 0;

  int MoistureHumidityPercent_Dev_1 = 0;
  int MoistureHumidityPercent_Dev_2 = 0;
  int MoistureGeneral = 0;

  bool PumpIsRunnign = false;
  int StartPumpWhenMoistureLower = 35;
  int StopPumpWhenMoistureHigher = 50;


  int GetLightMesurment_TimeLenghtDefault = 5000; // 5 sek
  uint32_t GetLightMesurment_FromTime = 0;

  int LightSensorPercent = 0;
  Light_State LightSensor = DARK;
  bool LightIsON = false;
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUF_SIZE);
  HAL_TIM_Base_Start_IT(&htim3);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		 if(adc_ready == true)
		 {
				if(Buffer2_TableCount < Channel_2_BUF_SIZE && DMA_Channel_2_Was_Filled == false)
				{
					ADC_MoistureSensor_Dev_1[Buffer2_TableCount]= adc_buffer[0];
					Buffer2_TableCount++;
				}
				else
				{
					DMA_Channel_2_Was_Filled = true;
					Buffer2_TableCount = 0;
				}

				if(Buffer3_TableCount < Channel_3_BUF_SIZE && DMA_Channel_3_Was_Filled == false)
				{
					ADC_MoistureSensor_Dev_2[Buffer3_TableCount]= adc_buffer[1];
					Buffer3_TableCount++;
				}
				else
				{
					DMA_Channel_3_Was_Filled = true;
					Buffer3_TableCount = 0;
				}

				if(Buffer4_TableCount < Channel_4_BUF_SIZE && DMA_Channel_4_Was_Filled == false)
				{
					ADC_LightSensor_Dev[Buffer4_TableCount]= adc_buffer[2];
					Buffer4_TableCount++;
				}
				else
				{
					DMA_Channel_4_Was_Filled = true;
					Buffer4_TableCount = 0;
				}

				 adc_ready = false;
				 HAL_ADC_Start_DMA(&hadc1, adc_buffer, ADC_BUF_SIZE);
		 }



		TimeNOW = HAL_GetTick();

		if(((uint32_t)TimeNOW - GetSoilMoisture_FromTime) >= GetMoistureAvg_TimeLenght)
		{
			GetSoilMoisture_FromTime = TimeNOW;

			 if(DMA_Channel_2_Was_Filled && DMA_Channel_3_Was_Filled)
			 {
				 DMA_Channel_2_Was_Filled = false;
				 DMA_Channel_3_Was_Filled = false;

	//			Moisture soil 1.2 -5 Volt
	//			 MoistureHumidityPercent_Dev_1 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_1, Channel_2_BUF_SIZE, 4095, 2300);
	//			 MoistureHumidityPercent_Dev_2 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_2, Channel_3_BUF_SIZE, 4095, 2300);

				 //MakerSoil MoistureSensor 5 Volt
//				 MoistureHumidityPercent_Dev_1 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_1, Channel_2_BUF_SIZE, 4095, 2910);
//				 MoistureHumidityPercent_Dev_2 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_2, Channel_3_BUF_SIZE, 4095, 2840);

				 //MakerSoil MoistureSensor 3.3 Volt

				 MoistureHumidityPercent_Dev_1 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_1, Channel_2_BUF_SIZE, 2600, 1730);
				 MoistureHumidityPercent_Dev_2 = ScaleADC_To_Percent_Inverted_Ranged(ADC_MoistureSensor_Dev_2, Channel_3_BUF_SIZE, 2600, 1730);


				 memset(txBuffer, 0, sizeof(txBuffer));
				 snprintf(txBuffer, sizeof(txBuffer), "Moisture sensor Dev 1: %d %%\r\n", MoistureHumidityPercent_Dev_1);
				 Send(txBuffer);

				 memset(txBuffer, 0, sizeof(txBuffer));
				 snprintf(txBuffer, sizeof(txBuffer), "Moisture sensor Dev 2: %d %%\r\n", MoistureHumidityPercent_Dev_2);
				 Send(txBuffer);

				 MoistureGeneral = (MoistureHumidityPercent_Dev_1 +MoistureHumidityPercent_Dev_2)/2;
				 memset(txBuffer, 0, sizeof(txBuffer));
				 snprintf(txBuffer, sizeof(txBuffer), "Soil moisture avg: %d %%\r\n", MoistureGeneral);
				 Send(txBuffer);

				 if(MoistureGeneral <= StartPumpWhenMoistureLower && PumpIsRunnign == false)
				 {
					 memset(txBuffer, 0, sizeof(txBuffer));
					 snprintf(txBuffer, sizeof(txBuffer), "Soil moisture avg: %d is <= %d \r\n", MoistureGeneral, StartPumpWhenMoistureLower);
					 Send(txBuffer);

					 GetMoistureAvg_TimeLenght = GetMoistureAvg_TimeLenghtFast;
					 HAL_GPIO_WritePin(PIN_PB10_Pump_GPIO_Port, PIN_PB10_Pump_Pin, GPIO_PIN_SET);
					 PumpIsRunnign = true;
				 }

				 if(MoistureGeneral >= StopPumpWhenMoistureHigher && PumpIsRunnign == true)
				 {
					 memset(txBuffer, 0, sizeof(txBuffer));
					 snprintf(txBuffer, sizeof(txBuffer), "Soil moisture avg: %d is >= %d \r\n", MoistureGeneral, StopPumpWhenMoistureHigher);
					 Send(txBuffer);
					 HAL_GPIO_WritePin(PIN_PB10_Pump_GPIO_Port, PIN_PB10_Pump_Pin, GPIO_PIN_RESET);
					 GetMoistureAvg_TimeLenght = GetMoistureAvg_TimeLenghtDefault;
					 PumpIsRunnign = false;
				 }

			 }


		}

		TimeNOW = HAL_GetTick();

		if(((uint32_t)TimeNOW - GetLightMesurment_FromTime) >= GetLightMesurment_TimeLenghtDefault)
		{
			GetLightMesurment_FromTime = TimeNOW;

			 if(DMA_Channel_4_Was_Filled)
			 {
				 DMA_Channel_4_Was_Filled = false;

				 //jasno → niska rezystancja LDR → wysokie napięcie → wysokie ADC (np. 3500)
				 //ciemno → wysoka rezystancja LDR → niskie napięcie → niskie ADC (np. 100)
				 LightSensorPercent = ScaleADC_Light_To_Percentage(ADC_LightSensor_Dev, Channel_4_BUF_SIZE, 100, 3700);

				 memset(txBuffer, 0, sizeof(txBuffer));
				 snprintf(txBuffer, sizeof(txBuffer), "Light sensor: %d %% light\r\n", LightSensorPercent);
				 Send(txBuffer);

				 if(LightSensorPercent<= DARK)
					 LightSensor = DARK;
				 else if(LightSensorPercent> DARK && LightSensorPercent< BRIGHT)
					 LightSensor = NIGHT_OR_MORNING;
				 else if(LightSensorPercent > BRIGHT)
					 LightSensor = BRIGHT;


				 memset(txBuffer, 0, sizeof(txBuffer));
				 switch (LightSensor)
				 {
					 case DARK:
						 if(LightIsON == true)
						 {
							 HAL_GPIO_WritePin(PIN_PB11_Light_GPIO_Port, PIN_PB11_Light_Pin,GPIO_PIN_RESET);
							 snprintf(txBuffer, sizeof(txBuffer), "Light sensor DARK disable LED\r\n");
							 Send(txBuffer);
							 LightIsON = false;
						 }
						break;

					 case NIGHT_OR_MORNING:
						 if(LightIsON == false)
						 {
							 HAL_GPIO_WritePin(PIN_PB11_Light_GPIO_Port, PIN_PB11_Light_Pin, GPIO_PIN_SET);
							 snprintf(txBuffer, sizeof(txBuffer), "Light sensor NIGHT_OR_MORNING enable LED\r\n");
							 Send(txBuffer);
							 LightIsON = true;
						 }

						break;

					 case BRIGHT:
						 if(LightIsON == true)
						 {
							 HAL_GPIO_WritePin(PIN_PB11_Light_GPIO_Port, PIN_PB11_Light_Pin,GPIO_PIN_RESET);
							 snprintf(txBuffer, sizeof(txBuffer), "Light sensor DARK disable LED\r\n");
							 Send(txBuffer);
							 LightIsON = false;
						 }
						break;

					 default:
						break;
				 }

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
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 144;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	adc_ready = true;
	HAL_ADC_Stop_DMA(&hadc1); // jesli chcial bym wylaczyc timer
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);  1 jako ON 0 jako OFF
		HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {

    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) // Sprawdź, czy to Twój UART
    {
        isTransmissionComplete = true;
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
