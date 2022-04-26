#ifndef __SEGMENT_H
#define __SEGMENT_H

#define SEG_PORT GPIOA

#define SEG_A GPIO_Pin_1
#define SEG_B GPIO_Pin_2
#define SEG_C GPIO_Pin_3
#define SEG_D GPIO_Pin_4
#define SEG_E GPIO_Pin_5
#define SEG_F GPIO_Pin_6
#define SEG_G GPIO_Pin_7

void Segment_Init(void);
void Segment_Display (u8 num);
void Segment_Display2(u8 num);

#endif
