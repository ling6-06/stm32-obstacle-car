#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	PWM_Init();
}

static uint8_t Limit_Speed(int16_t speed)
{
	if (speed > 100)
	{
		speed = 100;
	}
	if (speed < 0)
	{
		speed = 0;
	}
	return speed;
}

void Motor_Speed(int16_t left1, int16_t left2, int16_t right1, int16_t right2)
{
	TIM_SetCompare1(TIM4, Limit_Speed(left1));
	TIM_SetCompare2(TIM4, Limit_Speed(left2));
	TIM_SetCompare3(TIM4, Limit_Speed(right1));
	TIM_SetCompare4(TIM4, Limit_Speed(right2));
}
