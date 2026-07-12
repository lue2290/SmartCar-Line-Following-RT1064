#ifndef _servo_h
#define _servo_h

#include "zf_common_headfile.h"

#define SERVO_PWM_PIN (PWM4_MODULE2_CHA_C30)

#define SERVO_MID 700 // 舵机中值
#define SERVO_LIM 60  // 舵机极限
#define SERVO_HIM 830 // 舵机极限

void servo_init(void);
void servo_set(int16 duty);

#endif
