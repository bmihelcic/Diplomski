/*
 * hal.h
 *
 *  Created on: 16. tra 2020.
 *      Author: branimir
 */

#ifndef HAL_INC_HAL_H_
#define HAL_INC_HAL_H_

#include "stm32f1xx_hal.h"

#define ON_BOARD_LED_Pin GPIO_PIN_13
#define ON_BOARD_LED_GPIO_Port GPIOC
#define OUTPUT0_Pin GPIO_PIN_2
#define OUTPUT0_GPIO_Port GPIOA
#define OUTPUT1_Pin GPIO_PIN_3
#define OUTPUT1_GPIO_Port GPIOA
#define OUTPUT2_Pin GPIO_PIN_4
#define OUTPUT2_GPIO_Port GPIOA
#define OUTPUT3_Pin GPIO_PIN_5
#define OUTPUT3_GPIO_Port GPIOA
#define INPUT0_Pin GPIO_PIN_6
#define INPUT0_GPIO_Port GPIOA
#define INPUT1_Pin GPIO_PIN_7
#define INPUT1_GPIO_Port GPIOA
#define INPUT2_Pin GPIO_PIN_8
#define INPUT2_GPIO_Port GPIOA
#define INPUT3_Pin GPIO_PIN_15
#define INPUT3_GPIO_Port GPIOA

void HAL_init();
void HAL_deInit();
void SystemClock_Config(void);
void Error_Handler(void);

#endif /* HAL_INC_HAL_H_ */
