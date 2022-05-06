#ifndef __BUTTON_H
#define __BUTTON_H

#define GPIO_PORT GPIOB
#define GPIO_PIN  GPIO_Pin_14

void Button_Init(void);
uint16_t Button_Get(void);

#endif
