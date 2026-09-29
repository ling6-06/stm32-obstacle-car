#ifndef __AVOID_H
#define __AVOID_H

#define AVOID_SPEED  70
#define TRIGGER_CM   30      /* 近了 → 开始避障 */

void Robot_StartAvoid(uint8_t speed, float distance_cm);
void Robot_Task(float distance_cm);
uint8_t Robot_IsAvoiding(void);

#endif
