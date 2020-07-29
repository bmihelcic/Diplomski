/*
 * hal.c
 *
 *  Created on: 25. tra 2020.
 *      Author: branimir
 */
#include "bm_hal.h"
#include "bm_hal_gpio.h"
#include "bm_hal_adc.h"
#include "bm_hal_can.h"
#include "bm_hal_uart.h"

int main(void)
{
    uint16_t adcValue1 = 0;
    uint16_t adcValue2 = 0;

    BM_HAL_init();
    BM_HAL_GPIO_init();
    BM_HAL_ADC_init();
    BM_HAL_CAN_init();
    BM_HAL_UART_init();

    while (1)
    {
        // INPUT0 CHECK
        if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port, INPUT0_Pin))
        {
            CAN_tx_data[0] |= 1;
            BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_HIGH);
        }
        else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port, INPUT0_Pin))
        {
            CAN_tx_data[0] &=  ~1;
            BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_LOW);
        }

        // INPUT1 CHECK
        if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port, INPUT1_Pin))
        {
            CAN_tx_data[0] |= (1 << 1);
            BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port, OUTPUT1_Pin, BM_GPIO_HIGH);
        }
        else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port, INPUT1_Pin))
        {
            CAN_tx_data[0] &=  ~(1 << 1);
            BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port, OUTPUT1_Pin, BM_GPIO_LOW);
        }


        // INPUT2 CHECK
        if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port, INPUT2_Pin))
        {
            CAN_tx_data[0] |= (1 << 2);
            BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port, OUTPUT2_Pin, BM_GPIO_HIGH);
        }
        else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port, INPUT2_Pin))
        {
            CAN_tx_data[0] &=  ~(1 << 2);
            BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port, OUTPUT2_Pin, BM_GPIO_LOW);
        }


        // INPUT3 CHECK
        if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port, INPUT3_Pin))
        {
            CAN_tx_data[0] |= (1 << 3);
            BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_HIGH);
        }
        else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port, INPUT3_Pin))
        {
            CAN_tx_data[0] &=  ~(1 << 3);
            BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_LOW);
        }

        adcValue1 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_0);
        CAN_tx_data[1] = adcValue1;
        CAN_tx_data[2] = (adcValue1 >> 8) & 0x0F;
        adcValue2 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_1);
        CAN_tx_data[3] = adcValue2;
        CAN_tx_data[4] = (adcValue2 >> 8) & 0x0F;

        BM_HAL_CAN_send(CAN_tx_data, 5);

        HAL_Delay(200);

        BM_HAL_UART_printf("%d\t%d\t%d\r\n", CAN_tx_data[0], adcValue1, adcValue2);

   }
}
