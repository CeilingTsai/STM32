/**********************************************************
 * @file main.c
 * @date 2022/4/14  
 * @brief ADC-Display voltage on OLED
 *        	VDD 	- 3.3V
 *       	GND 	- GND
 * 	  	SCL/SCK - PB8    // IIC時鐘信號
 *	  	SDA 	- PB9    // IIC資料信號
 **********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"
#include "adcx.h"

float ADValue;

int main (void)
{
	uint16_t vtg;
	delay_init();	   	//延時函數初始化	  	
	OLED_Init();		//初始化OLED  
	ADCX_Init();
	
	OLED_Clear(0);     	//清除螢幕
	
	OLED_ShowString(1, 2, "ADValue:",16);
	OLED_ShowString(1, 4, "Voltage:",16);

	while (1)
	{
		ADValue = ADCX_GetValue(0);
		OLED_ShowNum(70, 2, ADValue, 4, 16);		
		OLED_ShowNum(70, 4, ADValue / 4096 * 3.3, 1, 16);	  //整數位
		OLED_ShowString(80, 4, ".",16); 

		vtg = (uint16_t)(ADValue / 4096 * 3.3 * 100) % 100;  //小數點後兩位
		if (vtg < 10) {
			OLED_ShowNum(88, 4, vtg, 2, 16);	
			OLED_ShowString(87, 4,"0",16); 		//個位數時前面補0	
		} else
		{
			OLED_ShowNum(88, 4, vtg, 2, 16);			
		}
		
		delay_ms(300);
	}
}
