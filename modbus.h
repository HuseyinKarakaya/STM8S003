#ifndef __MODBUS_HEADER_H
#define __MODBUS_HEADER_H

/* Includes -----------------------------------------------*/
#include "stm8s.h"

/* Private defines -----------------------------------------------------------*/
#define TIM4_PERIOD       124
#define TOTAL_REG_SIZE 10
#define OFS 25   // Output_Frame SIZE
#define REGSIZE 10	//Number of Output and Input registers
#define BufforsSize 45  //Size of Temporary and modbus frame buffor must be the same
//#define HREGSIZE     50
/* If you want redefine this value you should 
change this value in _it file too
*/

//Error defines 
#define ILLEGAL_FUNCTION 01
#define ILLEGAL_DATA_ADDR 02
#define ILLEGAL_DATA_VAL 03
//uint16_t Output_Registers[REGSIZE];

/* Private function prototypes -----------------------------------------------*/
void UART1_Config(void);
void TIM2_Config(void);
void TIM4_Config(void);

//void TimingDelay_Decrement(void);
//void Delay(__IO uint32_t nTime);
void SetDevAddr(uint8_t Addr);
void UART1Send(void);

/* Modbus handler function prototypes ------------------------------------------*/
void ReadHoldingRegisters(void);
void WriteSingleRegisters(void);
uint8_t __checkAddr(uint8_t address);
void calculateCRC(uint8_t frameSize);
/* Errors and check ------------------------------------------*/


/*
*****************************************
INPUT and OUTPUT registers for user data
*****************************************
*/

#endif
