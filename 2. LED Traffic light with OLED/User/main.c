/**********************************************************
 * @file main.c
 * @date 2022/3/27  
 * @brief traffic light with OLED
 *        	VDD 	- 3.3V
 *       	GND 	- GND
 * 	  	SCL/SCK - PB8    // IIC時鐘信號
 *	  	SDA 	- PB9    // IIC資料信號
 **********************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"

GPIO_InitTypeDef GPIO_InitStruct;    // 定義一個GPIO_InitTypeDef類型的結構體

int main (void)
{
	delay_init();	    	 //延時函數初始化	  
	NVIC_Configuration(); 	 //設置 NVIC 中斷分組2:2位搶佔優先順序，2位回應優先順序 	
	OLED_Init();		 //初始化OLED  
	OLED_Clear(0);           //清除螢幕

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //開啟GPIOA的外設時鐘
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;  //選擇要控制的GPIOA引腳
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;   		  //設置引腳速率為2MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   		  //設置引腳為通用推輓輸出模式
	GPIO_Init(GPIOA, &GPIO_InitStruct);  				  //調用庫函數，初始化GPIOA5引腳

	// 將引腳設為高電位，此時三個燈熄滅
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);	

	OLED_Init();
		
	OLED_ShowString(1, 2, "Welcome...  ",16);
	OLED_ShowString(1, 4, "Stop Light Proj",16);
	delay_s(3);

	OLED_Clear(0);
	
	while (1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	// 點亮紅燈	
		OLED_ShowString(6, 3, "Red    ",16); 
		delay_s(3);				// 停 3 秒

		GPIO_SetBits(GPIOA, GPIO_Pin_0);	// 熄滅紅燈
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);	// 點亮黃燈	
		OLED_ShowString(6, 3, "Yellow",16);
		delay_s(1);

		GPIO_SetBits(GPIOA, GPIO_Pin_1);	// 熄滅黃燈
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);	// 點亮綠燈	
		OLED_ShowString(6, 3, "Green ",16);		
		delay_s(3);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_2);	// 熄滅綠燈
	}
}
