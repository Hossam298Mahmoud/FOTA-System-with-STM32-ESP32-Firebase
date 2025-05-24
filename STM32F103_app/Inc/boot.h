/*
 * boot.h
 *
 *  Created on: Mar 26, 2025
 *      Author: Hossam
 */

#ifndef BOOT_H_
#define BOOT_H_

/**************************************************************************************/
#define APP_START_ADDRESS 0x08001000 //Page 4 0x0800 1000 - 0x0800 13FF 1Kbyte
extern uint32_t _estack ;  /* end of "RAM" Ram type memory */
#define SRAM_BASE   0x20000000
#define SRAM_SIZE	96*1024
#define SRAM_END    (SRAM_BASE+SRAM_SIZE)

/**************************************************************************************/

void write_program_pages_fota(uint32_t First_Page_Address,uint16_t LEN);


#endif /* BOOT_H_ */
