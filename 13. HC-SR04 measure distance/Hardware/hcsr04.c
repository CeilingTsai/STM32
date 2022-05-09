#include "stm32f10x.h"
#include "hcsr04.h"
#include "delay.h"

u16 msHcCount = 0;	//ms計數

void HCSR04_Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //定義計時器結構體
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
     
    //IO 初始化
    GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;       	//發送電位引腳 Trig
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽輸出
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;    	 //返回電位引腳 Echo
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);	
	 
	//計時器初始化 使用基本計時器TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //啟動TIM3 RCC時鐘

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 65535; //(1000-1); //設置在下一個更新事件裝入活動的自動重裝載寄存器週期的值         計數到1000為1ms
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //設置用來作為TIMx時鐘頻率除數的預分頻值  1M的計數頻率 1US計數
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	//不分頻
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上計數模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根據TIM_TimeBaseInitStruct中指定的參數初始化TIMx的時間基數單位		 
		
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);   	//清除更新中斷，避免立即產生中斷
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);    //啟用計時器更新中斷

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            	//設定中斷服務
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//主要優先順序
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         	//次要優先順序
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//啟用中斷
	NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM3,DISABLE);     
}

//計時器中斷服務程式
void TIM3_IRQHandler(void)   //TIM3中斷
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //檢查TIM更新中斷是否發生
    {
         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM更新中斷標誌 
         msHcCount++;
    }
}
 
 
//開始計數
static void StartTimer()        //打開計時器
{
    TIM_SetCounter(TIM3,0);	//清除計數
    msHcCount = 0;
    TIM_Cmd(TIM3, ENABLE);  //啟用TIM3外設	
}
 
static void StopTimer() 	//關閉計時器
{
    TIM_Cmd(TIM3, DISABLE); //啟用TIMx外設
}
 
//獲取計時器時間
float GetEchoTimer(void)
{
    u32 t = 0;
    t = msHcCount*1000;			//得到MS
    t += TIM_GetCounter(TIM3);	//得到US
	TIM3->CNT = 0;  			//將TIM3計數寄存器的計數值清零
	delay_ms(50);
    return t;
}
 
//取五次數據平均值
float HCSR04GetLength(void)
{
	u32 t = 0;
	int i = 0;
	float lengthTemp = 0;  
	float sum = 0; 	
	float distance;

	for(i=0;i<5;i++){		
		
		TRIG_SEND = 1;      //發送口高電平輸出
		delay_us(20);
		TRIG_SEND = 0;
			
		while(ECHO_RECEIVE == 0); 	//等待接收口高電平輸出
		StartTimer();        	//打開計時器

		while(ECHO_RECEIVE == 1);
		StopTimer();        	//關閉計時器
		
		t = GetEchoTimer();     //獲取時間,解析度為1US

		lengthTemp = ((float)t/58.2);	//cm
		sum = lengthTemp + sum ;
	}
	distance = 100*sum/5.0;
	return distance;
}
 


/*
float HCSR04GetLength(void)
{
	int distance = 0;
	int start_echo = 0;
	int end_echo = 0;

//    while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO)==1);//echo为高电平时,则等待至低电平,才启动超声波
	
//	UltrasonicWave_StartMeasure(); //启动超声波		
	GPIO_SetBits(HCSR04_PORT, HCSR04_TRIG);   //拉高PB1电平
	delay_us(20); 				//持续20us
	GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG); //拉低PB1电平

//	TRIG_SEND = 1;      //發送口高電平輸出
//	delay_us(20);
//	TRIG_SEND = 0;

	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO) == 0);//等待echo的高电平到来	
	start_echo = TIM_GetCounter(TIM3);
	
	TIM_SetCounter(TIM3,0); //清零计数器
	TIM_Cmd(TIM3, ENABLE);  //使能定时器2,开始计数

//	while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_Update));
	
	while(GPIO_ReadInputDataBit(HCSR04_PORT,HCSR04_ECHO) == 1);//等待echo的高电平结束

	TIM_Cmd(TIM3, DISABLE);	//失能定时器2,截止计数	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	end_echo = TIM_GetCounter(TIM3);
	distance = (end_echo - start_echo);
	
//	return (TIM_GetCounter(TIM3))/1000000*340/2 *100;	//此处单位转换为cm
//	return distance/1000000*340/2 *100;	//此处单位转换为cm
	return distance;
}
	
*/

