/**********************************************************
 * @file main.c
 * @date 2022/4/19  
 * @brief Display Voltage using DMA on OLED
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

float ADValue[Channel_Num];

int main (void)
{
	uint16_t vtg;
	
	delay_init();	   	//延時函數初始化	  	
	OLED_Init();		//初始化OLED  
	ADC_GPIO_Config();
	ADC_Mode_Config();
	
	OLED_Clear(0);     	//清除螢幕
	OLED_ShowString(1, 2, "Voltage1:",16);
	OLED_ShowString(1, 4, "Voltage2:",16);
	OLED_ShowString(1, 6, "Voltage3:",16);
	
	while (1)
	{
		u16 x;
		u16 colnum;
		for(x=0;x<3;x++){

			ADValue[x] = Read_ADC_AverageValue(x);
		
			colnum = 2+(x*2);
			OLED_ShowNum(72, colnum, ADValue[x] / 4096 * 3.3, 1, 16);	  //整數位
			OLED_ShowString(82, colnum, ".",16); 

			vtg = (uint16_t)(ADValue[x] / 4096 * 3.3 * 100) % 100;  //小數點後兩位
			if (vtg < 10) {
				OLED_ShowNum(90, colnum, vtg, 2, 16);	
				OLED_ShowString(89, colnum,"0",16); 		//個位數時前面補0	
			} else
			{
				OLED_ShowNum(90, colnum, vtg, 2, 16);			
			}
		
			delay_ms(300);
		}	
	}
}
