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
    uint16_t adcValueReceived1 = 0;
    uint16_t adcValueReceived2 = 0;
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
                adcValue1 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_0);
                CAN_tx_data[1] = adcValue1;
                CAN_tx_data[2] = (adcValue1 >> 8) & 0x0F;
                adcValue2 = BM_HAL_ADC_readChannel(ADC1, LL_ADC_CHANNEL_1);
                CAN_tx_data[3] = adcValue2;
                CAN_tx_data[4] = (adcValue2 >> 8) & 0x0F;

                systemState = SYSTEM_STATE_CAN_OUT;
                break;
            }
            case SYSTEM_STATE_CAN_OUT:
            {
                BM_HAL_CAN_send(CAN_tx_data, 5);
                BM_HAL_UART_printf("INPUT: %d  ADC0: %d  ADC1: %d\r\n", inputState, adcValue1, adcValue2);
                BM_HAL_UART_printf("CAN TX MSG: 0x%x : 0x%x 0x%x 0x%x 0x%x 0x%x\r\n\n", ownID, CAN_tx_data[0],
                        CAN_tx_data[1], CAN_tx_data[2], CAN_tx_data[3], CAN_tx_data[4]);
                break;
            }
            case SYSTEM_STATE_UART_OUT:
            {
                adcValueReceived1 = CAN_rx_data[1] | ((CAN_rx_data[2] << 8) & 0b111100000000);
                adcValueReceived2 = CAN_rx_data[3] | ((CAN_rx_data[4] << 8) & 0b111100000000);
                BM_HAL_UART_printf("CAN RX MSG: 0x%x : 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", CAN_rx_header.StdId, CAN_rx_data[0],
                        CAN_rx_data[1], CAN_rx_data[2], CAN_rx_data[3], CAN_rx_data[4]);
                BM_HAL_UART_printf("INPUT: %d  ADC0: %d  ADC1: %d\r\n", CAN_rx_data[0], adcValueReceived1, adcValueReceived2);
                BM_HAL_UART_printf("\n");
                systemState = SYSTEM_STATE_MEASUREMENT;
                break;
            }
            default:
            {
                systemState = SYSTEM_STATE_MEASUREMENT;
            }
        }
        HAL_Delay(500);
   }
}
