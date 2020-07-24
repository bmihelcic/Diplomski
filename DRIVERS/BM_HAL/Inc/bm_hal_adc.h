/*
 * bm_hal_adc.h
 *
 *  Created on: Jul 23, 2020
 *      Author: Branimir
 */

#ifndef BM_HAL_INC_BM_HAL_ADC_H_
#define BM_HAL_INC_BM_HAL_ADC_H_


#include "bm_hal.h"

ErrorStatus BM_HAL_ADC_init(ADC_TypeDef *ADCx);
uint32_t BM_HAL_ADC_readChannel(ADC_TypeDef *ADCx, uint32_t channel);


#endif /* BM_HAL_INC_BM_HAL_ADC_H_ */
