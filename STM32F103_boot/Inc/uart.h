/*
 * uart.h
 *
 *  Created on: Aug 22, 2024
 *      Author: HossamMahmoud
 */

#ifndef UART_H_
#define UART_H_

/*-------------------------- USART registers--------------------------*/
#include <stdint.h>
#include "stm32f103xb.h"

/*---------------------------------------------------------------------------*/
void Uart2_Init (void);

void Uart2_SendChar (uint8_t c);

void Uart2_SendString (char *string);

uint8_t Uart2_GetChar (void);

uint8_t HAL_UART_Receive(char *UartBuffer, uint8_t len, uint32_t time_out);

#endif /* UART_H_ */
