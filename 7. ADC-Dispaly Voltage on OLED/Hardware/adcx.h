#ifndef __ADCX_H
#define __ADCX_H

void ADCX_Init(void);
uint16_t ADCX_GetValue(u8 ch);

u16 Get_Temp(void);
u16 Get_Adc_Average(u8 ch,u8 times);
short Get_Temprate(void);


#endif
