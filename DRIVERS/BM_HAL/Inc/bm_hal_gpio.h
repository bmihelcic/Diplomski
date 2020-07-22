/*
 * bm_hal_gpio.h
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */

#ifndef INC_BM_HAL_GPIO_H_
#define INC_BM_HAL_GPIO_H_

//#ifdef STM32
#include "stm32f103xb.h"

//#elif defined(VIRTUAL_MCU)
#include <stdint.h>

//#define GPIOA (0u)
//#define GPIOB (1u)
#define LL_GPIO_PIN_0 (0u)
#define LL_GPIO_PIN_1 (1u)
#define LL_GPIO_PIN_2 (2u)
#define LL_GPIO_PIN_3 (3u)

//#endif



typedef enum
{
  GPIO_LOW = 0u,
  GPIO_HIGH
} BM_HAL_GPIO_pin_state_t;



BM_HAL_GPIO_pin_state_t BM_HAL_GPIO_digitalRead(GPIO_TypeDef *GPIOx, uint32_t pin_mask);
void BM_HAL_GPIO_digitalWrite(GPIO_TypeDef *GPIOx, uint32_t pin_mask, BM_HAL_GPIO_pin_state_t pin_state);
void BM_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t pin_mask);

#endif /* INC_BM_HAL_GPIO_H_ */
