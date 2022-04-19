/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
//void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_GREEN_BOARD_Pin GPIO_PIN_13
#define LED_GREEN_BOARD_GPIO_Port GPIOC
#define KEY_BOARD_Pin GPIO_PIN_0
#define KEY_BOARD_GPIO_Port GPIOA
#define LEFT_RESISTOR_Pin GPIO_PIN_1
#define LEFT_RESISTOR_GPIO_Port GPIOA
#define RIGHT_RESISTOR_Pin GPIO_PIN_2
#define RIGHT_RESISTOR_GPIO_Port GPIOA
#define BRASLET_Pin GPIO_PIN_3
#define BRASLET_GPIO_Port GPIOA
#define SHUP_Pin GPIO_PIN_4
#define SHUP_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOB
#define KEY_UP_Pin GPIO_PIN_14
#define KEY_UP_GPIO_Port GPIOB
#define KEY_DOWN_Pin GPIO_PIN_15
#define KEY_DOWN_GPIO_Port GPIOB
#define KEY_ESC_Pin GPIO_PIN_4
#define KEY_ESC_GPIO_Port GPIOB
#define KEY_ENTER_Pin GPIO_PIN_5
#define KEY_ENTER_GPIO_Port GPIOB
#define KEY_LEFT_Pin GPIO_PIN_6
#define KEY_LEFT_GPIO_Port GPIOB
#define KEY_RIGHT_Pin GPIO_PIN_7
#define KEY_RIGHT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif


//extern void MY_ADC_Start_DMA(void);

extern uint32_t hdma_ErrorCode;

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
inline void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
}


#endif /* __MAIN_H */
