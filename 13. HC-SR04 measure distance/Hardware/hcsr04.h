#ifndef __HCSR04_H
#define __HCSR04_H

#define HCSR04_TRIG     GPIO_Pin_0
#define HCSR04_ECHO     GPIO_Pin_1
#define HCSR04_PORT     GPIOA

#define TRIG_SEND		PAout(0) 
#define ECHO_RECEIVE  	PAin(1)

void 	HCSR04_Init(void);
static void OpenTimerForHc(void);
void 	TIM3_IRQHandler(void);
float 	GetEchoTimer(void);
float 	HCSR04GetLength(void);

#endif
