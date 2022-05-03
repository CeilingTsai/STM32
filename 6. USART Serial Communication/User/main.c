/**********************************************************
 * @file main.c
 * @date 2022/4/7  
 * @brief USART communication 
 **********************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "USART.h"

u8 ch2;

int main (void)
{
	delay_init();	  
	USART_User_Init(115200);

	while (1)
	{

	}
}
