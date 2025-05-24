/*
 * uart.c
 *
 *  Created on: Aug 22, 2024
 *      Author: HossamMahmoud
 */
#include <gpio.h>
#include "uart.h"
#include "tim.h"


void Uart2_Init (void)
{
		/******* STEPS FOLLOWED ********

	1. Enable the UART CLOCK and GPIO CLOCK
	2. Configure the UART PINs for ALternate Functions
	3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	4. Program the M bit in USART_CR1 to define the word length.
	5. Select the desired baud rate using the USART_BRR register.
	6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register

	********************************/

	// 1. Enable the UART2 CLOCK
	RCC->APB1ENR |= (1<<17);  // Enable UART2 CLOCK

	// 2. Configure the UART PINs for ALternate Functions
	GPIO_Init(GPIOA, PIN2, GPIO_OUTPUT_AF_PP);  //Alternate Function for Pin PA2 As TX
	GPIO_Init(GPIOA, PIN3, GPIO_INPUT_PU_PD);  //Alternate Function for Pin PA3 As RX
	GPIOA->ODR |= 1<<PIN3; // Pull Up for PA3
	// 3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART2->CR1 = 0x00;  // clear all

	// 4. Program the M bit in USART_CR1 to define the word length.
	USART2->CR1 &= ~(1<<12);  // M =0; 8 bit word length

	// 5. Select the desired baud rate using the USART_BRR register.
	USART2->BRR = (1<<0) | (52<<4);   // Baud rate of 19200, HSI at 1MHz

	/*Bit 7 DMAT: DMA enable transmitter
				  This bit is set/reset by software
				  1: DMA mode is enabled for transmission.
				  0: DMA mode is disabled for transmission.*/

	/*Bit 6 DMAR: DMA enable receiver
				This bit is set/reset by software
				1: DMA mode is enabled for reception
				0: DMA mode is disabled for reception*/
	//USART2->CR3 |= (1<<6);  // Enable DMA for Receive

	//USART2->CR1 |= (1<<5); // Bit 5 RXNEIE: RXNE interrupt enable

	// 6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter

	USART2->CR1 |= (1<<13);  // UE = 1... Enable USART

}


void UART2_SendChar (uint8_t c)
{
		/*********** STEPS FOLLOWED *************

	1. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
		 for each data to be transmitted in case of single buffer.
	2. After writing the last data into the USART_DR register, wait until TC=1. This indicates
		 that the transmission of the last frame is complete. This is required for instance when
		 the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.

	****************************************/
	while (!(USART2->SR & (1<<7))); // Wait until TXE (Transmit Data Register Empty) is set
	USART2->DR = c; // load the data into DR register
	while (!(USART2->SR & (1<<6)));  // Wait for TC to SET.. This indicates that the data has been transmitted
}

void UART2_SendString (char *string)
{
	while (*string) UART2_SendChar (*string++);
}

uint8_t UART2_GetChar (void)
{
			/*********** STEPS FOLLOWED *************

	1. Wait for the RXNE bit to set. It indicates that the data has been received and can be read.
	2. Read the data from USART_DR  Register. This also clears the RXNE bit

	****************************************/

	uint8_t temp;

	while (!(USART2->SR & (1<<5)));  // wait for RXNE bit to set
	temp = USART2->DR;  // Read the data. This clears the RXNE also
	return temp;
}


uint8_t HAL_UART_Receive(char *UartBuffer, uint8_t len, uint32_t time_out)
{
	uint8_t status=0 ,i=0;
    uint32_t start_tick = GetTick();
	while(1)
	{
		if( (GetTick()-start_tick)>= time_out )//time out
		{
    		status=0;
    		break;
		}
		else if((USART2->SR & (1<<5)))
		{
			UartBuffer[i]=USART2->DR;
			if(i==(len-1))
			{
	    		status=1;
				break;
			}
			i++;
		}

	}
	return status;
}


