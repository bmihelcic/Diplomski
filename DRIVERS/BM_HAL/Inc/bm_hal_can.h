/*
 * bm_hal_can.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Branimir
 */

#ifndef BM_HAL_INC_BM_HAL_CAN_H_
#define BM_HAL_INC_BM_HAL_CAN_H_

#include "bm_hal.h"

extern uint8_t CAN_rx_data[8];
extern uint8_t CAN_tx_data[8];


void BM_HAL_CAN_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *canHandle);


#endif /* BM_HAL_INC_BM_HAL_CAN_H_ */
