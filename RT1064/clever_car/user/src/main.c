#include "zf_common_headfile.h"

uint8 image_copy[MT9V03X_H][MT9V03X_W];
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M); // 不可删除
    debug_init();                  // 调试端口初始化
    system_delay_ms(500);          // 等待主板其他外设上电完成

    // 设置逐飞助手使用DEBUG串口进行收发
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);

    // 初始化逐飞助手示波器的结构体
    seekfree_assistant_oscilloscope_struct oscilloscope_data;
    oscilloscope_data.channel_num = 5;

    tft180_set_dir(TFT180_CROSSWISE); // 需要先横屏 不然显示不下
    tft180_init();
    tft180_show_string(0, 0, "mt9v03x init.");
    while (1)
    {
        if (mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(1000); // 闪灯表示异常
    }
    Peripheral_init();
    flag.init_ok = 1; // 初始化成功
    // Handle_Barn_Out(1); // 左出库
    flag.start = 1;

    ////    r_speed_target = 1000;
    while (1)
    {
        if (mt9v03x_finish_flag)
        {
            if (flag.init_ok)
            {

                if (flag.start && !flag.stop)
                {
//                    
                }

                if (flag.start)
                {
                    //                Datasend();
                    Flag_update();
                }
            }
            // tft180_displayimage03x((const uint8 *)mt9v03x_image, 160, 128);   // 需要注意 直接显示 188*120 分辨率是显示不下的 会直接进入断言报错
            // tft180_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 160, 128, 0);
            mt9v03x_finish_flag = 0;
        }
        if (flag.msg_collect == 10)
        {

            oscilloscope_data.data[0] = l_speed_now;
            oscilloscope_data.data[1] = l_speed_target;
            oscilloscope_data.data[2] = r_speed_now;
            oscilloscope_data.data[3] = r_speed_target;

            oscilloscope_data.data[4] = l_s_pid.out;

            seekfree_assistant_oscilloscope_send(&oscilloscope_data);
            flag.msg_collect = 0;
        }
    }
}
void pit_handler(void)
{
    if (flag.init_ok)
    {
        flag.t_ms = 5;
    }
}