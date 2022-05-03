#include "stm32f10x.h" 
#include "delay.h"
#include "adcx.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
 
__IO uint16_t ADC_ConvertedValue[Sample_Num][Channel_Num];
u16 AD_After_Filter[Channel_Num]; 

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

// 初始化 GPIO
void ADC_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE); //啟用ADC1和GPIOA時鐘
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 ; // 啟用PA0-PA2當做類比輸入 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模擬輸入引腳
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//輸入時不用設置速率
}

// 設定 ADC1 的工作模式為 MDA 模式
void ADC_Mode_Config(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  	//啟用DMA時鐘
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  	//啟用ADC1時鐘
	
	/* DMA channel1 configuration */
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);		//恢復 DMA1頻道初始值
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 	//ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//記憶體位址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				//外設為資料來源
	DMA_InitStructure.DMA_BufferSize = Sample_Num*Channel_Num;		//保存DMA要傳輸的資料總大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外設地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  		//記憶體位址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//設定外設資料寬度為半字16位元
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//設定記憶體資料寬度為半字16位元
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;					//DMA_Mode_Circular;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					//禁止記憶體到記憶體的傳輸
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE); //啟用 DMA channel1 
	
	/* ADC1 configuration */
	ADC_InitTypeDef ADC_InitStructure;	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//獨立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 	//掃描模式用於多通道採集
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//開啟連續轉換模式，即不停地進行ADC轉換
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部觸發轉換
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//採集資料右對齊
	ADC_InitStructure.ADC_NbrOfChannel = 3;	 			//要轉換的通道數目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//設定ADC時鐘，為PCLK2的8分頻，即9MHz
	
	/*設定ADC1的通道0-2為239.5個採樣週期，序列為1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );	
	
	ADC_DMACmd(ADC1, ENABLE);	//啟動 ADC1ADC1 DMA
	ADC_Cmd(ADC1, ENABLE);		//啟動 ADC1
	
	ADC_ResetCalibration(ADC1);					//復位校準暫存器   
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待校準暫存器復位完成
	ADC_StartCalibration(ADC1);					//ADC校準 
	while(ADC_GetCalibrationStatus(ADC1));		//等待校準完成
	 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// 由於沒有採用外部觸發，所以使用軟體觸發ADC轉換
}


u16 Read_ADC_AverageValue(u16 Channel)
{
	u8 t;
	u32 sum = 0;
	
	//完成一次DMA傳輸，資料大小10*5	
	DMA_SetCurrDataCounter(DMA1_Channel1,Sample_Num*Channel_Num);				//設置DMA的傳送數量為10*5
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//使用軟體轉換啟動功能	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=SET);			//等待DMA傳送完成
	DMA_ClearFlag(DMA1_FLAG_TC1);							//清除DMA傳送完成標誌
	DMA_Cmd(DMA1_Channel1,DISABLE);	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);	

	for(t=0;t<Sample_Num;t++)
		{
			sum += ADC_ConvertedValue[t][Channel];
		}
	AD_After_Filter[Channel] = sum/Sample_Num;
	sum = 0;
	return AD_After_Filter[Channel];
	//delay_ms(5);
}

