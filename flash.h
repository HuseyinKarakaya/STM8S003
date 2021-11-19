#ifndef __FLASH_H
#define __FLASH_H


/* Includes -----------------------------------------------*/
#include "stm8s.h"

void Flash_Setup(void);
void WriteFLASH(uint8_t address, uint8_t number);
uint16_t ReadFLASH(uint16_t address);

#endif
