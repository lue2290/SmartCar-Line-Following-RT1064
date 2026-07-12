#include "state.h"

/************************************************
函数名：Speed_strategy_choose
功  能：速度策略选择
参  数：void
返回值：void
************************************************/
int count = 0;
void Speed_strategy_choose(void)
{
    // 速度策略1
    speed_type.straight = 3300; // 300  2000
    speed_type.bend = 3300;     // 210  2000
    speed_type.cross = 150;
    speed_type.fork = 200;
    speed_type.cirque = 200;
    speed_type.lean_cross = 150;
    speed_type.ramp = 180;
    speed_type.barn = 3000; // 160
    speed_type.stop = 0;
    // 方向环pid
    dpid.kp = 5.0; // 5.0
    dpid.ki = 30;  // 35.0 45 30
    dpid.kd = 10;  // 27.0 10  6
    // 速度环pid
    l_spid.kp = 2;   // 左轮31
    l_spid.ki = 0.1; // 0.65
    r_spid.kp = 2;   // 右轮28
    r_spid.ki = 0.1; // 0.65
}

/************************************************
函数名：Peripheral_init
功  能：外设初始化
参  数：void
返回值：void
************************************************/

void Peripheral_init(void)
{
    // 上电延时
    system_delay_ms(500);

    // LCD初始化
    // lcd_init();

    // 初始化摄像头
    // mt9v03x_init();

    // 舵机初始化
    pwm_init(PWM4_MODULE3_CHA_C31, 50, 775);

    // 电机初始化
    gpio_init(B13, GPO, 1, GPI_PULL_UP);
    pwm_init(PWM1_MODULE3_CHB_D1, 17000, 0);
    pwm_init(PWM1_MODULE3_CHA_D0, 17000, 0);
    pwm_init(PWM2_MODULE3_CHB_D3, 17000, 0);
    pwm_init(PWM2_MODULE3_CHA_D2, 17000, 0);

    // 编码器初始化
    qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
    qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
    encoder_quad_init(QTIMER1_ENCOEDER1, QTIMER1_ENCOEDER1_CH1_C0, QTIMER1_ENCOEDER1_CH2_C1);  /// 右轮
    encoder_quad_init(QTIMER1_ENCOEDER2, QTIMER1_ENCOEDER2_CH1_C2, QTIMER1_ENCOEDER2_CH2_C24); /// 左轮

    // 蜂鸣器初始化
    gpio_init(BUZZER_PIN, GPO, 0, PULLUP_100K | SPEED_50MHZ | DSE_R0);
    // gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG);
    // 蓝牙串口初始化
    uart_init(UART_3, 38400, UART3_TX_C8, UART3_RX_C9);
    NVIC_SetPriority(LPUART3_IRQn, 15); // 设置串口中断优先级 范围0-15 越小优先级越高
    // uart_tx_interrupt(UART_3,1);

    // 拨码开关初始化
    gpio_init(SWITCH_KEY1_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN); // 前三
    gpio_init(SWITCH_KEY2_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    gpio_init(SWITCH_KEY3_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    /*
    gpio_init(SWITCH_KEY4_PIN, GPI, 1, GPIO_PIN_CONFIG);//后三
    gpio_init(SWITCH_KEY5_PIN, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(SWITCH_KEY6_PIN, GPI, 1, GPIO_PIN_CONFIG);
    */
    // 按键初始化
    gpio_init(KEY1_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    gpio_init(KEY2_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    gpio_init(KEY3_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    gpio_init(KEY4_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);
    gpio_init(KEY5_PIN, GPI, 1, SPEED_100MHZ | DSE_R0 | PULLDOWN_100K | PULL_EN);

    // 设置定时中断
    // pit_init(PIT_CH0,5);                     //初始化pit外设
    // pit_interrupt_ms(PIT_CH0,10);  //初始化pit通道0 周期
    pit_ms_init(PIT_CH0, 5);
    NVIC_SetPriority(PIT_IRQn, 2); // 设置中断优先级 范围0-15 越小优先级越高 四路PIT共用一个PIT中断函数
    /*
        pit_interrupt_ms(PIT_CH1,1);  //初始化pit通道0 周期
    NVIC_SetPriority(PIT_IRQn,15);  //设置中断优先级 范围0-15 越小优先级越高 四路PIT共用一个PIT中断函数
    */
    EnableGlobalIRQ(0);
}

/************************************************
函数名：Parameter_init
功  能：参数初始化
参  数：void
返回值：void
************************************************/

void Parameter_init(void)
{
    Speed_strategy_choose();
    //    Parameter_eeprom_init();
    //    Parameter_read_eeprom();
    //    Key_scan();
    //    Parameter_write_eeprom();
}

/************************************************
函数名：State_Adjust
功  能：状态调节
参  数：void
返回值：void
************************************************/

void State_Adjust(void)
{
    // 调节标志位
    if (flag.t_ms == 5)
    {
        flag.msg_collect += 5;
        flag.speed += 5;
        flag.direction += 5;

        flag.t_ms = 0;
    }

    // 信息采集：10ms
    if (flag.msg_collect == 10) // 10
    {
        Speed_Measure(); // 速度值测量

        flag.msg_collect = 0;
    }

    // 方向调节:10ms
    if (flag.direction == 10)
    {
        Direction_pid(&dpid);
        if (road_type.Cross || road_type.Barn_l_in || road_type.Barn_r_in)
        {
            Limit_Min_Max(dpid.out, dpid.last_out - 2, dpid.last_out + 2);
        }
        Set_Steer(dpid.out);

        flag.direction = 0;
    }

    // 速度调节：20ms
    if (flag.speed == 10) // 20
    {
        // tft180_show_int(0, 20, length, 3);
        // tft180_show_int(0, 40, r_lose_value, 3);
        // tft180_show_int(0, 60, road_type.Barn_l_in , 3);
        // tft180_show_int(0, 80, road_type.Barn_r_in , 3);
        tft180_show_int(0, 100, huandao_stage, 3);
        L_speed_pid(&l_spid);                                            // 左轮pid调节
        R_speed_pid(&r_spid);                                            // 右轮pid调节
        Speed_difference_calculate(dpid.out);                            // 差速计算
        Set_Motor(l_spid.out - differential, r_spid.out + differential); // l_spid.out-differential,r_spid.out+differential   Set_Motor(l_spid.out, r_spid.out)
        // Set_Motor(1000,1000);

        flag.speed = 0;
    }

    // 停车处理
    if (flag.stop)
    {

        // Set_Motor(-4000,-4000);
        //        systick_delay_ms(STM0, 200);
        Set_Steer(75);
        // system_delay_ms(350);
        // tft180_show_string(0,0,"straight");tft180_show_int(100,0,road_type.straight,3);
        // tft180_show_string(0,1,"bend");tft180_show_int(100,1,road_type.bend,3);
        // tft180_show_string(0,2,"L_Cross");tft180_show_int(100,2,road_type.L_Cross,3);
        // tft180_show_string(0,3,"R_Cross");tft180_show_int(100,3,road_type.R_Cross,3);
        // tft180_show_string(0,4,"LeftCirque");tft180_show_uint(100,4,road_type.LeftCirque,3);
        // tft180_show_string(0,5,"RightCirque");tft180_show_uint(100,5,road_type.RightCirque,3);
        // tft180_show_string(0,6,"Fork");tft180_show_uint(100,6,road_type.Fork,3);
        interrupt_global_disable(); // 关中断
        BUZZER_OFF;

        while (1)
        {
            //            Speed_Measure();//速度值测量
            //            L_speed_pid(&l_spid);//左轮pid调节
            //            R_speed_pid(&r_spid);//右轮pid调节
            if (count < 500)
            {
                Set_Motor(1000, 5000); // 关电机
                count += 1;
            }
            else
            {
                system_delay_ms(300);
                Set_Motor(0, 0);
            }
            //            systick_delay_ms(STM0, 20);
        }
    }
}
