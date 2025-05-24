/*
 * tim.h
 *
 *  Created on: Jul 25, 2024
 *      Author: HossamMahmoud
 */

#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>
#include "stm32f103xb.h"

/*******************************FUNCTIONS FOR TIMERS**************************************************/
void TIM2_Init();

void TIM2_Delay_ms(uint32_t MS);

void TIM2_CNTUP_Init();

#endif /* TIM_H_ */
