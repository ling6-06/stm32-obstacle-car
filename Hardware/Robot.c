#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "SystemTime.h"
#include "Buzzer.h"

void Robot_SetBack(uint8_t speed);
void Robot_SetRight(uint8_t speed);
void Robot_SetBrake(void);

/* ==================== 状态定义 ==================== */
typedef enum
{
	ROBOT_IDLE = 0,      /* 空闲 */
	ROBOT_BACK,          /* 后退中 */
    ROBOT_RIGHT          /* 右转中 */
	
} RobotState_t;
 

/* ==================== 可调参数 ==================== */
#define BUZZ_MS     500      /* 蜂鸣器响 500ms 后关闭 —— 你自己定这个时刻 */
#define BACK_MS    1000      /* 后退持续 1000ms */
#define RIGHT_MS    700      /* 右转持续 700ms */

/* ==================== 内部状态 ==================== */
static RobotState_t robot_state = ROBOT_IDLE;
static uint32_t     seq_t0;      /* 整个序列的起点  —— 蜂鸣器用 */
static uint32_t     step_t0;     /* 当前这一步的起点 —— 状态机用 */
static uint8_t      speed_now;   /* 启动时传入的速度，跨状态传递 */
static uint8_t      buzzing;     /* 蜂鸣器在响吗（兼作"已经关过了"的标志）*/

/* ==================== 启动：进入第一个状态 ==================== */
void Robot_StartAvoid(uint8_t speed)
{
    uint32_t now = Millis();

    seq_t0    = now;
    step_t0   = now;             /* 第一步的起点 = 序列起点 */
    speed_now = speed;

    Robot_SetBack(speed);        /* 进入 ROBOT_BACK 该做的动作 */
    Buzzer_ON();
    buzzing = 1;

    robot_state = ROBOT_BACK;    /* ★ 记住进度 */
}

/* ==================== 推进：主循环每轮调用 ==================== */
void Robot_Task(void)
{
    uint32_t now = Millis();     /* 一轮只读一次表 */

    /* ---------- 线 A：蜂鸣器（独立，只看 seq_t0）---------- */
    if (buzzing && (now - seq_t0) >= BUZZ_MS)
    {
        Buzzer_OFF();
        buzzing = 0;
    }

    /* ---------- 线 B：运动状态机（看 step_t0）---------- */
    switch (robot_state)
    {
        case ROBOT_IDLE:
            break;

        case ROBOT_BACK:
            if ((now - step_t0) >= BACK_MS)
            {
                Robot_SetRight(speed_now);
                step_t0     = now;           /* 刷新"这一步"的起点 */
                robot_state = ROBOT_RIGHT;
            }
            break;

        case ROBOT_RIGHT:
            if ((now - step_t0) >= RIGHT_MS)
            {
                Robot_SetBrake();
                robot_state = ROBOT_IDLE;
            }
            break;

        default:
            break;
    }
}

uint8_t Robot_IsIdle(void)
{
    return (robot_state == ROBOT_IDLE);
}

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
