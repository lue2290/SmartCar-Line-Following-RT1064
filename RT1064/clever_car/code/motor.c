#include "motor.h"

// 全局变量定义
int16 l_speed_now = 0;    // 左轮当前速度
int16 r_speed_now = 0;    // 右轮当前速度
int16 l_speed_target = 0; // 左轮目标速度
int16 r_speed_target = 0; // 右轮目标速度
int16 differential = 0;   // 后轮差速值

void motor_init(void)
{
    //  PWM初始化
    gpio_init(L_MOTOR_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(L_MOTOR_PWM_PIN, MOTOR_FREQ, 0);

    gpio_init(R_MOTOR_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(R_MOTOR_PWM_PIN, MOTOR_FREQ, 0);

    // 编码器初始化
    encoder_dir_init(QTIMER1_ENCODER1, ENCODER_R_LSB_PIN, ENCODER_R_DIR_PIN); /// 右轮
    encoder_dir_init(QTIMER1_ENCODER2, ENCODER_L_LSB_PIN, ENCODER_L_DIR_PIN); /// 左轮
}

void motor_get(void)
{
    // 获取右编码器脉冲
    r_speed_now = encoder_get_count(QTIMER1_ENCODER1);
    encoder_clear_count(QTIMER1_ENCODER1);

    // 获取左编码器脉冲
    l_speed_now = -encoder_get_count(QTIMER1_ENCODER2); // 正转脉冲为负
    encoder_clear_count(QTIMER1_ENCODER2);
}

void motor_set(int16 l_speed, int16 r_speed)
{
    // 左轮速度设置
    if (l_speed >= 0)
    {
        l_speed = l_speed > MOTOR_MAX_SPEED ? MOTOR_MAX_SPEED : l_speed;
        gpio_set_level(L_MOTOR_DIR_PIN, GPIO_HIGH);
        pwm_set_duty(L_MOTOR_PWM_PIN, l_speed);
    }
    else
    {
        l_speed = l_speed < -MOTOR_MAX_SPEED ? -MOTOR_MAX_SPEED : l_speed;
        gpio_set_level(L_MOTOR_DIR_PIN, GPIO_LOW);
        pwm_set_duty(L_MOTOR_PWM_PIN, -l_speed);
    }
    // 右轮速度设置
    if (r_speed >= 0)
    {
        r_speed = r_speed > MOTOR_MAX_SPEED ? MOTOR_MAX_SPEED : r_speed;
        gpio_set_level(R_MOTOR_DIR_PIN, GPIO_HIGH);
        pwm_set_duty(R_MOTOR_PWM_PIN, r_speed);
    }
    else
    {
        r_speed = r_speed < -MOTOR_MAX_SPEED ? -MOTOR_MAX_SPEED : r_speed;
        gpio_set_level(R_MOTOR_DIR_PIN, GPIO_LOW);
        pwm_set_duty(R_MOTOR_PWM_PIN, -r_speed);
    }
}

void Speed_difference_calculate(int16 diff_base)
{
    float k_diff = 21; // 差速系数5  65  24

    differential = k_diff * diff_base;
}

int16 Distance_Measure(void)
{
    int16 distance;

    distance = (l_speed_now + r_speed_now) / 2;

    return distance;
}
