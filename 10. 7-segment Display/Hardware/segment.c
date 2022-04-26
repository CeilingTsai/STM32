#include "stm32f10x.h"
#include "segment.h"
#include "delay.h"

void Segment_Init(void){
	// 設定 GPIO    	
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SEG_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
}

void Segment_Display(u8 num){

	switch(num)
	{
		case 0:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F);
			break;

		case 1:
			GPIO_ResetBits(SEG_PORT, SEG_B|SEG_C);
			break;

		case 2:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_D|SEG_E|SEG_G);
			break;

		case 3:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_G);
			break;

		case 4:
			GPIO_ResetBits(SEG_PORT, SEG_B|SEG_C|SEG_F|SEG_G);
			break;

		case 5:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_C|SEG_D|SEG_F|SEG_G);
			break;

		case 6:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
			break;

		case 7:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_C);
			break;

		case 8:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G);
			break;

		case 9:
			GPIO_ResetBits(SEG_PORT, SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G);
			break;

		default:
			break;
	}	
}	

void Segment_Display2(u8 num){
	uint16_t SEGTAB[10]={0x80,0xf2,0x48,0x60,0x32,0x24,0x04,0xf0,0x00,0x20};
	if(num >= 10)
		return;
	GPIO_Write(SEG_PORT, SEGTAB[num]);
}

