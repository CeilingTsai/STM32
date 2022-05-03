#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h" 

//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_UCOS		0		//定義系統資料夾是否支援UCOS
	 
//位帶操作,實現51類似的GPIO控制功能
//具體實現思想,參考<<CM3權威指南>>第五章(87頁~92頁).
//IO口操作巨集定義

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口位址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只對單一的IO口!
//確保n的值小於16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //輸出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //輸入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //輸出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //輸入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //輸出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //輸入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //輸出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //輸入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //輸出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //輸入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //輸出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //輸入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //輸出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //輸入

void NVIC_Configuration(void);

#endif
