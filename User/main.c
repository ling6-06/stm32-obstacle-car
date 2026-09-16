#include "stm32f10x.h" // Device header
#include "SystemTime.h"
#include "Delay.h"
#include "Key.h"
#include "Buzzer.h"
#include "Timer.h"
#include "Robot.h"
#include "Ultrasonic.h"

float Distance;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemTime_Init();
	Key_Init();
	Buzzer_Init();
	Timer_Init();
	Robot_Init();
	Ultrasonic_Init();
	
	while(Key_GetNum() == RESET);
	while(1)
	{   
		Distance = Ultrasonic_StartMeasure();
		
		if(Distance < 40)
		{
			Buzzer_ON();
			Robot_SetBack(70);
			Buzzer_OFF();
			Robot_SetRight(70);
		}
		else
		{
			Robot_SetRun(70);
		}
	}
} 
