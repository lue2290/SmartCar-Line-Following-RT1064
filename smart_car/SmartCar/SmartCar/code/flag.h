#ifndef _flag_h
#define _flag_h

#include "zf_common_headfile.h"

// 周期中断定义
#define PIT_CH (PIT_CH0)        // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY (PIT_IRQn) // 对应周期中断的中断编号

// 标志位结构体
struct FLAG
{
    int8 init_ok;       // 初始化成功标志位
    int8 start;         // 发车成功标志位
    int8 strategy1;     // 速度策略1标志位
    int8 strategy2;     // 速度策略2标志位
    int8 strategy3;     // 速度策略3标志位
    int8 strategy4;     // 速度策略4标志位
    int8 adjust_ok;     // 调参完成标志位
    int8 t_ms;          // 中断基准时间标志位
    int8 msg_collect;   // 信息采集标志位
    int8 speed;         // 速度环标志位
    int8 direction;     // 方向环标志位
    int8 open_check_ku; // 检测库标志位
    int8 stop;          // 停车标志位
    int8 image_show;    // 图像显示标志位
};

extern struct FLAG flag;
void Peripheral_init(void);
void Flag_update(void);

#endif
