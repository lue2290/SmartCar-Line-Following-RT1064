#include "flag.h"

int count = 0;
int nuuu = 0;
// 结构体定义
struct FLAG flag = {
    .init_ok = 0,
    .start = 0,
    .strategy1 = 0,
    .strategy2 = 0,
    .strategy3 = 0,
    .strategy4 = 0,
    .adjust_ok = 0,
    .t_ms = 0,
    .msg_collect = 0,
    .speed = 0,
    .direction = 0,
    .open_check_ku = 1,
    .stop = 0,
    .image_show = 1};

void Peripheral_init(void)
{
    motor_init();
    servo_init();

    pit_ms_init(PIT_CH, 5);
    interrupt_set_priority(PIT_PRIORITY, 0);
    interrupt_global_enable(0);
    EnableGlobalIRQ(0);
}
void Flag_update(void)
{
    // 调节标志位
    if (flag.t_ms == 5)
    {
        flag.msg_collect += 5;
        flag.speed += 5;
        flag.direction += 5;
        nuuu++;
        flag.t_ms = 0;
    }

    // 信息采集：10ms
    if (flag.msg_collect == 10) // 10
    {
        motor_get(); // 速度值测量
    }

    // 方向调节:10ms
    if (flag.direction == 10)
    {
        PID_Calc_d(&d_pid);
        if (road_type.Cross || road_type.Barn_l_in || road_type.Barn_r_in)
        {
            Limit_Min_Max(d_pid.out, d_pid.last_out - 2, d_pid.last_out + 2);
        }

        servo_set(d_pid.out);

        flag.direction = 0;
    }

    // 速度调节：20ms
    if (flag.speed == 10) // 20
    {
        tft180_show_int(0, 100, huandao_stage, 3);
        PID_Calc_l_s(&l_s_pid); // 左轮pid调节
        PID_Calc_r_s(&r_s_pid); // 右轮pid调节
//        // Speed_difference_calculate(d_pid.out); // 差速计算
        motor_set(l_s_pid.out, r_s_pid.out);
        flag.speed = 0;
    }
    if (nuuu == 100)
    {
				static int nege = 0;
			if(nege){
        l_speed_target = 600;
				nege = 0;
			}else{
			l_speed_target = 300;
				nege=1;
			}
			nuuu= 0;
    }
    // 停车处理
    if (flag.stop)
    {
        // servo_set(75);
        // interrupt_global_disable(); // 关中断
        // while (1)
        // {
        //     if (count < 500)
        //     {
        //         motor_set(1000, 5000); // 关电机
        //         count += 1;
        //     }
        //     else
        //     {
        //         system_delay_ms(300);
        //         motor_set(0, 0);
        //     }
        // }
    }
}
