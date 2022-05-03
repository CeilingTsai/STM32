#include "sys.h"

void NVIC_Configuration(void)
{
    //設置NVIC中斷分組2:2位搶佔優先順序，2位回應優先順序
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

}
