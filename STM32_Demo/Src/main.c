/*
 * hal.c
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */
#include "bm_hal.h"
#include "bm_hal_gpio.h"


int main(void)
{
	BM_HAL_init();
	while (1)
	{
		if(BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port,INPUT0_Pin) == GPIO_LOW)
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port,OUTPUT0_Pin,GPIO_HIGH);
		}
		else
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port,OUTPUT0_Pin,GPIO_LOW);
		}

		if(BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port,INPUT1_Pin) == GPIO_LOW)
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port,OUTPUT1_Pin,GPIO_HIGH);
		}
		else
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port,OUTPUT1_Pin,GPIO_LOW);
		}


		if(BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port,INPUT2_Pin) == GPIO_LOW)
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port,OUTPUT2_Pin,GPIO_HIGH);
		}
		else
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port,OUTPUT2_Pin,GPIO_LOW);
		}


		if(BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port,INPUT3_Pin) == GPIO_LOW)
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port,OUTPUT3_Pin,GPIO_HIGH);
		}
		else
		{
			BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port,OUTPUT3_Pin,GPIO_LOW);
		}
		HAL_Delay(100);
		BM_HAL_GPIO_TogglePin(ON_BOARD_LED_GPIO_Port,ON_BOARD_LED_Pin);
	}
}
