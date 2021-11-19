#include "gpio.h"

void GPIO_Config(void)
{
  GPIO_DeInit(GPIOD);
  GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);  // modbus enable
  GPIO_WriteLow(GPIOD, GPIO_PIN_4);
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);   // analog input LDR
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);   // analog input TEMP
}


