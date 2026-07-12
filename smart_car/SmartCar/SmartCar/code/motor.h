#ifndef _motor_h
#define _motor_h

#include "zf_common_headfile.h"

#define Limit_Min_Max(data, min, max) (((data) > (max)) ? (max) : (((data) < (min)) ? (min) : (data)))

// 左轮编码器定义
#define ENCODER_R_LSB_PIN (QTIMER1_ENCODER1_CH1_C0) // A 相对应的引脚
#define ENCODER_R_DIR_PIN (QTIMER1_ENCODER1_CH2_C1) // B 相对应的引脚

// 右轮编码器定义
#define ENCODER_L_LSB_PIN (QTIMER1_ENCODER2_CH1_C2)  // A 相对应的引脚
#define ENCODER_L_DIR_PIN (QTIMER1_ENCODER2_CH2_C24) // B 相对应的引脚

// 左右电机引脚定义
#define L_MOTOR_DIR_PIN (D2)
#define L_MOTOR_PWM_PIN (PWM2_MODULE3_CHB_D3)

#define R_MOTOR_DIR_PIN (C10)
#define R_MOTOR_PWM_PIN (PWM2_MODULE2_CHB_C11)

// 电机控制参数定义
#define MOTOR_FREQ 17000
#define MOTOR_MAX_SPEED 3000

extern int16 l_speed_now;    // 左轮当前速度
extern int16 r_speed_now;    // 右轮当前速度
extern int16 l_speed_target; // 左轮目标速度
extern int16 r_speed_target; // 右轮目标速度
extern int16 differential;   // 后轮差速值

void motor_init(void);
void motor_get(void);
void motor_set(int16 l_speed, int16 r_speed);

void Speed_difference_calculate(int16 diff_base);
int16 Distance_Measure(void);
#endif
