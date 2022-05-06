/**********************************************************
 * @file  delay.c
 * @brief delay function
**********************************************************/
#include "delay.h"
 
static u8  fac_us=0;//us延時倍乘數
static u16 fac_ms=0;//ms延時倍乘數

//初始化延遲函數
//當使用ucos的時候,此函數會初始化ucos的時鐘節拍
//SYSTICK的時鐘固定為HCLK時鐘的1/8
//SYSCLK:系統時鐘
void delay_init()	 
{

#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定義了,說明使用ucosII了.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//選擇外部時鐘  HCLK/8
	fac_us=SystemCoreClock/8000000;	//為系統時鐘的1/8  
	 
#ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定義了,說明使用ucosII了.
	reload=SystemCoreClock/8000000;		//每秒鐘的計數次數 單位為K	   
	reload*=1000000/OS_TICKS_PER_SEC;//根據OS_TICKS_PER_SEC設定溢出時間
							//reload為24位寄存器,最大值:16777216,在72M下,約合1.86s左右	
	fac_ms=1000/OS_TICKS_PER_SEC;//代表ucos可以延時的最少單位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//開啟SYSTICK中斷
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中斷一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//開啟SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每個ms需要的systick時鐘數   
#endif
}								    

//延時nus
//nus為要延時的us數.	

void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //時間載入	  		 
	SysTick->VAL=0x00;        //清空計數器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //開始倒數	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待時間到達   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //關閉計數器
	SysTick->VAL =0X00;       //清空計數器	 
}
//延時nms
//注意nms的範圍
//SysTick->LOAD為24位寄存器,所以,最大延時為:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK單位為Hz,nms單位為ms
//對72M條件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//時間載入(SysTick->LOAD為24bit)
	SysTick->VAL =0x00;           //清空計數器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //開始倒數  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待時間到達   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //關閉計數器
	SysTick->VAL =0X00;       //清空計數器	  	    
} 

void delay_s(u16 s)			//S秒級延時程式（參考值即是延時數，最大值65535）
{ 	 		  	  
	while( s-- != 0)
	{
		delay_ms(1000);		//呼叫1000毫秒的延時
	}
}

