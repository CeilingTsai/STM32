/**********************************************************
 * @file main.c
 * @date 2022/4/3  
 * @brief Push button control led-using GPIO
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
#include "button.h"

uint8_t KeyNum;

int main (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //開啟GPIOA的外設時鐘
	GPIO_InitTypeDef GPIO_InitStruct;    // 定義一個GPIO_InitTypeDef類型的結構體
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;  //選擇要控制的GPIOA引腳
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;   //設置引腳速率為2MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   //設置引腳為通用推輓輸出模式
	GPIO_Init(GPIOA, &GPIO_InitStruct);  			//調用庫函數，初始化GPIOA5引腳

	// 將引腳設為高電位
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	
	
	delay_init();	    	 //延時函數初始化	  
	OLED_Init();		 //初始化OLED  
	Button_Init2();
	OLED_Clear(0);           //清除螢幕
	OLED_ShowString(2, 2, "Status:",16);	
	while (1)
	{
		KeyNum = Button_Press();
		if (KeyNum == 1) {
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);		// 點亮紅燈
			OLED_ShowString(60, 2, "On ",16); 
		}
		else {	
			GPIO_SetBits(GPIOA, GPIO_Pin_0);		// 熄滅紅燈
			OLED_ShowString(60, 2, "Off",16);
		}
	}
}
