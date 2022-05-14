#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

#define READ_SDA   PBin(7)  //輸入SDA 

void SDA_IN(void);
void SDA_OUT(void);


void I2C_Initation(void);           //初始化IIC的IO口				 
void I2C_Start(void);				//發送IIC開始信號
void I2C_Stop(void);	  			//發送IIC停止信號
void I2C_Send_Byte(u8 txd);			//IIC sends a byte
void I2C_Is_Ack(u8 ack);			// ack=0時,不發送ACK應答, ack=1時,發送ACK應答
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8   I2C_Read_Data(void);
u8   I2C_Write_Ack(void);
u8 	 I2C_Read_Byte(unsigned char ack); //IIC reads a byte
u8 	 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
// u8 IIC_Wait_Ack(void); 				//IIC waits for ACK signal
#endif
