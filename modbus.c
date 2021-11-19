#include "modbus.h"

__IO uint32_t TimingDelay = 0;
uint8_t Frame[45] = 0;    //Gönderilecek bilgiler burada toplaniyor
uint8_t FirstReg = 0;
uint8_t NumberOfRegs = 0;
uint8_t framecount;
uint16_t crc;
uint8_t sendSize;
uint8_t crccount;
uint16_t Output_Registers[20]; //modbusdan gönderilecek bilgiler,
uint16_t BAUDRATE;
uint8_t WORDLENGTH;
uint8_t PARITY;
uint8_t STOPBITS;
uint8_t Address;				//Device Address 
uint8_t mbflag=0;				//MB guncelleme flag 
extern uint8_t word[45];

uint16_t T1_5 = 0;
uint16_t T3_5 = 0;

extern void WriteFLASH(uint8_t address, uint8_t number);
/**
* @brief  Configure system clock to run at 16Mhz
* @param  None
* @retval None
*/

void UART1_Config(void)
{
  /* EVAL COM (UART) configuration -----------------------------------------*/
  /* USART configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - Odd parity
  - Receive and transmit enabled
  - UART Clock disabled
  */
  Address = Output_Registers[1];   // modbus ID guncelle
  switch(Output_Registers[2]){    
  case 1:    
    BAUDRATE = 9600; 
    T1_5 = (1000000/BAUDRATE)*15;
    T3_5 = (1000000/BAUDRATE)*35;
    break;  //istege bagli kullanilir.  
  case 2:    
    BAUDRATE = 19200; 
    T1_5 = (1000000/BAUDRATE)*15;
    T3_5 = (1000000/BAUDRATE)*35;
    break;  //istege bagli kullanilir.  
//  default:     
//    BAUDRATE = 9600; 
//    T1_5 = (1000000/BAUDRATE)*15;
//    T3_5 = (1000000/BAUDRATE)*35;
  } 
  switch(Output_Registers[3]){  
  case 1:    // 8 NONE 1
    WORDLENGTH = 0x00;  
    PARITY = 0x00;  
    STOPBITS = 0x00;  
    break;  
  case 2:    // 8 NONE 2
    WORDLENGTH = 0x00;  
    PARITY = 0x00;  
    STOPBITS = 0x20;   
    break;     
  case 3:    // 8 EVEN 1
    WORDLENGTH = 0x00;  
    PARITY = 0x04;    
    STOPBITS = 0x00;  
    break;    
  case 4:    // 8 ODD 1
    WORDLENGTH = 0x00;  
    PARITY = 0x06;    
    STOPBITS = 0x00;  
    break;    
  default:   
  }  
  
  UART1_Init(BAUDRATE, WORDLENGTH, STOPBITS, PARITY,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  //UART1_Init(BAUDRATE, UART1_WORDLENGTH_8D,UART1_STOPBITS_2, UART1_PARITY_NO,
  //            UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);  
  
  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
  receive data register is not empty */
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
  when the UART transmit Shift Register is empty */
  //UART1_ITConfig(UART1_IT_TXE, ENABLE);
  
  /* Enable UART */
  UART1_Cmd(ENABLE);
  
  /* Enable general interrupts */
  enableInterrupts();
}

/**
* @brief  Configure TIM4 to generate an update interrupt each 1ms 
* @param  None
* @retval None
*/
void TIM2_Config(void)
{
  /* TIM4 configuration:
  - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
  clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
  max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
  min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
  so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */
  
  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 65000); //
  /* Clear TIM4 update flag */
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  //enableInterrupts();
  
  /* Enable TIM4 */
  TIM2_Cmd(ENABLE);
}

