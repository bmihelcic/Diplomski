/*
 * bm_hal_uart.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Branimir
 */

#include "bm_hal.h"
#include "bm_hal_uart.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


UART_HandleTypeDef huart1;

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void BM_HAL_UART_init(void)
{
#ifdef STM32F103xB
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
#endif
}



int BM_HAL_UART_printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    int n = 0;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
#ifdef STM32F103xB
    HAL_UART_Transmit(&huart1, (uint8_t*) buf, strlen(buf), 0xFFFF);
#elif defined (VIRTUAL_MCU)
    printf("%s", buf);
    fflush(stdout);
#endif
    return n;
}
