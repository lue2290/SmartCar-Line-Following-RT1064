#include "pid.h"

struct PID l_s_pid = {
    .kp = 2,
    .ki = 0.1,
    .kd = 0,
    .error = 0,
    .last_error = 0,
    .pre_error = 0,
    .out = 0,
    .last_out = 0,
};

struct PID r_s_pid = {
    .kp = 2,
    .ki = 0.1,
    .kd = 0,
    .error = 0,
    .last_error = 0,
    .pre_error = 0,
    .out = 0,
    .last_out = 0,
};

struct PID d_pid = {
    .kp = 2,  // 5
    .ki = 30, // 30
    .kd = 10, // 10
    .error = 0,
    .last_error = 0,
    .pre_error = 0,
    .out = 0,
    .last_out = 0,
};

struct SPEED_TYPE speed_type = {
    .straight = 2000,
    .bend = 2000,
    .cross = 2000,
    .cirque = 2000,
    .fork = 2000,
    .lean_cross = 2000,
    .ramp = 2000,
    .stop = 2000,
    .barn = 2000,
};
// 速度设置
void l_speed_tar_get(void)
{
    l_speed_target = 2000;
}
void r_speed_tar_get(void)
{
    r_speed_target = 2000;
}
// 速度控制
void PID_Calc_l_s(struct PID *pid)
{
    pid->error = l_speed_target - l_speed_now; // 左轮速度偏差计算
    pid->out += pid->kp * (pid->error - pid->last_error) + pid->ki * pid->error;
    pid->last_error = pid->error;
}
// 速度控制
void PID_Calc_r_s(struct PID *pid)
{
    pid->error = r_speed_target - r_speed_now; // 右轮速度偏差计算
    pid->out += pid->kp * (pid->error - pid->last_error) + pid->ki * pid->error;
    pid->last_error = pid->error;
}
// 方向控制
void PID_Calc_d(struct PID *pid)
{
    float real_kp;

    pid->error = -(1.0 * offset) / 12; // 面积法计算的偏差值
    real_kp = 1.0 * (pid->error * pid->error) / pid->ki + pid->kp;
    pid->out = (int16)(real_kp * pid->error + pid->kd * (pid->error - pid->last_error));
    pid->out = Filter_first(pid->out, pid->last_out, 0.2); // 一阶低通滤波
    pid->last_error = pid->error;
    pid->last_out = pid->out;
}

int16 Filter_first(int16 data, int16 last_data, float k)
{
    int16 result;

    result = (int16)((1 - k) * data + k * last_data);

    return result;
}
