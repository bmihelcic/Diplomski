/*
 * bm_hal_adc.c
 *
 *  Created on: Jul 23, 2020
 *      Author: Branimir
 */
#include "bm_hal_adc.h"

ErrorStatus BM_HAL_ADC_init(ADC_TypeDef *ADCx)
{
    ErrorStatus error = SUCCESS;

    LL_ADC_InitTypeDef adcConfig;
    adcConfig.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    adcConfig.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;

    error = LL_ADC_Init(ADCx, &adcConfig);
    LL_ADC_Enable(ADCx);

    return error;
}


uint32_t BM_HAL_ADC_readChannel(ADC_TypeDef *ADCx, uint32_t channel)
{
    uint32_t adc_value = 0;
#ifdef STM32F103xB
    LL_ADC_REG_SetSequencerRanks(ADCx, LL_ADC_REG_RANK_1, channel);
    LL_ADC_REG_StartConversionSWStart(ADCx);
    while(!LL_ADC_IsActiveFlag_EOS(ADCx));
    adc_value = LL_ADC_REG_ReadConversionData32(ADCx);
#endif
    return adc_value;
}
