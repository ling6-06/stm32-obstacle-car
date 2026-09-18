#include "stm32f10x.h"                  // Device header
#include "SystemTime.h"
#include "Motor.h"
#include "Robot.h"
#include "Buzzer.h"

/* ==================== ① 动作：一步能做什么 ==================== */
typedef enum
{
    ACT_STOP = 0,        /* 停（PWM 归零）*/
    ACT_RUN,             /* 前进 */
    ACT_BACK,            /* 后退 */
    ACT_LEFT,            /* 左偏 */
    ACT_RIGHT,           /* 右偏 */
    ACT_SPIN_LEFT,       /* 原地左转 */
    ACT_SPIN_RIGHT       /* 原地右转 */
} RobotAct_t;

/* ==================== ② 步骤 = 动作 + 持续时长 ==================== */
typedef struct
{
    RobotAct_t act;
    uint16_t   ms;
} RobotStep_t;

/* ==================== ③ 避障序列：整个动作在这里一目了然 ==================== */
#define AVOID_SPEED  70

static const RobotStep_t avoid_seq[] =
{
    { ACT_BACK,  500 },     /* 步骤 1：后退 0.5s    —— 退开      */
    { ACT_RIGHT,  500 },     /* 步骤 2：右转 0.5s  —— 转开      */
    { ACT_STOP,   100 },     /* 步骤 3：停 0.1s    —— 观察一下  */
};

#define AVOID_STEPS  (sizeof(avoid_seq) / sizeof(avoid_seq[0]))

/* ==================== ④ 可调参数 ==================== */
#define BUZZ_MS   500        /* 蜂鸣器响多久后关闭 */

/* ==================== ⑤ 状态与变量 ==================== */
typedef enum
{
    ROBOT_FREE = 0,      /* 空闲：没有序列在跑，由主循环决策 */
    ROBOT_AVOID          /* 避障序列执行中 */
} RobotState_t;

static RobotState_t robot_state = ROBOT_FREE;

static uint32_t seq_t0;       /* 整个序列的起点  —— 蜂鸣器用 */
static uint32_t step_t0;      /* 当前步骤的起点  —— 序列推进用 */
static uint8_t  step_idx;     /* 当前执行到第几步 */
static uint8_t  speed_now;
static uint8_t  buzzing;

/* ==================== ⑥ 执行一个动作 ==================== */
static void Robot_DoAct(RobotAct_t act, uint8_t speed)
{
    switch (act)
    {
        case ACT_STOP:       Robot_SetBrake();           break;
        case ACT_RUN:        Robot_SetRun(speed);        break;
        case ACT_BACK:       Robot_SetBack(speed);       break;
        case ACT_LEFT:       Robot_Setleft(speed);       break;
        case ACT_RIGHT:      Robot_SetRight(speed);      break;
        case ACT_SPIN_LEFT:  Robot_SetSpin_left(speed);  break;
        case ACT_SPIN_RIGHT: Robot_SetSpin_Right(speed); break;
        default:             Robot_SetBrake();           break;   /* 未知动作 → 停车，安全兜底 */
    }
}

/* ==================== ⑦ 启动避障序列 ==================== */
void Robot_StartAvoid(uint8_t speed)
{
    uint32_t now = Millis();

    seq_t0    = now;                          /* 序列起点 */
    step_t0   = now;                          /* 第 0 步的起点 */
    step_idx  = 0;
    speed_now = speed;

    Robot_DoAct(avoid_seq[0].act, speed);     /* 执行第 1 步 */
    Buzzer_ON();
    buzzing = 1;

    robot_state = ROBOT_AVOID;
}

/* ==================== ⑧ 推进（主循环每轮调用）==================== */
void Robot_Task(void)
{
    uint32_t now = Millis();
	
    /* ---------- 蜂鸣器：独立于序列进度 ---------- */
    if (buzzing && (now - seq_t0) >= BUZZ_MS)
    {
        Buzzer_OFF();
        buzzing = 0;
    }

	 /* ---------- 避障序列推进 ---------- */
    if (robot_state != ROBOT_AVOID) return;

    if ((now - step_t0) >= avoid_seq[step_idx].ms)
    {
        step_idx++;
        step_t0 = now;

        if (step_idx >= AVOID_STEPS)
        {
            robot_state = ROBOT_FREE;         /* 序列跑完，交回主循环 */
        }
        else
        {
            Robot_DoAct(avoid_seq[step_idx].act, speed_now);
        }
    }
}

uint8_t Robot_IsAvoiding(void)
{
    return (robot_state == ROBOT_AVOID);
}

