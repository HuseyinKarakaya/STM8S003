#include "ai_read.h"
uint16_t ADC_value_3;
uint16_t ADC_value_4;
uint16_t Response[10]; //örneklem
uint8_t r = 0;
uint16_t TEMPavg;
uint16_t A0;
uint16_t Aort = 0;
uint16_t A1;
uint32_t sum;
uint32_t res1;
uint32_t res2;
double temperature;
double Temp;
uint16_t TIM4_Delay;

extern uint16_t Output_Registers[20]; //modbusdan gönderilecek bilgiler,

void ADC1_setup(void)
{
  
  ADC1_DeInit();         
  
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, 
            ADC1_CHANNEL_4,
            ADC1_PRESSEL_FCPU_D18, 
            ADC1_EXTTRIG_GPIO, 
            DISABLE, 
            ADC1_ALIGN_RIGHT, 
            ADC1_SCHMITTTRIG_CHANNEL0, 
            DISABLE);
  ADC1_Cmd(ENABLE);
}

void ADC_read(void)
{
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == FALSE);                       
  A0 =  ADC1_GetConversionValue();
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  //Output_Registers[12] = A0;      // ortalama deger
}

void ADC_Output(void)
{ 
  
   if(TIM4_Delay > 100*(Output_Registers[7]))      // 1 s reponse
    {
      if( r > 9)  {
        r = 0; 
      }
      ADC_read();
      Response[r] =  A0;
      Aort = (Response[0]+Response[1]+Response[2]+Response[3]+Response[4]+Response[5]+Response[6]+Response[7]+Response[8]+Response[9])/10;
      r++;
      TIM4_Delay = 0;
    }
  
  Output_Registers[9] = ((uint16_t)(Thermistor(A0)*100) - 1000 + 10*Output_Registers[6]); //TEMP X 100
  Output_Registers[8] = Output_Registers[9]/10; // TEMP X 10

} 

double Thermistor(double analogValue){
  // double temperature;
  res1 = analogValue * 10000;
  res2 = res1 / (1024 - analogValue); 
  //analogValue
  temperature = log(res2);// 1024 ADC çözünürlügü(10 bit)
// temperature = 1 / (0.00112 + (0.0002373 + (0.0000000711 * temperature * temperature)) * temperature);  // 5c, 25c, 45c    
  temperature = 1 / (0.001143615431 + (0.0002310824443 + (0.0000001075998838 * temperature * temperature)) * temperature); // 90c, 75c, 50c       
  temperature = temperature - 273.15;
  return temperature;
}