/**********************************************************
 * @file main.c
 * @date 2022/5/6
 * @brief ULN2003 Control 28BYJ-48 stepper motor
 **********************************************************/
#include "delay.h"
#include "sys.h"
#include "motor.h"
#include "button.h"
	
int main(void)
{
	uint16_t btn_count;
	
	delay_init(); 				//延時函數初始化
	Button_Init();				//按鍵初始化
	Step_Motor_GPIO_Init();		//步進馬達初始化
	
	while (1)
	{
		btn_count = Button_Get();
		if (btn_count%5 == 1){
			Motor_Start(1, 4, 1); 	// 順時鐘轉, 延時需大於4ms, 使用波浪驅動 	
		} 
		if (btn_count%5 == 2){
			Motor_Start(0, 4, 2); 	// 逆時鐘轉, 延時需大於4ms, 使用全步驅動
		}
		if (btn_count%5 == 3){ 
			Motor_Start(1, 2, 3); 	// 順時鐘轉, 延時2ms, 使用半步驅動
		}	
		if (btn_count%5 == 4){	
			Motor_Start_Angle(0, 4, 1, 180); //	轉動 180度
			delay_ms(50000);    	// 暫停再繼續
		} 
	}	
}

