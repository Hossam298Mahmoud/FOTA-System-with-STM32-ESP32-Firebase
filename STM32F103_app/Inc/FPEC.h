/*
 * FPEC.h
 *
 *  Created on: Mar 25, 2025
 *      Author: Hossam
 */

#ifndef FPEC_H_
#define FPEC_H_

#include "stm32f103xb.h"

/*********************************************************************************/
#define FLASH_BASE	0x08000000
#define FPEC_BASE	0x40022000
#define FPEC		((volatile FLASH_TypeDef *)FPEC_BASE)
#define PAGE_SIZE   1024

/*********************************************************************************/
void FPEC_Unlock();
void FPEC_PagesErase(uint16_t first_page_num,uint16_t num_of_pages);

/*********************************************************************************/

#endif /* FPEC_H_ */
