/*
 * hal.c
 *
 *  Created on: 16. tra 2020.
 *      Author: branimir
 */

#include "hal.h"

ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan;

UART_HandleTypeDef huart1;

static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);


void HAL_init(){
#ifdef STM32F103xB
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_Init();
	SystemClock_Config();
	MX_ADC1_Init();
	MX_CAN_Init();
	MX_USART1_UART_Init();
	MX_GPIO_Init();
#elif defined(VIRTUAL_MCU)

#endif
}

void HAL_deInit(){
#ifdef STM32F103xB
	  if(hadc1.Instance==ADC1)
	  {
	    /* Peripheral clock disable */
	    __HAL_RCC_ADC1_CLK_DISABLE();

	    /**ADC1 GPIO Configuration
	    PA0-WKUP     ------> ADC1_IN0
	    PA1     ------> ADC1_IN1
	    */
	    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);
	  }

	  if(hcan.Instance==CAN1)
	  {
	    /* Peripheral clock disable */
	    __HAL_RCC_CAN1_CLK_DISABLE();

	    /**CAN GPIO Configuration
	    PA11     ------> CAN_RX
	    PA12     ------> CAN_TX
	    */
	    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);
	  }

	  if(huart1.Instance==USART1)
	   {
	     /* Peripheral clock disable */
	     __HAL_RCC_USART1_CLK_DISABLE();

	     /**USART1 GPIO Configuration
	     PA9     ------> USART1_TX
	     PA10     ------> USART1_RX
	     */
	     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
	   }
#elif defined(VIRTUAL_MCU)

#endif
}




/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
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
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
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
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OUTPUT0_Pin|OUTPUT1_Pin|OUTPUT2_Pin|OUTPUT3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ON_BOARD_LED_Pin */
  GPIO_InitStruct.Pin = ON_BOARD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ON_BOARD_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OUTPUT0_Pin OUTPUT1_Pin OUTPUT2_Pin OUTPUT3_Pin */
  GPIO_InitStruct.Pin = OUTPUT0_Pin|OUTPUT1_Pin|OUTPUT2_Pin|OUTPUT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INPUT0_Pin INPUT1_Pin INPUT2_Pin INPUT3_Pin */
  GPIO_InitStruct.Pin = INPUT0_Pin|INPUT1_Pin|INPUT2_Pin|INPUT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  if(hadc1.Instance==ADC1)
  {
	  /* Peripheral clock enable */
      __HAL_RCC_ADC1_CLK_ENABLE();
      /**ADC1 GPIO Configuration
      PA0-WKUP     ------> ADC1_IN0
      PA1     ------> ADC1_IN1
      */
      GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  if(hcan.Instance==CAN1)
  {
	  /* Peripheral clock enable */
      __HAL_RCC_CAN1_CLK_ENABLE();
      /**CAN GPIO Configuration
      PA11     ------> CAN_RX
      PA12     ------> CAN_TX
      */
      GPIO_InitStruct.Pin = GPIO_PIN_11;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_12;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
  if(huart1.Instance==USART1)
  {
     /* Peripheral clock enable */
     __HAL_RCC_USART1_CLK_ENABLE();
     /**USART1 GPIO Configuration
     PA9     ------> USART1_TX
     PA10     ------> USART1_RX
     */
     GPIO_InitStruct.Pin = GPIO_PIN_9;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

     GPIO_InitStruct.Pin = GPIO_PIN_10;
     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */

}

