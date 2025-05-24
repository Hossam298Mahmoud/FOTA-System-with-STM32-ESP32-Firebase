/*-----------------------GPIO Driver Interface--------------------------*/
				
				/*&&&&&&&&&&&&&%%%%%%%%%%%%%%%&&&&&&&&&&&&&*/

/*---------------Include Headers Files-------------------------*/				
#include "gpio.h"
/*-------------------------------------------------------------*/

/*----------------GPIO_Init------------------*/
void GPIO_Init(GPIO_TypeDef* GPIOx, char Pin, char  Mode)
{
	if(Pin<=7)
	{
		GPIOx->CRL &= ~(0b1111<<(Pin*4));
		GPIOx->CRL |= (Mode<<(Pin*4));
	}
	else if(Pin<=15)
	{
		Pin -=8;
		GPIOx->CRH &= ~(0b1111<<(Pin*4));
		GPIOx->CRH |= (Mode<<(Pin*4));
	}
	else
	{
		//error
	}
}

void GPIO_SetBit(GPIO_TypeDef* GPIOx, char Pin)
{
	GPIOx->ODR |= 1<<Pin;
}
void GPIO_ClrBit(GPIO_TypeDef* GPIOx, char Pin)
{
	GPIOx->ODR &= ~(1<<Pin);
}
char GPIO_GetBit(GPIO_TypeDef* GPIOx, char Pin)
{
	return GET_BIT(GPIOx->IDR,Pin);
}
/*------------------------------------------------------------*/



