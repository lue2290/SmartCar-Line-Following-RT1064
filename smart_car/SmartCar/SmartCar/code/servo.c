#include "servo.h"

void servo_init(void)
{
    pwm_init(SERVO_PWM_PIN, 50, SERVO_MID);
}

void servo_set(int16 duty)
{
    duty = Limit_Min_Max(duty, -SERVO_LIM, SERVO_LIM);
    pwm_set_duty(SERVO_PWM_PIN, SERVO_MID + duty);
}
