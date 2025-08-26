/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "waweroBasicFunction.h"
#include "ili9341.h"
#include "SPITouchScreen.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "string.h"
#include "lvgl.h"
#include "lvgl_Controller.h"
#include "ui.h"
#include "LoRa.h"
#include "CommunicationMessages.h"
#include "DataTime.h"
#include "ESP8266_WiFi.h"
#include "BMP280.h"
#include "actions.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DIO0_Pin GPIO_PIN_2
#define DIO0_GPIO_Port GPIOA
#define DIO0_EXTI_IRQn EXTI2_IRQn
#define RST_Pin GPIO_PIN_3
#define RST_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_4
#define NSS_GPIO_Port GPIOA
#define SCK_LoRa_Pin GPIO_PIN_5
#define SCK_LoRa_GPIO_Port GPIOA
#define TOUCH_INTERRUPT_Pin GPIO_PIN_5
#define TOUCH_INTERRUPT_GPIO_Port GPIOC
#define TOUCH_INTERRUPT_EXTI_IRQn EXTI9_5_IRQn
#define LCD_BL_Pin GPIO_PIN_1
#define LCD_BL_GPIO_Port GPIOB
#define TOUCH_CS_Pin GPIO_PIN_12
#define TOUCH_CS_GPIO_Port GPIOB
#define SCK_TouchScreen_Pin GPIO_PIN_13
#define SCK_TouchScreen_GPIO_Port GPIOB
#define CardDetectPin_Pin GPIO_PIN_7
#define CardDetectPin_GPIO_Port GPIOC
#define CardDetectPin_EXTI_IRQn EXTI9_5_IRQn
#define ESP_ResetPin_Pin GPIO_PIN_8
#define ESP_ResetPin_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
