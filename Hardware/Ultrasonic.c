#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"

#define GPIO_Pin_Echo		GPIO_Pin_14			//超声波模块输入
#define GPIO_Pin_Trig		GPIO_Pin_15			//超声波模块输出

float volatile Ultrasonic_Distance;

void Ultrasonic_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Trig;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Echo;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	Ultrasonic_Distance = 5.0f;		//先给一个远距离防止上电直接触发距离过近函数
}

void EXTI15_10_IRQHandler(void)
{
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_Echo) != RESET)
	{	
		TIM_SetCounter(TIM2, 0);
		TIM_Cmd(TIM2, ENABLE);
	}
	else
	{
		Ultrasonic_Distance = TIM_GetCounter(TIM2) * 100 / 1000000.0 * 340 / 2; 		// 单位为m
		TIM_Cmd(TIM2, DISABLE);
	}
	EXTI_ClearFlag(EXTI_Line14);
}

float Ultrasonic_StartMeasure(void)
{
	float Temp;
	GPIO_SetBits(GPIOB, GPIO_Pin_Trig);
	Delay_us(20);
	GPIO_ResetBits(GPIOB, GPIO_Pin_Trig);
	Temp = Ultrasonic_Distance * 100;		//单位为cm
	return Temp;
}
