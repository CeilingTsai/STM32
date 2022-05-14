#include "AHT10.h"
#include "I2C.h"

u8 ACK,DATA[6];

//存在0 不存在 1	
u8 AHT10_State(void)
{        
	u8 ACK;	
	I2C_Start();
	I2C_Send_Byte(AHT_WRITE);	
	ACK = I2C_Write_Ack();
	I2C_Stop();	
	return ACK;
}
void AHT10_Write_Init(void)
{ 
//	bit3 0 1
	I2C_Start();
	I2C_Send_Byte(AHT_WRITE);	
	I2C_Write_Ack();
	I2C_Send_Byte(0XE1);
	I2C_Write_Ack();
	I2C_Send_Byte(0X08);
	I2C_Write_Ack();
	I2C_Send_Byte(0X00);	
	I2C_Write_Ack();
	I2C_Stop();
	delay_ms(40);
}

void AHT10_Write_Reset(void)
{
	I2C_Start();
	I2C_Send_Byte(AHT_WRITE);
	I2C_Write_Ack();
	I2C_Send_Byte(0XBA);	
	I2C_Write_Ack();
	I2C_Stop();
	delay_ms(20);
}

u8 AHT10_Read_Humi_Temp(float *humidity, float *temperature)
{ 
	u32 humi = 0,temp = 0;
	
	I2C_Start();
	I2C_Send_Byte(AHT_WRITE);		
	I2C_Write_Ack();
	I2C_Send_Byte(0XAC);	 //觸發測量	
	I2C_Write_Ack();
	I2C_Send_Byte(0X33);		
	I2C_Write_Ack();
	I2C_Send_Byte(0X00);		
	I2C_Write_Ack();
	I2C_Stop();
	
	delay_ms(80);
	
	I2C_Start();
	I2C_Send_Byte(AHT_READ);	
	I2C_Write_Ack();
	ACK = I2C_Read_Data();
	I2C_Is_Ack(1);
	
	if((ACK&0X08) == 0)
	{
		AHT10_Write_Init();
	}
	if((ACK&0X80) == 0)
	{ 	
	//bit7 1 0
		for(u8 i=0;i<5;i++){ // 0 1 2 3 4 5 ++i
			
			DATA[i] = I2C_Read_Data();			
			if(i == 4)
				I2C_Is_Ack(0);
			else
				I2C_Is_Ack(1);
		} 
		I2C_Stop();
		
		humi = (DATA[0]<<12)|(DATA[1]<<4)|(DATA[2]>>4);
		temp = ((DATA[2]&0X0F)<<16)|(DATA[3]<<8)|(DATA[4]);
		
		*humidity = (humi * 100.0/1024/1024+0.5);
		*temperature = (temp * 2000.0/1024/1024+0.5)/10.0-50;

		return 0;
	}
	
	I2C_Stop();
	return 1;
}
