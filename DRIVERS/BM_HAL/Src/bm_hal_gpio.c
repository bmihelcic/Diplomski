/*
 * bm_hal_gpio.c
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */

#include "bm_hal_gpio.h"
#include "stm32f1xx_ll_gpio.h"


/**
  * @brief  Get the level of a pin configured in input mode
  * @param  GPIOx: where x can be (A..G depending on device used) to select the GPIO peripheral
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval The input pin state, see BM_HAL_GPIO_pin_state_t.
  */
BM_HAL_GPIO_pin_state_t BM_HAL_GPIO_digitalRead(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	BM_HAL_GPIO_pin_state_t pin_value;

	if ((LL_GPIO_ReadInputPort(GPIOx) & ((PinMask >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)) != (uint32_t)GPIO_LOW)
	{
		pin_value = GPIO_HIGH;
	}
	else
	{
		pin_value = GPIO_LOW;
	}
	return pin_value;
}


/**
  * @brief  To set/reset the level of a pin configured in output mode
  *
  * @param  GPIOx: where x can be (A..G depending on device used) to select the GPIO peripheral
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @param  pin_state: specifies the value to be written to the selected bit.
  *          This parameter can be one of the BM_HAL_GPIO_pin_state_t enum values:
  *            @arg GPIO_LOW: to clear the port pin
  *            @arg GPIO_HIGH: to set the port pin
  * @retval None
  */
void BM_HAL_GPIO_digitalWrite(GPIO_TypeDef *GPIOx, uint32_t PinMask, BM_HAL_GPIO_pin_state_t pin_state)
{
	if (pin_state != GPIO_LOW)
	{
		LL_GPIO_SetOutputPin(GPIOx, PinMask);
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOx, PinMask);
	}
}

/**
  * @brief  Toggles pin output
  * @param  GPIOx: where x can be (A..G depending on device used) to select the GPIO peripheral
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref LL_GPIO_PIN_0
  *         @arg @ref LL_GPIO_PIN_1
  *         @arg @ref LL_GPIO_PIN_2
  *         @arg @ref LL_GPIO_PIN_3
  *         @arg @ref LL_GPIO_PIN_4
  *         @arg @ref LL_GPIO_PIN_5
  *         @arg @ref LL_GPIO_PIN_6
  *         @arg @ref LL_GPIO_PIN_7
  *         @arg @ref LL_GPIO_PIN_8
  *         @arg @ref LL_GPIO_PIN_9
  *         @arg @ref LL_GPIO_PIN_10
  *         @arg @ref LL_GPIO_PIN_11
  *         @arg @ref LL_GPIO_PIN_12
  *         @arg @ref LL_GPIO_PIN_13
  *         @arg @ref LL_GPIO_PIN_14
  *         @arg @ref LL_GPIO_PIN_15
  *         @arg @ref LL_GPIO_PIN_ALL
  * @retval None
  */
void BM_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_TogglePin(GPIOx, PinMask);
}
