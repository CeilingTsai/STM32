/**********************************************************
 * @file main.c
 * @date 2022/3/22  
 * @brief traffic light 
 **********************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

GPIO_InitTypeDef GPIO_InitStruct;    // 定義一個GPIO_InitTypeDef類型的結構體

int main (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //開啟GPIOA的外設時鐘
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;  //選擇要控制的GPIOA引腳
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;   //設置引腳速率為2MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   //設置引腳為通用推輓輸出模式
	GPIO_Init(GPIOA, &GPIO_InitStruct);  			//調用庫函數，初始化GPIOA5引腳

	// 將引腳設為高電位，此時三個燈熄滅
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	
	
	while (1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);		// 點亮紅燈	
		delay_s(3);								// 停 3 秒

		GPIO_SetBits(GPIOA, GPIO_Pin_0);		// 熄滅紅燈
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);		// 點亮黃燈	
		delay_s(1);

		GPIO_SetBits(GPIOA, GPIO_Pin_1);		// 熄滅黃燈
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);		// 點亮綠燈	
		delay_s(3);
		
		GPIO_SetBits(GPIOA, GPIO_Pin_2);		// 熄滅綠燈
	}
}
