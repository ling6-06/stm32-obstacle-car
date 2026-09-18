#ifndef __AVOID_H
#define __AVOID_H

#define AVOID_SPEED  70

void Robot_StartAvoid(uint8_t speed);
void Robot_Task(void);
uint8_t Robot_IsAvoiding(void);

#endif
