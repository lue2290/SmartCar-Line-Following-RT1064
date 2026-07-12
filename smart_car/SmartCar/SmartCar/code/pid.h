#ifndef _pid_h
#define _pid_h

#include "zf_common_headfile.h"

// pid结构体声明
struct PID
{
    float kp;
    float ki;
    float kd;
    float error;
    float last_error;
    float pre_error;
    int16 out;
    int16 last_out;
};

// 速度策略结构体声明
struct SPEED_TYPE
{
    int16 straight;
    int16 bend;
    int16 cross;
    int16 cirque;
    int16 fork;
    int16 lean_cross;
    int16 ramp;
    int16 stop;
    int16 barn;
};

extern struct SPEED_TYPE speed_type; // 目标速度参数结构体定义// 左右轮pid参数结构体定义
extern struct PID l_s_pid;           // 左右轮pid参数结构体定义
extern struct PID r_s_pid;
extern struct PID d_pid; // 方向pid参数结构体定义

void l_speed_error(void);
void PID_Calc_l_s(struct PID *pid);

void r_speed_error(void);
void PID_Calc_r_s(struct PID *pid);

int16 Filter_first(int16 data, int16 last_data, float k);
void PID_Calc_d(struct PID *pid);

#endif
