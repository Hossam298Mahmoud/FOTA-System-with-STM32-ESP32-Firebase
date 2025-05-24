/*
 * tim.c
 *
 *  Created on: Jul 25, 2024
 *      Author: HossamMahmoud
 */

#include "tim.h"
/***---------------------------TIM2------------------------------------------***/
void TIM2_Init()
{
	/*1-Enable the APB clock FOR TIM2 */
	RCC->APB1ENR  |= 0x1;

	/*2-Prescaler value
		 The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).*/
	TIM2->PSC=0;

	/*3-ARR is the value to be loaded in the actual auto-reload register.
	  (1 MHz / 1000) = 1KHz = 1ms  So, this will generate the 1ms delay */
	TIM2->ARR = 7999;

	/*4-Enable TIM2 module */
	TIM2->CR1 = (1 << 0);

}

void TIM2_Delay_ms(uint32_t MS)
{
	for(uint32_t i =0 ; i<MS ; i++)
	{
		while (!(TIM2->SR & 0x01));   // Check UIF: Update interrupt flag

		TIM2->SR &= ~(0x01);  /* Clear UIF */
	}
}


void TIM2_CNTUP_Init()
{
	/*1-Enable the APB clock FOR TIM2 */
	RCC->APB1ENR  |= 0x1;

	/*2-Prescaler value
		 The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).
	TIM2->PSC=0;*/
	TIM2->CNT =0;
	/*3-ARR is the value to be loaded in the actual auto-reload register.
	  (1 MHz / 1000) = 1KHz = 1ms  So, this will generate the 1ms delay */
	TIM2->ARR = 5;

	 // 3. Configure TIM2 for External Clock Mode 2
	TIM2->SMCR &= ~(0b111 << 0);   // Clear SMS bits
	TIM2->SMCR |= (0b111 << 0);    // Set SMS to 111 for External Clock Mode 2


	TIM2->SMCR &= ~(0b1<< 15); //Bit 15 ETP: External trigger polarity

	TIM2->SMCR &= ~(0b11<< 12);//Bits 13:12 ETPS: External trigger prescaler

	//TIM2->SMCR |=(0b111<< 4);//Bits 6:4 TS: Trigger selection

	TIM2->SMCR |= (0b1<< 14);   //Bit 14 ECE: External clock enable

	/*4-Enable TIM2 module */
	TIM2->CR1 |= (1 << 0);

}


void TIM2_PWM_Init()
{
	/*1-Enable the APB clock FOR TIM2 */
	RCC->APB1ENR  |= 0x1;

	/*2-Prescaler value
		 The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).*/
	TIM2->PSC = 15;

	/*3-ARR is the value to be loaded in the actual auto-reload register.
	  (1 MHz / 1000) = 1KHz = 1ms  So, this will generate the 1ms delay

	  Pulse width modulation mode allows generating a signal with a frequency determined by the
		value of the TIMx_ARR register
	  */
	TIM2->ARR =999;

	/*4-The PWM mode can be selected independently on each channel (one PWM per OCx
		output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
		TIMx_CCMRx register.*/
	TIM2->CCMR1 |= (0b110<<4);  //Bits 6:4 OC1M: Output compare 1 mode

	TIM2->CCMR1 |= (0b110<<12);  //Bits 14:12 OC2M[2:0]: Output compare 2 mode

	//5-setting the OCxPE bit
	TIM2->CCMR1 |= (0b1<<3);	//Bit 3 OC1PE: Output compare 1 preload enable

	//5-setting the OCxPE bit
	TIM2->CCMR1 |= (0b1<<11);	//Bit 11 OC2PE: Output compare 2 preload enable

	//TIM2->CR1 |= (1 << 7);//Bit 7 ARPE: Auto-reload preload enable

	//6-duty cycle determined by the value of the TIMx_CCRx register
	TIM2->CCR1 =0;

	//6-duty cycle determined by the value of the TIMx_CCRx register
	TIM2->CCR2 =0;


	//7-OCx output is enabled by the CCxE bit in the TIMx_CCER register.
	TIM2->CCER |= (0b1<<0); //Bit 0 CC1E: Capture/Compare 1 output enable.

	//7-OCx output is enabled by the CCxE bit in the TIMx_CCER register.
	TIM2->CCER |= (0b1<<4); //Bit 4 CC2E: Capture/Compare 2 output enable.


	TIM2->CR1 |= (0b00 << 5); //Bits 6:5 CMS: Center-aligned mode selection

	/*8-Enable TIM2 module */
	TIM2->CR1 |= (1 << 0);

}

/*--------------------##########################-------------------------------*/

