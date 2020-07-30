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

extern SYSTEM_STATE_E systemState;

int main(void)
{
    uint16_t adc_val_0 = 0;
    uint16_t adc_val_1 = 0;
    uint8_t inputState = 0;

    BM_HAL_init();
    BM_HAL_GPIO_init();
    BM_HAL_ADC_init();
    BM_HAL_CAN_init();
    BM_HAL_UART_init();

    systemState = SYSTEM_STATE_MEASUREMENT;

    while (1)
    {
        switch(systemState)
        {
            case SYSTEM_STATE_MEASUREMENT:
            {
                // INPUT0 CHECK
                if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port, INPUT0_Pin))
                {
                    inputState |= 1;
                    BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_HIGH);
                }
                else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT0_GPIO_Port, INPUT0_Pin))
                {
                    inputState &=  ~1;
                    BM_HAL_GPIO_digitalWrite(OUTPUT0_GPIO_Port, OUTPUT0_Pin, BM_GPIO_LOW);
                }

                // INPUT1 CHECK
                if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port, INPUT1_Pin))
                {
                    inputState |= (1 << 1);
                    BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port, OUTPUT1_Pin, BM_GPIO_HIGH);
                }
                else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT1_GPIO_Port, INPUT1_Pin))
                {
                    inputState &=  ~(1 << 1);
                    BM_HAL_GPIO_digitalWrite(OUTPUT1_GPIO_Port, OUTPUT1_Pin, BM_GPIO_LOW);
                }


                // INPUT2 CHECK
                if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port, INPUT2_Pin))
                {
                    inputState |= (1 << 2);
                    BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port, OUTPUT2_Pin, BM_GPIO_HIGH);
                }
                else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT2_GPIO_Port, INPUT2_Pin))
                {
                    inputState &=  ~(1 << 2);
                    BM_HAL_GPIO_digitalWrite(OUTPUT2_GPIO_Port, OUTPUT2_Pin, BM_GPIO_LOW);
                }


                // INPUT3 CHECK
                if(BM_GPIO_LOW == BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port, INPUT3_Pin))
                {
                    inputState |= (1 << 3);
                    BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_HIGH);
                }
                else if(BM_GPIO_HIGH == BM_HAL_GPIO_digitalRead(INPUT3_GPIO_Port, INPUT3_Pin))
                {
                    inputState &=  ~(1 << 3);
                    BM_HAL_GPIO_digitalWrite(OUTPUT3_GPIO_Port, OUTPUT3_Pin, BM_GPIO_LOW);
                }

                CAN_tx_data[0] = inputState;
                adc_val_0 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_0);
                CAN_tx_data[1] = adc_val_0;
                CAN_tx_data[2] = (adc_val_0 >> 8) & 0x0F;
                adc_val_1 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_1);
                CAN_tx_data[3] = adc_val_1;
                CAN_tx_data[4] = (adc_val_1 >> 8) & 0x0F;

                systemState = SYSTEM_STATE_CAN_OUT;
                break;
            }
            case SYSTEM_STATE_CAN_OUT:
            {
                BM_HAL_CAN_send(CAN_tx_data, 5);
                systemState = SYSTEM_STATE_UART_OUT;
                break;
            }
            case SYSTEM_STATE_UART_OUT:
            {
#ifdef STM32F103xB
                BM_HAL_UART_printf("CAN TX MSG: 0x%x : 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", ownID, CAN_tx_data[0],
                        CAN_tx_data[1], CAN_tx_data[2], CAN_tx_data[3], CAN_tx_data[4]);
                BM_HAL_UART_printf("INPUT: %d  ADC0: %d  ADC1: %d\r\n\n", inputState, adcValue1, adcValue2);
#elif defined(VIRTUAL_MCU)
#endif
                systemState = SYSTEM_STATE_MEASUREMENT;
                break;
            }
            default:
            {
                systemState = SYSTEM_STATE_MEASUREMENT;
            }
        }
#ifdef STM32F103xB
        BM_HAL_delay(50);
#elif defined(VIRTUAL_MCU)
        BM_HAL_delay(500);
#endif
   }
}
