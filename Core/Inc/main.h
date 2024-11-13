/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
<<<<<<< HEAD
  * Copyright (c) 2024 STMicroelectronics.
=======
  * Copyright (c) 2023 STMicroelectronics.
>>>>>>> master
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
#include "stm32g4xx_hal.h"

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
<<<<<<< HEAD
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define RCC_OSC32_OUT_Pin GPIO_PIN_14
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define RCC_OSC32_OUTC15_Pin GPIO_PIN_15
#define RCC_OSC32_OUTC15_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
=======
#define USR_BTN_Pin GPIO_PIN_13
#define USR_BTN_GPIO_Port GPIOC
#define USR_BTN_EXTI_IRQn EXTI15_10_IRQn
#define U_VPh_Pin GPIO_PIN_0
#define U_VPh_GPIO_Port GPIOC
#define W_VPh_Pin GPIO_PIN_1
#define W_VPh_GPIO_Port GPIOC
#define Bus_Imes_Pin GPIO_PIN_2
#define Bus_Imes_GPIO_Port GPIOC
#define V_VPh_Pin GPIO_PIN_3
#define V_VPh_GPIO_Port GPIOC
#define Bus_V_Pin GPIO_PIN_0
#define Bus_V_GPIO_Port GPIOA
#define U_Imes_Pin GPIO_PIN_1
#define U_Imes_GPIO_Port GPIOA
#define USR_LED_Pin GPIO_PIN_5
#define USR_LED_GPIO_Port GPIOA
#define U_PWM_L_Pin GPIO_PIN_13
#define U_PWM_L_GPIO_Port GPIOB
#define V_PWM_L_Pin GPIO_PIN_14
#define V_PWM_L_GPIO_Port GPIOB
#define W_PWM_L_Pin GPIO_PIN_15
#define W_PWM_L_GPIO_Port GPIOB
#define U_PWM_H_Pin GPIO_PIN_8
#define U_PWM_H_GPIO_Port GPIOA
#define V_PWM_H_Pin GPIO_PIN_9
#define V_PWM_H_GPIO_Port GPIOA
#define W_PWM_H_Pin GPIO_PIN_10
#define W_PWM_H_GPIO_Port GPIOA
>>>>>>> master
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
<<<<<<< HEAD
=======
#define NRST_SafetyUC_Pin GPIO_PIN_2
#define NRST_SafetyUC_GPIO_Port GPIOD
>>>>>>> master
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
