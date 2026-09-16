#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

extern float volatile Ultrasonic_Distance;
void Ultrasonic_Init(void);
float Ultrasonic_StartMeasure(void);

#endif
