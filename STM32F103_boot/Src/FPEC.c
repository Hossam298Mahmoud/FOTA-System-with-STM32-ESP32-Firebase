/*
 * FPEC.c
 *
 *  Created on: Mar 25, 2025
 *      Author: Hossam
 */
/************************************************************************************/
#include <stdint.h>
#include <FPEC.h>
#include <gpio.h>

/*************************************************************************************/
void FPEC_Unlock()
{
	/*
	 * In this case, the FPEC can be unlocked by writing the correct sequence of keys into FLASH_KEYR.
	 * */

   // Check if the flash locked or not.
   if(FPEC->CR & FLASH_CR_LOCK)
   {
      //Perform unlock sequency
      FPEC->KEYR = FLASH_KEY1;
      FPEC->KEYR = FLASH_KEY2;
   }
}
/*************************************************************************************/
void FPEC_lock()
{
	/*
	 *The FPEC block and FLASH_CR register can be locked by the
	 	user’s software by writing the LOCK bit of the FLASH_CR register to 1.
     * */
	FPEC->CR |= FLASH_CR_LOCK;
}
/*************************************************************************************/
void FPEC_PageErase(uint16_t PageNumber)
{
	 // Check if the flash locked or not.
	FPEC_Unlock();
	/*
	 * A page of the Flash memory can be erased using the Page Erase feature of the FPEC.
	 * To erase a page, the procedure below should be followed:
	 * */
	//1-Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH_CR register
	while(FPEC->SR & FLASH_SR_BSY);
	//2-Set the PER bit in the FLASH_CR register
	FPEC->CR |= FLASH_CR_PER;
	//3-Program the FLASH_AR register to select a page to erase
	FPEC->AR=FLASH_BASE+ (uint32_t)(PageNumber*PAGE_SIZE);
	//4-Set the STRT bit in the FLASH_CR register
	FPEC->CR |= FLASH_CR_STRT;
	//5-Wait for the BSY bit to be reset
	while(FPEC->SR & FLASH_SR_BSY);
	//6-Read the erased page and verify

	/*
	 * EOP: End of operation
		Set by hardware when a Flash operation (programming / erase) is completed.
		Reset by writing a 1
	 * */
	FPEC->SR |=FLASH_SR_EOP;
	FPEC->CR &=~FLASH_CR_STRT;
	FPEC->CR &=~ FLASH_CR_PER;
	FPEC_lock();
}
/*************************************************************************/
void programflash(uint16_t *addr , uint16_t D)
{
	// Check if the flash locked or not.
	FPEC_Unlock();
	/*
	* The main Flash memory programming sequence in standard mode is as follows:
	*/
	//1- Check that no main Flash memory operation is ongoing by checking the BSY bit in the FLASH_SR register.
	while(FPEC->SR & FLASH_SR_BSY);
	//2- Set the PG bit in the FLASH_CR register.
	FPEC->CR |= FLASH_CR_PG ;
	//3- Perform the data write (half-word) at the desired address.
	*((volatile uint16_t*)addr) = D;
	//4- Wait for the BSY bit to be reset.
	while(FPEC->SR & FLASH_SR_BSY);
	//5- Read the programmed value and verify.

	/*
	 * EOP: End of operation
		Set by hardware when a Flash operation (programming / erase) is completed.
		Reset by writing a 1
	 * */
	FPEC->SR |=FLASH_SR_EOP;
	FPEC->CR &=~FLASH_CR_PG ;
	FPEC_lock();

}

void FPEC_PagesErase(uint16_t first_page_num,uint16_t num_of_pages)
{
	uint16_t last_page_num= first_page_num + num_of_pages;
	for(int i=first_page_num;i<=last_page_num;i++)
	{
		FPEC_PageErase(i);
	}
}






