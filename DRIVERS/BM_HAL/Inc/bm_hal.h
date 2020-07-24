/*
 * bm_hal.h
 *
 *  Created on: 16. tra 2020.
 *      Author: branimir
 */

#ifndef BM_HAL_INC_HAL_H_
#define BM_HAL_INC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef STM32F103xB
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

#define ON_BOARD_LED_Pin LL_GPIO_PIN_13
#define ON_BOARD_LED_GPIO_Port GPIOC
#define OUTPUT0_Pin LL_GPIO_PIN_2
#define OUTPUT0_GPIO_Port GPIOA
#define OUTPUT1_Pin LL_GPIO_PIN_3
#define OUTPUT1_GPIO_Port GPIOA
#define OUTPUT2_Pin LL_GPIO_PIN_4
#define OUTPUT2_GPIO_Port GPIOA
#define OUTPUT3_Pin LL_GPIO_PIN_5
#define OUTPUT3_GPIO_Port GPIOA
#define INPUT1_Pin LL_GPIO_PIN_7
#define INPUT1_GPIO_Port GPIOA
#define INPUT0_Pin LL_GPIO_PIN_0
#define INPUT0_GPIO_Port GPIOB
#define INPUT2_Pin LL_GPIO_PIN_8
#define INPUT2_GPIO_Port GPIOA
#define INPUT3_Pin LL_GPIO_PIN_15
#define INPUT3_GPIO_Port GPIOA

typedef enum ERRORS{
    ERROR_OK,
    ERROR_INVALID_HEX
}ERROR_E;


CAN_HandleTypeDef hcan;

#include <math.h>
#include <stdio.h>
#include <string.h>

#elif defined(VIRTUAL_MCU)
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdint.h>
#include <tchar.h>

#define OUTPUT0_Pin LL_GPIO_PIN_0
#define OUTPUT0_GPIO_Port GPIOA
#define OUTPUT1_Pin LL_GPIO_PIN_1
#define OUTPUT1_GPIO_Port GPIOA
#define OUTPUT2_Pin LL_GPIO_PIN_2
#define OUTPUT2_GPIO_Port GPIOA
#define OUTPUT3_Pin LL_GPIO_PIN_3
#define OUTPUT3_GPIO_Port GPIOA

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_PORT_STR        "8080"
#define DEFAULT_PORT            (8080)
#endif

int stringToHex(char*);
int hexCharToDec(char, ERROR_E*);


void BM_HAL_init();
void Error_Handler(void);

#ifdef __cplusplus
}
#endif
#endif /* BM_HAL_INC_HAL_H_ */
