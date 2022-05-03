#ifndef __ADCX_H
#define __ADCX_H
#include "stm32f10x.h"
 
#define Sample_Num 10
#define Channel_Num 3

extern __IO uint16_t ADC_ConvertedValue[Sample_Num][Channel_Num];
extern u16 AD_After_Filter[Channel_Num];

void ADCX_Init(void);
uint16_t ADCX_GetValue(u8 ch);

u16 Get_Temp(void);
u16 Get_Adc_Average(u8 ch,u8 times);
short Get_Temprate(void);

void ADC_GPIO_Config(void);	
void ADC_Mode_Config(void); 
	
u16 Read_ADC_AverageValue(u16 Channel);

#endif
