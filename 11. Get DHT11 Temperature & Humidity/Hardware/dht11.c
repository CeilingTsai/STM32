#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

// DHT11初始化，返回0：初始化成功，1：失敗
uint8_t DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = DHT11_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Port,&GPIO_InitStructure);
	GPIO_SetBits(DHT11_Port,DHT11_pin);	   		//拉高
 
	DHT11_Rst();	  
	return DHT11_Check();	
}

//重置DHT11
void DHT11_Rst()	   
{                 
	DHT11_IO_OUT(); 		//設定I/O口為輸出模式
    DHT11_DQ_OUT=0; 		//拉低DQ
    delay_ms(20);    		//拉低至少18ms
    DHT11_DQ_OUT=1; 		//DQ=1 
	delay_us(30);     		//主機拉高20~40us
}

// 檢測DHT11，傳回0:存在，返回1:未檢測到DHT11的存在
uint8_t DHT11_Check() 	   
{   
	u8 retry=0;
	DHT11_IO_IN();						//設定I/O口為輸入模式 
    while (DHT11_DQ_IN&&retry<100)		//高電平迴圈，低電平跳出，DHT11會拉低40~80us
	{
		retry++;
		delay_us(1);
	}; 
	if(retry>=100)return 1;
	else retry=0;
	
    while (!DHT11_DQ_IN && retry<100)	//DHT11拉低後會再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
 
//從DHT11讀取一個位，返回值：1或0
uint8_t DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)	//等待變為低電平 12-14us 開始
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)	//等待變高電平 26-28us表示0,116-118us表示1
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);		//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//從DHT11讀取一個位元組，返回值：讀到的數據
uint8_t DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1;    			//dat的數據左移一個bit, 右邊補0
	    dat|=DHT11_Read_Bit();  //兩邊做 OR 運算，結果存到 dat
    }						    
    return dat;
}

//從DHT11讀取一次資料，返回值：0,正常;1,讀取失敗
uint8_t DHT11_Read_Data(u8 *temp,u8 *tempd, u8 *humi, u8 *humid)
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)	//讀取40位元數據
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi =buf[0];
			*humid=buf[1];
			*temp =buf[2];
			*tempd=buf[3];
		}
	}
		else {
			return 1;}
		return 0;	    
}

//DHT11 輸出模式配置
void DHT11_IO_OUT()	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //推挽輸出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_Port,&GPIO_InitStructure);	
}
 
//DHT11 輸入模式配置
void DHT11_IO_IN()	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 	//上拉輸入模式
	GPIO_Init(DHT11_Port,&GPIO_InitStructure);	
}
