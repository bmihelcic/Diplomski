/*
 * bm_hal_gpio.h
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */

#ifndef INC_BM_HAL_GPIO_H_
#define INC_BM_HAL_GPIO_H_

#include <stdint.h>

#ifdef STM32F103xB
#include "stm32f103xb.h"
#elif defined(VIRTUAL_MCU)
// unused, needed for compatibility with stm32 api
#define GPIOA (0)
#define GPIOB (0)

typedef struct{

}GPIO_TypeDef;

extern int outputPort[4];
extern int inputPort[4];

#endif


typedef enum
{
  BM_GPIO_LOW = 0u,
  BM_GPIO_HIGH
} BM_HAL_GPIO_pin_state_t;


void BM_HAL_GPIO_init();
BM_HAL_GPIO_pin_state_t BM_HAL_GPIO_digitalRead(GPIO_TypeDef *GPIOx, uint32_t pin_mask);
void BM_HAL_GPIO_digitalWrite(GPIO_TypeDef *GPIOx, uint32_t pin_mask, BM_HAL_GPIO_pin_state_t pin_state);
void BM_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t pin_mask);

#endif /* INC_BM_HAL_GPIO_H_ */
