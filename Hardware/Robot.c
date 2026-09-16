#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "SystemTime.h"
#include "Buzzer.h"

typedef enum
{
	ROBOT_IDLE = 0,      /* 空闲 */
    
	ROBOT_BACK,          /* 后退中 */
    ROBOT_RIGHT          /* 右转中 */
	
} RobotState_t;

void Robot_Init(void)
{
	Motor_Init();
}

void Robot_SetRun(uint8_t speed)
{
	Motor_Speed(speed, 0, speed, 0);
}

void Robot_SetBack(uint8_t speed)
{
	Motor_Speed(0, speed, 0, speed);
}

void Robot_Setleft(uint8_t speed)
{
	Motor_Speed(speed - 10, 0, speed + 10, 0);
}

void Robot_SetRight(uint8_t speed)
{
	Motor_Speed(speed + 10, 0, speed - 10, 0);
}

void Robot_SetSpin_left(uint8_t speed)
{
	Motor_Speed(0, speed, speed, 0);
}

void Robot_SetSpin_Right(uint8_t speed)
{
	Motor_Speed(speed, 0, 0, speed);
}

void Robot_SetBrake(void)
{
	Motor_Speed(0, 0, 0, 0);
}
