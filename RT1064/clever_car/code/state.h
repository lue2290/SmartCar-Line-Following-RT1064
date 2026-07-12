#ifndef _code_state_h
#define _code_state_h

#include "zf_common_headfile.h"

//舵机引脚
#define STEER_PIN PWM4_MODULE3_CHA_C31

//蜂鸣器引脚
#define BUZZER_PIN C27

//按键引脚-0~5：C25,C26,C7,C5,C6
#define KEY1_PIN C25//2
#define KEY2_PIN C26//3
#define KEY3_PIN C7//6
#define KEY4_PIN C5//5
#define KEY5_PIN C6//4

//拨码开关引脚-从上到下
#define SWITCH_KEY1_PIN C11//前三
#define SWITCH_KEY2_PIN C3
#define SWITCH_KEY3_PIN C4
#define SWITCH_KEY4_PIN P20_6//后三
#define SWITCH_KEY5_PIN P20_7
#define SWITCH_KEY6_PIN P20_8


void Peripheral_init(void);
void Parameter_init(void);
void Speed_strategy_choose(void);
void State_Adjust(void);

#endif
