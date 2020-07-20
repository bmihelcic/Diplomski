/*
 * bm_hal_gpio.h
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */

#ifndef INC_BM_HAL_GPIO_H_
#define INC_BM_HAL_GPIO_H_

#ifdef STM32
#include "stm32f103xb.h"
#endif

typedef enum
{
  GPIO_LOW = 0u,
  GPIO_HIGH
} BM_HAL_GPIO_pin_state_t;


#ifdef STM32
BM_HAL_GPIO_pin_state_t BM_HAL_GPIO_digitalRead(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void BM_HAL_GPIO_digitalWrite(GPIO_TypeDef *GPIOx, uint32_t PinMask, BM_HAL_GPIO_pin_state_t pin_state);
void BM_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
#endif

#endif /* INC_BM_HAL_GPIO_H_ */
