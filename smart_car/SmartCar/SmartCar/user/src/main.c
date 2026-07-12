#include "zf_common_headfile.h"

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M); // 不可删除
    debug_init();                  // 调试端口初始化

    system_delay_ms(500); // 等待主板其他外设上电完成

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
    tft180_show_string(0, 16, "init success.");

    Peripheral_init();
    flag.init_ok = 1; // 初始化成功
    // Handle_Barn_Out(1); // 左出库
    flag.start = 1;
    while (1)
    {
        if (mt9v03x_finish_flag)
        {
            if (flag.init_ok)
            {

                if (flag.start && !flag.stop)
                {
                    Camera_Display();
                }

                if (flag.start)
                {
                    //                Datasend();
                    Flag_update();
                }
            }
            // tft180_displayimage03x((const uint8 *)mt9v03x_image, 160, 128);   // 需要注意 直接显示 188*120 分辨率是显示不下的 会直接进入断言报错
            //            tft180_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 160, 128, 0);
            mt9v03x_finish_flag = 0;
        }
    }

    servo_set(0);
    while (1)
    {
    }
}

void pit_handler(void)
{
    if (flag.init_ok)
    {
        flag.t_ms = 5;
    }
}