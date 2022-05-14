#ifndef _AHT10_H_
#define _AHT10_H_

#include "sys.h"
#include "delay.h"

#define AHT_ADDRESS 0X70 //0X38
#define AHT_WRITE   0X70
#define AHT_READ    0X71 //0X39

extern u8 ACK,DATA[6];

void  AHT10_Write_Reset(void);
u8    AHT10_State(void);
u8    AHT10_Read_Humi_Temp(float *humidity, float *temperature);

#endif
