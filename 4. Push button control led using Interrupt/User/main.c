/**********************************************************
 * @file main.c
 * @date 2022/4/3  
 * @brief Push button control led using Interrupt
 *        	VDD 	- 3.3V
 *       	GND 	- GND
 * 	  		SCL/SCK - PB8    // IIC時鐘信號
 *	  		SDA 	- PB9    // IIC資料信號
 **********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "button.h"

// GPIO_InitTypeDef GPIO_InitStruct;    // 定義一個GPIO_InitTypeDef類型的結構體

int main (void)
{
	delay_init();	    	 //延時函數初始化	  
	NVIC_Configuration(); 	 //設置 NVIC 中斷分組2:2位搶佔優先順序，2位回應優先順序 	
	OLED_Init();		 //初始化OLED  
	Button_Init();
	OLED_Clear(0);           //清除螢幕
	OLED_ShowString(2, 2, "Count:",16);	
	while (1)
	{

		OLED_ShowNum(60, 2, Button_Get(),3, 16); 
	}
}
