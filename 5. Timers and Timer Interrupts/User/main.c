/**********************************************************
 * @file main.c
 * @date 2022/4/7  
 * @brief Timers and Timer Interrupts
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
#include "Timer.h"

uint16_t Second;

int main (void)
{
	delay_init();	    	 //延時函數初始化	  
	NVIC_Configuration(); 	 //設置 NVIC 中斷分組2:2位搶佔優先順序，2位回應優先順序 	
	OLED_Init();		 //初始化OLED  
	Timer_Init();
	
	OLED_Clear(0);           //清除螢幕
	OLED_ShowString(2, 2, "Second:",16);	
	while (1)
	{

		OLED_ShowNum(60, 2, Second,5, 16);
	}
}

