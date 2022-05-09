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
#include "delay.h"
#include "oled.h"
#include "hcsr04.h"

uint16_t Second;

int main (void)
{
	u32 vtg;
	u32 distance;
	
	delay_init();	    	 //延時函數初始化	  
	OLED_Init();			 //初始化OLED  
	HCSR04_Init();

	OLED_Clear(0);           //清除螢幕
	OLED_ShowString(2, 2, "Dist.:",16);	
	while (1)
	{
		distance = HCSR04GetLength();
		OLED_ShowNum(52, 2, distance/100, 4, 16);	  //整數位
	    OLED_ShowString(85, 2, ".",16); 

		vtg = distance % 100;  	//小數點後兩位
		if (vtg < 10) {
			OLED_ShowNum(93, 2, vtg, 2, 16);
			OLED_ShowString(93, 2,"0",16); 			//個位數時前面補0	
		} else
		{
			OLED_ShowNum(93, 2, vtg, 2, 16);			
		}
		
//		OLED_ShowNum(70, 2, distance,5, 16);
		delay_ms(200);
	}
}

