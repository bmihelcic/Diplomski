/*
 * hal.c
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */
#include "bm_hal.h"
#include "bm_hal_gpio.h"
#include "bm_hal_adc.h"


int main(void)
{
    uint32_t adcValue = 0;
    size_t flag = 0;


    BM_HAL_init();
    BM_HAL_ADC_init(ADC1);



    while (1)
    {
        flag = (flag == 0) ? 1 : 0;
//        BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port, INPUT0_Pin));
//        BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port, OUTPUT1_Pin, BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port, INPUT1_Pin));
//        BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port, OUTPUT2_Pin, BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port, INPUT2_Pin));
//        BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port, INPUT3_Pin));
        BM_HAL_GPIO_TogglePin(ON_BOARD_LED_GPIO_Port,ON_BOARD_LED_Pin);

        if(0 == flag)
        {
            adcValue = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_0);
            if(adcValue > 2000)
            {
                BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_HIGH);
            }
            else
            {
                BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_LOW);
            }
        }
        else
        {
            adcValue = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_1);
            if(adcValue > 2000)
            {
                BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_HIGH);
            }
            else
            {
                BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_LOW);
            }
        }
    }
}
