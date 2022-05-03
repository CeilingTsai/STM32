/**********************************************************
 * @file main.c
 * @brief Breathing light with PWM
 *        	VDD 	- 3.3V
 *       	GND 	- GND
 * 	  	SCL/SCK - PB8    // IIC時鐘信號
 *	  	SDA 	- PB9    // IIC資料信號
 **********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "pwm.h"

//主函數	
int main(void)
{
    u16 pwmValue = 1;
    u8 dir = 1;
	delay_init();	    	 //延時函數初始化	  
	
	OLED_Init();		 //初始化OLED  
	OLED_Clear(0);           //清除螢幕
	OLED_ShowString(2, 2, "Count:",16);	
	
    PWM_Init(99, 7199);
	
    while(1)
    {
		OLED_ShowNum(60, 2, pwmValue,3, 16); 		
        delay_ms(30);		
        if(dir) pwmValue++;  //從暗到亮
		else pwmValue--;     //從亮到暗
		
		if(pwmValue >= 100) dir = 0;
        if(pwmValue <= 1) dir = 1;

		TIM_SetCompare2(TIM3,pwmValue);    //通道2, 改變比較值CCRx
    }
}
