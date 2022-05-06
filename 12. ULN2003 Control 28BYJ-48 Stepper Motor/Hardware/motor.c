#include "delay.h"
#include "motor.h"

//引腳初始化
void Step_Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = IN1|IN2|IN3|IN4;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
}

// 波浪驅動
void Phase4_Single(u8 step, u8 delay)
{
    switch(step){
		case 1:
		IN1_HIGH; IN2_LOW; IN3_LOW; IN4_LOW;
		break;
	case 2:
		IN1_LOW; IN2_HIGH; IN3_LOW; IN4_LOW;
		break;
	case 3:
		IN1_LOW; IN2_LOW; IN3_HIGH; IN4_LOW;		
		break;
	case 4:
		IN1_LOW; IN2_LOW; IN3_LOW; IN4_HIGH;			
		break;
	}
	delay_ms(delay);
}

// 全步驅動
void Phase4_Double(u8 step, u8 delay)
{
    switch(step){
		case 1:
		IN1_LOW; IN2_HIGH; IN3_HIGH; IN4_LOW;
		break;
	case 2:
		IN1_LOW; IN2_LOW; IN3_HIGH; IN4_HIGH;
		break;
	case 3:
		IN1_HIGH; IN2_LOW; IN3_LOW; IN4_HIGH;		
		break;
	case 4:
		IN1_HIGH; IN2_HIGH; IN3_LOW; IN4_LOW;			
		break;
	}
	delay_ms(delay);
}

// 半步驅動
void Phase8_Single(u8 step, u8 delay)
{
    switch(step){
		case 1:
		IN1_LOW; IN2_HIGH; IN3_HIGH; IN4_HIGH;
		break;
	case 2:
		IN1_LOW; IN2_LOW; IN3_HIGH; IN4_HIGH;
		break;
	case 3:
		IN1_HIGH; IN2_LOW; IN3_HIGH; IN4_HIGH;		
		break;
	case 4:
		IN1_HIGH; IN2_LOW; IN3_LOW; IN4_HIGH;			
		break;
	case 5:
		IN1_HIGH; IN2_HIGH; IN3_LOW; IN4_HIGH;
		break;
	case 6:
		IN1_HIGH; IN2_HIGH; IN3_LOW; IN4_LOW;
		break;
	case 7:
		IN1_HIGH; IN2_HIGH; IN3_HIGH; IN4_LOW;		
		break;
	case 8:
		IN1_LOW; IN2_HIGH; IN3_HIGH; IN4_LOW;			
		break;	
	}
	delay_ms(delay);
}

//馬達停止
void Motor_Stop(void)
{
	IN1_LOW; IN2_LOW; IN3_LOW; IN4_LOW;	
}
	
/*
功能：轉動1/64圈
direction: 轉動方向 1:正轉 非1:反轉
delay: 延時時長，需 >= 2
phase: 1:四相單步, 2:四相雙步, 3:八相單步
*/

void Motor_Start(u8 direction, u8 delay, u8 phase)
{
	if (direction){  // 順時針轉			
		switch(phase){
			case 1:	
				for(u8 i=1;i<5;i++){
					Phase4_Single(i, delay);	}
				break;
			case 2:
				for(u8 i=1;i<5;i++){
					Phase4_Double(i, delay); }
				break;
			case 3:
				for(u8 i=1;i<9;i++){
					Phase8_Single(i, delay); }
				break;
			default:
				break;
		}	
	} else // 逆時針轉
	{
		switch(phase){
			case 1:	
				for(u8 i=4;i>0;i--){
					Phase4_Single(i, delay); }
				break;
			case 2:
				for(u8 i=4;i>0;i--){
					Phase4_Double(i, delay); }
				break;
			case 3:
				for(u8 i=8;i>0;i--){
					Phase8_Single(i, delay);	}
				break;
			default:
				break;
		}	
	}	
}

/*
功能：旋轉至特定角度
direction: 轉動方向 1:正轉 非1:反轉
delay: 延時時長，需 >= 2
phase: 1:四相單步, 2:四相雙步, 3:八相單步
angle: 轉動角度
*/
void Motor_Start_Angle(u8 direction, u8 delay, u8 phase, u16 angle)
{
	for(u16 j=0;j<(64*angle/45);j++){
		Motor_Start(direction, delay, phase);
	}
}

