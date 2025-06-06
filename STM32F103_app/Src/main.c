/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f1xx.h"
#include "gpio.h"
#include "tim.h"
#include "lcd.h"
#include <FPEC.h>
#include "constants.h"
#include "boot.h"

/*-------------------------------------------------------------*/
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void SystemInit(void)
{
	  /*--------- Enabling the HSI clock -------------*/

	/* Set the HSION bit*/
    RCC->CR |= RCC_CR_HSION;

	/* Waiting until HSI clock is ready */
	while(!(RCC->CR & RCC_CR_HSIRDY ));

	/*--------- Enabling the HSE clock -------------*/

	/* Set the HSEON bit*/
	//RCC->CR |= 1<<HSEON;

	/* Waiting until HSE clock is ready */
	//while(!(RCC->CR & 1<<HSE_RDY));

	/*-----------Select Prescaler for different Buses--------*/
	/* Select AHB prescaler to 1 */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1<<RCC_CFGR_HPRE_Pos;

	/* APB1 prescaler to 1 */
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1<<RCC_CFGR_PPRE1_Pos;

	/* APB2 prescaler to 1 */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1<<RCC_CFGR_PPRE2_Pos;

	// System clock switch to HSI
	/*Bits 1:0 SW: System clock switch
		00: HSI oscillator selected as system clock
		01: HSE oscillator selected as system clock
		10: PLL selected as system clock
		11: not allowed
	 */
	RCC->CFGR &= ~ RCC_CFGR_SW_Msk;
	//RCC->CFGR |= RCC_CFGR_SW_HSE;   //HSE oscillator selected
}

void GPIO_Config()
{
	/* Enable the APB2 clock all GPIO port A */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	/* Enable the APB2 clock all GPIO port B */
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	/* Enable the APB2 clock all GPIO port C */
	//RCC->APB2ENR |= RCC_APB2ENR_IOPCEN ;
	GPIO_Init(GPIOA,PIN10,GPIO_OUTPUT_10MHz_PP); //PIN8 in PORTA as OUTPUT_10MHz_PP
	GPIO_Init(GPIOA,PIN11,GPIO_OUTPUT_10MHz_PP); //PIN8 in PORTA as OUTPUT_10MHz_PP
    /******************************LCD PINS*******************************************/
	GPIO_Init(GPIOB,PIN10,GPIO_OUTPUT_10MHz_PP);
	GPIO_Init(GPIOB,PIN11,GPIO_OUTPUT_10MHz_PP);
	GPIO_Init(GPIOB,PIN12,GPIO_OUTPUT_10MHz_PP);
	GPIO_Init(GPIOB,PIN13,GPIO_OUTPUT_10MHz_PP);
	GPIO_Init(GPIOB,PIN14,GPIO_OUTPUT_10MHz_PP);
	GPIO_Init(GPIOB,PIN15,GPIO_OUTPUT_10MHz_PP);
	/********************************************************************************/
	GPIO_Init(GPIOA,PIN0,GPIO_INPUT_PU_PD );   //PIN0 in PORTA as GPIO_INPUT_PU_PD
	SET_BIT(GPIOA->ODR,PIN0); //Enable pull-up Resistance for PIN0 in PORTA
	GPIO_Init(GPIOA,PIN1,GPIO_INPUT_PU_PD );   //PIN1 in PORTA as GPIO_INPUT_PU_PD
	SET_BIT(GPIOA->ODR,PIN1); //Enable pull-up Resistance for PIN1 in PORTA
}
/***********************************************************************************/
volatile uint32_t system_tick_counter=0;

uint32_t GetTick()
{
	return system_tick_counter;
}

void SysTick_Handler(void)
{
	system_tick_counter++;

}
void USART2_IRQHandler(void)
{
	/********************************************************/
	//Bit 5 RXNE: Read data register not empty

	//uart2_Receive = USART2->DR;  // Read the data. This clears the RXNE also

    if (USART2->SR & (1<<5))
    {
        uint8_t uart2_Receive = USART2->DR; // Reading DR clears RXNE
    	if(uart2_Receive==REQEST_UPDATE)
    	{  Update_Flag=1;  }
    }
}

/*****************************************************************************/
void Interrupt_Config (void)
{
	NVIC_SetPriority (USART2_IRQn, 1);  // Set Priority

	NVIC_EnableIRQ (USART2_IRQn);       // Enable Interrupt
}
/*****************************************************************************/
/**********************************************************************/
int main(void)
{
	/***********************Define Variables*************************************/
	//char buff[5]="";
	uint32_t start_tick=0;
	/**************************Initialization Functions***************************/
	SystemInit();
	TIM2_Init();
	GPIO_SetBit(GPIOA,PIN11);

	Uart2_Init();
	GPIO_Config();
	LCD_Init ();
	/*****************************************************************************/
	LCD_String_xy (0, 0,">>>FOTA  V1.2<<<");
	LCD_String_xy (1, 0,"LED Blink 0.2Sec");
	TIM2_Delay_ms(2000);
	// 4. Enable USART2 interrupt
	NVIC_EnableIRQ(USART2_IRQn);
	//SysTick_Config(8000000/1000);
	//start_tick = GetTick();
    /****************************************************************************/
	for(;;)
	{
		/*if( (GetTick()-start_tick)>=500 )
		{
			TOGGLE_BIT(GPIOA->ODR,PIN10);
			start_tick = GetTick();
		}*/
		GPIO_SetBit(GPIOA,PIN11);
		TIM2_Delay_ms(200);
		GPIO_ClrBit(GPIOA,PIN11);
		TIM2_Delay_ms(200);
		/**********************************************************/
		if(Update_Flag)
		{
			LCD_Clear();
			LCD_String_xy (0, 0,"Update Request");
			LCD_String_xy (1, 0,"Accept OR Reject!");
			if(!GET_BIT(GPIOA->IDR, PIN0))
			{
				LCD_Clear();
				LCD_String_xy (0, 0,"Request accepted"); Update_Flag=0;
				USART2->CR1 &= ~(1<<5); // Bit 5 RXNEIE: RXNE interrupt enable
				NVIC_DisableIRQ (USART2_IRQn);       // disable Interrupt
				TIM2_Delay_ms(3000);
				/***************Jump to Bootloader Section*************/
				__set_MSP(*((volatile uint32_t*) 0x08000000)); //Set the MSP
				//asm volatile("msr msp, %0" ::"r"(SRAM_END));
				SCB->VTOR = 0x08000000; //We relocate vector table to the sector 1
				/* We are now ready to jump to the Bootloader */
				uint32_t JumpAddress = *((volatile uint32_t*) (0x08000000 + 4));
				void (*reset_handler)(void) = (void*)JumpAddress;
				reset_handler();

			}else if(!GET_BIT(GPIOA->IDR, PIN1))
			{
				LCD_Clear();
				LCD_String_xy (0, 0,"Request rejected"); Update_Flag=0;
				UART2_SendChar (REJECT_UPDATE);
				TIM2_Delay_ms(3000);
				LCD_Clear();
				LCD_String_xy (0, 0,">>>FOTA  V1.2<<<");
				LCD_String_xy (1, 0,"LED Blink 0.2Sec");

			}
		}
		/**********************************************************/

	}
}

/***************************************************************/