/**
* @brief  Configure TIM4 to generate an update interrupt each 1ms 
* @param  None
* @retval None
*/
void TIM4_Config(void)
{
  /* TIM4 configuration:
  - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
  clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
  max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
  min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
  so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */
  
  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  // enableInterrupts();
  
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

///FC03  *This command is requests the content of analog output holding registers///
/// 			*registers => output or input
void ReadHoldingRegisters(void)
{
  Frame[0]= word[0];       // adres
  Frame[1]= word[1];       // fonksiyon
  Frame[2]= word[5]*2;     // kaç byte gönderilecek
  
  FirstReg = word[3];  
  NumberOfRegs = word[5];  
  
  framecount=2;
  
  for(uint8_t i = FirstReg; i<(FirstReg+NumberOfRegs); i++)
  {
    framecount++;
    Frame[framecount] = (Output_Registers[i] >> 8) & 0xFF;  // high
    framecount++;
    Frame[framecount] = (Output_Registers[i] & 0xFF);       // low
    crccount = framecount + 1;
  }
  
  calculateCRC(crccount);  
  
  framecount++;
  Frame[framecount] = crc >>8;;
  framecount++;
  Frame[framecount] = crc & 0xFF;
  sendSize = framecount+2;
  
  UART1Send();
}


///FC06  *This command is requests the content of analog output holding registers///
/// 			*registers => output or input
void WriteSingleRegisters(void)
{
  
  Frame[0]= word[0];       // adres
  Frame[1]= word[1];       // fonksiyon
  Frame[2]= word[2];       // register HIGH
  Frame[3]= word[3];       // register LOW
  Frame[4]= word[4];       // data HIGH
  Frame[5]= word[5];       // data LOW
  
  uint16_t receivedata = word[5] | (word[4] << 8); // Gelen datalari birlestirme
 
  
  switch(word[3]){    
  case 1:    // Modbus ID guncelle
    if(receivedata >= 1 && receivedata <= 254)  {
      Output_Registers[1] = receivedata;         // register içerisine yazma
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      FLASH_ProgramByte( 0x4003, word[5]);
      FLASH_Lock(FLASH_MEMTYPE_DATA);
      mbflag=1;
    }
    break;
  case 2:        // Modbus BAUDRATE guncelle
    if(receivedata >= 1 && receivedata <= 2)  {
      Output_Registers[2] = receivedata;         // register içerisine yazma
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      FLASH_ProgramByte( 0x4005, word[5]);
      FLASH_Lock(FLASH_MEMTYPE_DATA);
      mbflag=1;
    }
    break; 
  case 3:        // Modbus BIT-PARITY-STOP guncelle
    if(receivedata >= 1 && receivedata <= 2)  {
      Output_Registers[3] = receivedata;         // register içerisine yazma
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      FLASH_ProgramByte( 0x4007, word[5]);
      FLASH_Lock(FLASH_MEMTYPE_DATA);
      mbflag=1;
    }
    break; 
  case 6:        // Modbus OFFSET guncelle
    if(receivedata >= 0 && receivedata <= 200)  {
      Output_Registers[6] = receivedata;         // register içerisine yazma
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      FLASH_ProgramByte( 0x400D, word[5]);
      FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    break; 
  case 7:        // Modbus RESPONSE guncelle
    if(receivedata >= 1 && receivedata <= 10)  {
      Output_Registers[7] = receivedata;         // register içerisine yazma
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      FLASH_ProgramByte( 0x400F, word[5]);
      FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    break; 
  default:     
    // illegal data
  } 

  
  // Output_Registers[word[3]] = receivedata;         // register içerisine yazma
  
  
  //  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  //  FLASH_ProgramByte( 0x4000 + 2*word[3], word[4]);
  //  FLASH_ProgramByte( 0x4000 + (2*word[3] + 1), word[5]);
  //  FLASH_Lock(FLASH_MEMTYPE_DATA);
  
  calculateCRC(6);  
  
  Frame[6] = crc >>8;
  Frame[7] = crc & 0xFF;
  sendSize = 9;                        // bu 9 du sonra ben bunu 8 yaptim!!!!!!!!!!!
  
  UART1Send();
  
  if(mbflag == 1)  {
    UART1_Config();     // Uart refresh
    mbflag=0;
  }
}

void calculateCRC(uint8_t frameSize) 
{
  uint8_t crc2, flag; //
  crc = 0xFFFF;
  for (uint8_t i = 0; i < frameSize; i++)
  {
    crc = crc ^ Frame[i];
    for (uint8_t j = 1; j <= 8; j++)    {
      flag = crc & 0x0001;
      crc >>= 1;
      if (flag)
        crc ^= 0xA001;
    }
  }
  // Reverse byte order. 
  crc2 = crc >> 8;
  crc = (crc << 8) | crc2;
  crc &= 0xFFFF; 
  // the returned value is already swapped
  // crcLo byte is first & crcHi byte is last
  //return temp; 
}

void UART1Send(void)
{
  //Set Max485 in Transmitter mode
  
  GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
  
  for (uint8_t v = 0; v < sendSize; v++)     // Loop over each bit  
  {  
    UART1_SendData8(Frame[v]);
    while ((UART1->SR & UART1_SR_TXE ) != UART1_SR_TXE ); 
  }
  //Set Max485 in Receiver mode
  GPIO_WriteLow(GPIOD, GPIO_PIN_4);
}
