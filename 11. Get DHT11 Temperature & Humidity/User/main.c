/**********************************************************
 * @file main.c
 * @date 2022/5/3  
 * @brief DIsplay DHT11 temperature & Humidity on OLED
 **********************************************************/

#include <stm32f10x.h>
#include "oled.h"
#include "delay.h"
#include "dht11.h"
 
int main()
{
	u8 temperature, tempd;         
	u8 humidity, humid; 

	delay_init();	   	//延時函數初始化	  	
	OLED_Init();		//初始化OLED  
	DHT11_Init();       //DHT11初始化
	OLED_Clear(0);     	//清除螢幕
	
	OLED_ShowString(1, 2, "Temper. :",16);
	OLED_ShowString(1, 4, "Humidity:",16);
	while(1)
	{
		DHT11_Read_Data(&temperature,&tempd,&humidity,&humid);				 

		OLED_ShowNum(70, 2, temperature, 3, 16);	
		OLED_ShowString(94,2, ".",16);		
		OLED_ShowNum(98, 2, tempd, 1, 16);	
		
		OLED_ShowNum(70, 4, humidity, 3, 16);	
		OLED_ShowString(94,4, ".",16);				
		OLED_ShowNum(98, 4, humid, 1, 16);
		delay_ms(100);
	}
}
