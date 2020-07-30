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

#ifdef VIRTUAL_MCU
// unused, needed for compatibility with stm32 api
typedef struct{
    uint8_t sendMsgFlag;
}CAN_HandleTypeDef;
typedef struct{
}CAN_RxHeaderTypeDef;
#endif

extern CAN_HandleTypeDef hcan;
extern CAN_RxHeaderTypeDef CAN_rx_header;


void BM_HAL_CAN_init(void);
void BM_HAL_CAN_send(uint8_t *aData, uint8_t dataBytesNum);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *canHandle);


#endif /* BM_HAL_INC_BM_HAL_CAN_H_ */
