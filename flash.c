#include "flash.h"

extern uint16_t Output_Registers[20]; //modbusdan gönderilecek bilgiler,

void Flash_Setup(void)

{
  FLASH_DeInit();
  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  // ilk açilista birkez çalisacak.
  uint8_t Default = FLASH_ReadByte(0x4030);
  if (Default != 22)
  {
    FLASH_ProgramByte(0x4000, 0);//cihaz NO
    FLASH_ProgramByte(0x4001, 99);//cihaz NO
    FLASH_ProgramByte(0x4002, 0);//Modbus ID
    FLASH_ProgramByte(0x4003, 1);//Modbus ID  
    FLASH_ProgramByte(0x4004, 0);//Modbus UART1_BAUDRATE    
    FLASH_ProgramByte(0x4005, 2);//Modbus UART1_BAUDRATE     1: 9600, 2: 19200
    FLASH_ProgramByte(0x4006, 0);//Modbus UART1_BIT-PARITY-STOP  
    FLASH_ProgramByte(0x4007, 1);//Modbus UART1_BIT-PARITY-STOP   1:8N1 2:8N2 3:8E1 4:8O1 
    FLASH_ProgramByte(0x4008, 0);//Modbus                    
    FLASH_ProgramByte(0x4009, 0);//Modbus               
    FLASH_ProgramByte(0x400A, 0);//Modbus      
    FLASH_ProgramByte(0x400B, 0);//Modbus      
    FLASH_ProgramByte(0x400C, 0);//Sensor             
    FLASH_ProgramByte(0x400D, 100);//Sensor OFFSET           0:-10C   100:0C   200:+10C
    FLASH_ProgramByte(0x400E, 0);//Sensor RESPONSE           1: 1sn/10örneklem
    FLASH_ProgramByte(0x400F, 1);//Sensor RESPONSE           1: 1sn/10örneklem 
    //FLASH_ProgramByte(0x4010, 20);// TEMP X10        
    //FLASH_ProgramByte(0x4011, 20);// TEMP X10
    //FLASH_ProgramByte(0x4012, 20);// TEMP X100
    //FLASH_ProgramByte(0x4013, 20);// TEMP X100
    //FLASH_ProgramByte(0x4014, 20);// LIGTH 
    //FLASH_ProgramByte(0x4015, 20);// LIGTH
    //FLASH_ProgramByte(0x4016, 20);// BOS 1
    //FLASH_ProgramByte(0x4017, 20);// BOS 1
    FLASH_ProgramByte(0x4030, 22);//ilk çalistirma
  } 
  Output_Registers[0] = ReadFLASH(0x4000);//cihaz NO
  Output_Registers[1] = ReadFLASH(0x4002);//Modbus ID
  Output_Registers[2] = ReadFLASH(0x4004);//Modbus UART1_BAUDRATE     1: 9600, 2: 19200
  Output_Registers[3] = ReadFLASH(0x4006);//Modbus UART1_BIT-PARITY-STOP   1:8N1 2:8N2 3:8E1 4:8O1
  Output_Registers[4] = ReadFLASH(0x4008);//BOS  
  Output_Registers[5] = ReadFLASH(0x400A);//BOS 
  Output_Registers[6] = ReadFLASH(0x400C);//Sensor OFFSET
  Output_Registers[7] = ReadFLASH(0x400E);//Sensor RESPONSE           1: 1sn/20örneklem
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void WriteFLASH(uint8_t adr, uint8_t number)
{ 
  FLASH_ProgramByte(adr, number >> 8);       //H
  FLASH_ProgramByte(adr + 1, number & 0xFF); //L
}

uint16_t ReadFLASH(uint16_t adr)
{
  uint8_t byte1 = FLASH_ReadByte(adr);      //H
  uint8_t byte2 = FLASH_ReadByte(adr + 1);  //L
  return ((byte1 << 8) + byte2);
}
