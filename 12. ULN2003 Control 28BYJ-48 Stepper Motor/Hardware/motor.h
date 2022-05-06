#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_PORT GPIOA
#define IN1 GPIO_Pin_1
#define IN2 GPIO_Pin_2
#define IN3 GPIO_Pin_3
#define IN4 GPIO_Pin_4

#define IN1_HIGH GPIO_SetBits(MOTOR_PORT,IN1);
#define IN1_LOW  GPIO_ResetBits(MOTOR_PORT,IN1);

#define IN2_HIGH GPIO_SetBits(MOTOR_PORT,IN2);
#define IN2_LOW  GPIO_ResetBits(MOTOR_PORT,IN2);

#define IN3_HIGH GPIO_SetBits(MOTOR_PORT,IN3);
#define IN3_LOW  GPIO_ResetBits(MOTOR_PORT,IN3);

#define IN4_HIGH GPIO_SetBits(MOTOR_PORT,IN4);
#define IN4_LOW  GPIO_ResetBits(MOTOR_PORT,IN4);

void Step_Motor_GPIO_Init(void);
void motor_circle(int n, int direction, int delay);
void Phase4_Single(u8 step, u8 delay);
void Phase8_Single(u8 step, u8 delay);
void Phase4_Double(u8 step, u8 delay);
void Motor_Start(u8 direction, u8 freq, u8 phase);
void Motor_Start_Angle(u8 direction, u8 freq, u8 phase, u16 angle);
void Motor_Stop(void);

#endif

