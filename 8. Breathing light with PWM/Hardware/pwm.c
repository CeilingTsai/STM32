#include "pwm.h"

void PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;              //定義GPIO結構體
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    //定義TIMx定時器結構體
    TIM_OCInitTypeDef TIM_OCInitStructure;            //定義定時器脈寬調製結構體
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                     //使能TIM3時鐘
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//使能GPIOB時鐘和AFIO複用時鐘
    
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);    //TIM3部分重映射 TIM3_CH2->PB5
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //複用推輓輸出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;      //配置輸出速率
    GPIO_Init(GPIOB,&GPIO_InitStructure);                  //初始化GPIOB
    
    TIM_TimeBaseStructure.TIM_Period = arr;                //設置自動重裝載寄存器週期的值 arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;             //設置預分頻值 psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;           //設置時鐘分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //TIM向上計數模式
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);         //初始化TIMx時間基數
    
    //初始化TIM3 Channel2 PWM模式     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //選擇定時器模式:TIM脈衝寬度調製模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能比較輸出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //輸出極性:TIM輸出比較極性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);                    //根據T指定的參數初始化外設TIM3 OC2

    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);           //使能TIM3在CCR2上的預裝載寄存器
    TIM_Cmd(TIM3, ENABLE);                                     //使能TIM3
}
