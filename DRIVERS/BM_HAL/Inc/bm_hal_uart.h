/*
 * bm_hal_uart.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Branimir
 */

#ifndef BM_HAL_INC_BM_HAL_UART_H_
#define BM_HAL_INC_BM_HAL_UART_H_

#include "stm32f1xx_hal_uart.h"

extern UART_HandleTypeDef huart1;


void BM_HAL_UART_init(void);
int BM_HAL_UART_printf(const char *fmt, ...);


#endif /* BM_HAL_INC_BM_HAL_UART_H_ */
