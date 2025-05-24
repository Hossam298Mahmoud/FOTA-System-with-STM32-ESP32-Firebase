/*
 * LCD.c
 *
 * Created: 9/12/2023 10:09:07 PM
 *  Author: Hossam Mahmoud
 */ 
#include <lcd.h>
#include <stdint.h>

void _delay_us(uint8_t us)
{
	while(us)
	{
	us--;
	}
}
void _delay_ms(uint8_t ms)
{
	TIM2_Delay_ms(ms);
}
/****************** FUNCTION TO SEND A COMMAND TO LCD *************/

void LCD_Command(unsigned char cmd)
{
	LCD_Data_Port=((LCD_Data_Port & 0x0FFF) | ((cmd & 0x00F0 )<<8));/* Sending upper nibble */

	LCD_Command_Port  &= ~(1<<RS);	/* RS=0 command reg. */
	//LCD_Command_Port  &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port  |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port  &= ~(1<<EN);
	_delay_ms(2); //Wait
	LCD_Data_Port=((LCD_Data_Port & 0x0FFF) | ((cmd & 0x000F)<<12));/* Sending lower nibble */
	LCD_Command_Port  |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port  &= ~(1<<EN);
	_delay_ms(5);
}

/****************** FUNCTION TO INITIALIZE THE LCD *************/
void LCD_Init (void)	/* LCD Initialize function */
{
	_delay_ms(20);		/* LCD Power ON delay always >15ms */
	LCD_Command_Port  &= ~(1<<EN);
	_delay_us(2);        //Wait some time to do its Operation
	LCD_Command(0x33);
	LCD_Command(0x32);
	LCD_Command (0x28);	/* Initialization of 16X2 LCD in 4bit mode */
	LCD_Command (0x01);	/* clear display */
	LCD_Command (0x02);	/* cursor at home position */
	LCD_Command (0x0E);	/* Display ON Cursor OFF */
	LCD_Command (0x06);	/* Auto Increment cursor */
	
}

/****************** FUNCTION TO WRITE A CHARACHTER ON LCD *************/

void LCD_Char (unsigned char char_data)	/* LCD data write function */
{

 	LCD_Data_Port=((LCD_Data_Port & 0x0FFF) | ((char_data & 0x00F0)<<8));/* Sending upper nibble */

	LCD_Command_Port  |= (1<<RS);	/* RS=1 Data reg. */
	//LCD_Command_Port  &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port  |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port  &= ~(1<<EN);
	_delay_ms(1); //Wait

	LCD_Data_Port=((LCD_Data_Port & 0x0FFF) | ((char_data &0x000F)<<12));/* Sending lower nibble */

	LCD_Command_Port  |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port  &= ~(1<<EN);
	_delay_ms(5);
}

/****************** FUNCTION TO WRITE A STRING ON LCD *************/

void LCD_String (char *str)
{
	uint8_t i;
	for(i=0;str[i]!=0;i++)  /* send each char of string till the NULL */
	{
		LCD_Char (str[i]);  /* call LCD data write */
	}
}

/****************** FUNCTION TO Clear LCD *************/
void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
	_delay_ms(5); //Wait

}
/****************** Send string to LCD with xy position *************/

void LCD_String_xy (char row, char pos, char *str)   /* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}
