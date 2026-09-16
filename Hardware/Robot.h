#ifndef __ROBOT_H
#define __ROBOT_H

void Robot_Init(void);
void Robot_SetRun(uint8_t speed);
void Robot_SetBack(uint8_t speed);
void Robot_Setleft(uint8_t speed);
void Robot_SetRight(uint8_t speed);
void Robot_SetSpin_left(uint8_t speed);
void Robot_SetSpin_Right(uint8_t speed);
void Robot_SetBrake(void);

#endif
