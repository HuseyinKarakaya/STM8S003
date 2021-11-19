/**
******************************************************************************
* @file     TIM4_TimeBase\main.c
* @author   MCD Application Team
* @version V2.0.4
* @date     26-April-2018
* @brief    This file contains the main function for TIM4 Time Base Configuration example.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "clk.h"
#include "modbus.h"
#include "ai_read.h"
#include "gpio.h" 
#include "flash.h"



/**
* @addtogroup TIM4_TimeBase
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    extern uint8_t Address;	//Device Address 
    extern uint16_t Output_Registers[20]; //modbusdan gönderilecek bilgiler,
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
* @brief  Main program.
* @param  None
* @retval None
*/
void main(void)
{
  /* Clock configuration -----------------------------------------*/
  CLK_Config();  
  /* Flash_Setup configuration ---------------------------------*/
  Flash_Setup();
  /* UART1 configuration -----------------------------------------*/
  UART1_Config();  
  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();   
  /* TIM2 configuration ----------------------------------------*/
  TIM2_Config();
  /* TIM4 configuration ----------------------------------------*/
  TIM4_Config();
  /* SetDevAddr configuration --------------------------------*/
  //SetDevAddr(1);
  /* ADC1_setup configuration ---------------------------------*/
  ADC1_setup();
  while (1)  {
  ADC_Output();
  }
}


#ifdef USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @}
*/

 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
