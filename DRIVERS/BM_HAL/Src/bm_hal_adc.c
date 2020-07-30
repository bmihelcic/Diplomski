/*
 * bm_hal_adc.c
 *
 *  Created on: Jul 23, 2020
 *      Author: Branimir
 */
#include "bm_hal_adc.h"


uint16_t adcValue0 = 0;
uint16_t adcValue1 = 0;
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void BM_HAL_ADC_init()
{
#ifdef STM32F103xB
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    /**ADC1 GPIO Configuration
    PA0-WKUP   ------> ADC1_IN0
    PA1   ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /** Common config
    */
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);
    ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
    LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    /** Configure Regular Channel
    */
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_1CYCLE_5);

    LL_ADC_Enable(ADC1);
#endif
}


uint32_t BM_HAL_ADC_readChannel(ADC_TypeDef *ADCx, uint32_t channel)
{
    uint32_t adc_value = 0;
#ifdef STM32F103xB
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, channel);
    LL_ADC_REG_StartConversionSWStart(ADCx);
    while(!LL_ADC_IsActiveFlag_EOS(ADCx));
    adc_value = LL_ADC_REG_ReadConversionData32(ADCx);
#elif defined(VIRTUAL_MCU)
    if(LL_ADC_CHANNEL_0 == channel)
    {
        adc_value = adcValue0;
    }
    else if(LL_ADC_CHANNEL_1 == channel)
    {
        adc_value = adcValue1;
    }
#endif
    return adc_value;
}
