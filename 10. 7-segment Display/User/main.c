/**********************************************************
 * @file main.c
 * @date 2022/4/23  
 * @brief Display from 0 to 9 on 7-segment
 **********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "segment.h"

int main(void) 
{

	delay_init();  
	Segment_Init();

	while (1)
	{
		for(int x=0;x<10;x++){	
			Segment_Display(x);
			delay_ms(1000);
			GPIO_SetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
		}
		delay_ms(3000);
				
		int y=9;
		for(int x=0;x<10;x++){
			Segment_Display2(y);
			y--;
			delay_ms(1000);
			GPIO_SetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
		}
		delay_ms(3000);

	}
}
