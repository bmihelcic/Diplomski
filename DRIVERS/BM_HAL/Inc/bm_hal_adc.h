/*
 * bm_hal_adc.h
 *
 *  Created on: Jul 23, 2020
 *      Author: Branimir
 */

#ifndef BM_HAL_INC_BM_HAL_ADC_H_
#define BM_HAL_INC_BM_HAL_ADC_H_


#include "bm_hal.h"

// unused, needed for compatibility with stm32 api
#ifdef VIRTUAL_MCU
typedef struct {
}ADC_TypeDef;

#define LL_ADC_CHANNEL_0 (0u)
#define LL_ADC_CHANNEL_1 (1u)

ADC_TypeDef* ADC1;
ADC_TypeDef* ADC2;

extern uint16_t adcValue0;
extern uint16_t adcValue1;
#endif



void BM_HAL_ADC_init();
uint32_t BM_HAL_ADC_readChannel(ADC_TypeDef *ADCx, uint32_t channel);


#endif /* BM_HAL_INC_BM_HAL_ADC_H_ */
