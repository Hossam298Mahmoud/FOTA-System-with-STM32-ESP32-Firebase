/*
 * gpio.h
 *
 *  Created on: May 1, 2025
 *      Author: Hossam
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f103xb.h"

/*------------Macros for Bit Manipulation in C/C++-------------*/
#define SET_BIT(x, pos) (x |= (1U << pos))     //Macro to set nth-bit
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ORing (|) x with the mask.
*/

#define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))     //Macro to clear nth-bit
/*
Set single bit at pos to '0' by generating a mask
in the proper bit location and Anding x with the mask.
*/

#define TOGGLE_BIT(x, pos)   x ^= (1U<< pos)    //Macro to toggle nth-bit
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and ex-ORing x with the mask.
*/

#define CHECK_BIT(x, pos) (x & (1U << pos) )   //Macro to check nth-bit
/*
Set single bit at pos to '1' by generating a mask
in the proper bit location and Anding x with the mask.
It evaluates 1 if a bit is set otherwise 0.
*/

#define GET_BIT(x, pos)  ((x & ( 1 << pos)) >> pos)     /*Macro to Get bit from the given position*/
/*------------------------------------------------------------------*/

#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10   10
#define PIN11   11
#define PIN12   12
#define PIN13   13
#define PIN14   14
#define PIN15   15

/*---------------------------Pin Modes Of GPIO------------------------*/

/*----------output mode------------Description--------------------*/
/*
00: Input mode (reset state)
01: Output mode, max speed 10 MHz.
10: Output mode, max speed 2 MHz.
11: Output mode, max speed 50 MHz.

 In output mode (MODE[1:0] >00):
00: General purpose output push-pull
01: General purpose output Open-drain
10: Alternate function output Push-pull
11: Alternate function output Open-drain
 */
//01: Output mode, max speed 10 MHz.
#define GPIO_OUTPUT_10MHz_PP   0b0001    //General purpose output push-pull
#define GPIO_OUTPUT_10MHz_OD   0b0101    //General purpose output Open-drain
#define GPIO_OUTPUT_AF_PP      0b1001    //Alternate function output Push-pull
#define GPIO_OUTPUT_AF_OD      0b1101    //Alternate function output Open-drain
/*-------------------------------------------------------------*/
			/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/


/*----------input mode------------Description--------------------*/
/*
00: Input mode (reset state)
01: Output mode, max speed 10 MHz.
10: Output mode, max speed 2 MHz.
11: Output mode, max speed 50 MHz.

In input mode (MODE[1:0]=00):
00: Analog mode
01: Floating input (reset state)
10: Input with pull-up / pull-down
11: Reserved
*/
//00: Input mode (reset state)
#define GPIO_INPUT_AN      0b0000    //Analog Input mode
#define GPIO_INPUT_Float   0b0100    //Floating input mode
#define GPIO_INPUT_PU_PD   0b1000    //Input with pull-up / pull-down
/*-------------------------------------------------------------*/
/**************************************************************************/

/*----------------GPIO functions------------------*/
void GPIO_Init(GPIO_TypeDef* GPIOx, char Pin, char  Mode);
void GPIO_SetBit(GPIO_TypeDef* GPIOx, char Pin);
void GPIO_ClrBit(GPIO_TypeDef* GPIOx, char Pin);
char GPIO_GetBit(GPIO_TypeDef* GPIOx, char Pin);

#endif /* GPIO_H_ */
