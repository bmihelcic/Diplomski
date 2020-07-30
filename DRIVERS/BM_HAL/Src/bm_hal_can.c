/*
 * bm_hal_can.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Branimir
 */

#include "bm_hal_can.h"

CAN_HandleTypeDef hcan;

static void BM_HAL_CAN_initFilter();

uint8_t CAN_rx_data[8] = {0};
uint8_t CAN_tx_data[8] = {0};

CAN_RxHeaderTypeDef CAN_rx_header;

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
void BM_HAL_CAN_init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 9;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }

  BM_HAL_CAN_initFilter();
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_Start(&hcan);
}


void BM_HAL_CAN_send(uint8_t *aData, uint8_t dataBytesNum)
{
    CAN_TxHeaderTypeDef CAN_tx_header;
    uint32_t CAN_tx_mailbox;

    CAN_tx_header.StdId = ownID;
    CAN_tx_header.IDE = CAN_ID_STD;
    CAN_tx_header.RTR = CAN_RTR_DATA;
    CAN_tx_header.DLC = dataBytesNum;

    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0u)
    {
        if(HAL_CAN_AddTxMessage(&hcan, &CAN_tx_header, aData, &CAN_tx_mailbox) != HAL_OK)
        {
            Error_Handler();
        }
    }
    while(HAL_CAN_IsTxMessagePending(&hcan,CAN_tx_mailbox));

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *canHandle)
{
    if(HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0, &CAN_rx_header, CAN_rx_data) != HAL_OK)
    {
        Error_Handler();
    }
//    systemState = SYSTEM_STATE_UART_OUT;
}




static void BM_HAL_CAN_initFilter()
{
    CAN_FilterTypeDef filterConfig = {
            .FilterIdHigh = 0x000 <<5,
            .FilterIdLow = 0x0000,
            .FilterMaskIdHigh = 0x0000, //0xFFE0
            .FilterMaskIdLow = 0x0000,
            .FilterFIFOAssignment = 0,
            .FilterBank = 0,
            .FilterMode = CAN_FILTERMODE_IDMASK,
            .FilterScale = CAN_FILTERSCALE_32BIT,
            .FilterActivation = ENABLE,
            .SlaveStartFilterBank = 0,
    };

    HAL_CAN_ConfigFilter(&hcan, &filterConfig);

}
