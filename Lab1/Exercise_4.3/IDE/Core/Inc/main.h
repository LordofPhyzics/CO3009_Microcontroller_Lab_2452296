/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RED_N_Pin GPIO_PIN_0
#define RED_N_GPIO_Port GPIOA
#define YEL_N_Pin GPIO_PIN_1
#define YEL_N_GPIO_Port GPIOA
#define GRN_N_Pin GPIO_PIN_2
#define GRN_N_GPIO_Port GPIOA
#define RED_S_Pin GPIO_PIN_3
#define RED_S_GPIO_Port GPIOA
#define YEL_S_Pin GPIO_PIN_4
#define YEL_S_GPIO_Port GPIOA
#define GRN_S_Pin GPIO_PIN_5
#define GRN_S_GPIO_Port GPIOA
#define RED_E_Pin GPIO_PIN_6
#define RED_E_GPIO_Port GPIOA
#define YEL_E_Pin GPIO_PIN_7
#define YEL_E_GPIO_Port GPIOA
#define GRN_E_Pin GPIO_PIN_8
#define GRN_E_GPIO_Port GPIOA
#define RED_W_Pin GPIO_PIN_9
#define RED_W_GPIO_Port GPIOA
#define YEL_W_Pin GPIO_PIN_10
#define YEL_W_GPIO_Port GPIOA
#define GRN_W_Pin GPIO_PIN_11
#define GRN_W_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
