#include "stm32f10x.h" 
#include "delay.h"

void ADCX_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);  //重設 ADC1
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
		
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t ADCX_GetValue(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5);

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

//取得 ADC 採樣內部溫度感測器的值, 取10次,然後平均
u16 Get_Temp(void)
{
    u16 temp_val=0;
    u8 t;
    for(t=0;t<10;t++)
    {
        temp_val += ADCX_GetValue(ADC_Channel_16);      //TampSensor		
        delay_ms(5);
    }
    return temp_val/10;
}


//獲取通道ch的轉換值, 取times次平均
u16 Get_Adc_Average(u8 ch,u8 times)
{
    u32 temp_val=0;
    u8 t;
    for(t=0;t<times;t++)
    {
        temp_val += ADCX_GetValue(ADC_Channel_16);
        delay_ms(5);
    }
    return temp_val/times;
}      

//讀取內部溫度感測器溫度值
//返回值:溫度值(擴大了100倍,單位:℃.)
short Get_Temprate(void)
{
    u32 adcx;
    short result;
    double temperate;
    adcx = Get_Adc_Average(ADC_Channel_16,20);  //讀取通道16,20次取平均
    temperate = (float)adcx*(3.3/4096);       //電壓值 
    temperate = (1.43-temperate)/0.0043+25;   //轉換為溫度值     
    result = temperate *= 100;                  //擴大100倍.
    return result;
}
