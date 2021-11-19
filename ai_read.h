#ifndef __AI_READ_H
#define __AI_READ_H

/* Includes -----------------------------------------------*/
#include "stm8s.h"
#include "math.h"

void ADC1_setup(void);
void ADC_read(void);


double Thermistor(double analogValue);
void ADC_Output(void);

#endif
