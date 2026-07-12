#ifndef _code_direction_h
#define _code_direction_h

#include "zf_common_headfile.h"

#define STEER_MID   775         //舵机中值
#define STEER_LIM   75          //舵机极限
#define STEER_HIM   830          //舵机极限
extern struct PID dpid;

void Set_Steer(int16 duty);
void Direction_pid(struct PID *sptr);

#endif
