#include "stm32f10x.h"  

#define TXD GPIO_Pin_9
#define RXD GPIO_Pin_10
#define USART_GPIO GPIOA

extern u8 ch2;
 
void USART_User_Init(uint32_t BaudRate)  
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	// Config GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = TXD;  	// Output PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RXD;  	// Input 10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO, &GPIO_InitStructure);

	// Config USART	
	USART_InitTypeDef USART_InitStructure;

	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = BaudRate;			//一般設定爲 9600 或 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  	// 資料長度 = 8 Bits
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 1 個停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//無奇偶校驗位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//無硬體流控(即禁止 RTS 和 CTS )	
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//收發模式
	USART_Init(USART1, &USART_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		//開啓 ENABLE 中斷
	USART_Cmd(USART1, ENABLE);

	// Config NVIC 	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //主優先級1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //次優先級0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//啟用IRQ通道	
	NVIC_Init(&NVIC_InitStructure);
	
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
	    ch2 = USART_ReceiveData(USART1);  //接收來自終端機資料
		USART_SendData(USART1, ch2);	
//		USART_SendData(USART1, USART_ReceiveData(USART1));

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}
