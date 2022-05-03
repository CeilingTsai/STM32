#ifndef __dht11_H
#define __dht11_H 
#include "sys.h"
 
#define DHT11_pin 		GPIO_Pin_1 	//PA1
#define DHT11_Port 		GPIOA		//GPIO Port
#define DHT11_DQ_IN 	PAin(1)	  	//輸入
#define DHT11_DQ_OUT 	PAout(1)  	//輸出

void 	DHT11_IO_OUT(void);
void 	DHT11_IO_IN(void);
void 	DHT11_Rst(void);
uint8_t DHT11_Init(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(u8 *temp,u8 *tempd, u8 *humi, u8 *humid);
 
#endif
