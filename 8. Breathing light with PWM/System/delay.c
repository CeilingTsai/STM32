/**********************************************************
 * @file  delay.c
 * @brief delay function
**********************************************************/

#include "delay.h"
#include "sys.h"
 
static u8  fac_us=0;	//us延時倍乘數
static u16 fac_ms=0;	//ms延時倍乘數

//初始化延遲函數
//SYSTICK的時鐘固定為HCLK時鐘的1/8
//SYSCLK:系統時鐘
void delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);    //選擇外部時鐘 HCLK/8
    fac_us = SystemCoreClock/8000000;                        //為系統時鐘的1/8
    fac_ms = (u16)fac_us*1000;                               //每個ms需要的systick時鐘數
}								    

//延時nus，nus 為要延時的微秒數.	

void delay_us(u32 nus)
{		
	u32 temp;
    SysTick->LOAD = nus*fac_us;                 //時間載入
    SysTick->VAL = 0x00;                        //清空計數器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //開始倒數
    do
    {
        temp = SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));       //等待時間到達
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //關閉計數器
    SysTick->VAL = 0x00;                        //清空計數器
}

//延時 nms，nms 為要延時的毫秒數
//nms的範圍:SysTick->LOAD為24位暫存器,最大延時為: nms<=0xffffff*8*1000/SYSCLK
//SYSCLK單位為Hz, 單位為ms
//對72MHz條件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
    u32 temp;
    SysTick->LOAD = (u32)nms*fac_ms;            //時間載入(SysTick->LOAD為24bit)
    SysTick->VAL = 0x00;                        //清空計數器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //開始倒數
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));       //等待時間到達 
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //關閉計數器
    SysTick->VAL = 0x00;                        //清空計數器
} 

//延時 s 秒，s 為延時秒數，最大值為65535
void delay_s(u16 s)			
{ 	 		  	  
	while( s-- != 0)
	{
		delay_ms(1000);		//呼叫1000毫秒的延時
	}
}
