# 智能车循迹竞赛

基于 NXP i.MX RT1064 (Cortex-M7 @ 600MHz) 的高性能摄像头循迹智能车，用于校内智能车竞赛。采用 MT9V03X 全局快门摄像头采集赛道图像，完整实现图像处理 → 赛道识别 → PID 控制的 10ms 闭环管线。

## 硬件方案

| 部件 | 型号 | 说明 |
|------|------|------|
| 主控 | NXP MIMXRT1064DVL6A | Cortex-M7 @ 600MHz，4MB Flash，1MB RAM |
| 摄像头 | MT9V03X | 全局快门 CMOS，188×120，CSI 接口 + DMA |
| 舵机 | 转向舵机 | 50Hz PWM，中值 700~775µs，范围 ±75 |
| 电机 | 直流电机 × 2 | 17kHz PWM，带正交编码器 |
| 编码器 | QTIMER1 | 2 通道 4 倍频解码 |
| 显示屏 | 1.8寸 TFT | SPI 接口，实时显示摄像头画面 |
| 调试接口 | UART3 @ 38400 | 连接 Seekfree 上位机 |
| 开发板 | Seekfree RT1064 核心板 | 含 SDRAM |

## 引脚分配

| 功能 | 引脚 | 外设 |
|------|------|------|
| 摄像头 CSI | CSI 专用 | CSI + DMA |
| 舵机 PWM | C30/C31 | PWM4 |
| 左电机 PWM | D3 | PWM |
| 右电机 PWM | C11 | PWM |
| 左编码器 | C0, C1 | QTIMER1_ENCODER1 |
| 右编码器 | C2, C24 | QTIMER1_ENCODER2 |
| TFT SPI | B0(SCK), B3(CS), B1(MOSI) | SPI |
| 调试 UART | C8, C9 | UART3 @ 38400 |

## 图像处理管线

```
1. Camera IRQ → Transfer_Camera()          — CSI DMA 传输到帧缓冲
2. Get01change_Dajin()                     — 自适应二值化（OTSU 算法，静态备份阈值 200）
3. Pixle_Filter()                          — 腐蚀滤波，去除椒盐噪声
4. Search_Line(0)                          — 双边边界搜索（从底向上，左/右边界跟踪）
5. Element_Test()                          — 赛道元素分类
6. Element_Handle()                        — 元素专项处理
7. HDPJ_lvbo()                             — 中线滑动平均滤波
8. Calculate_Offset_1()                    — 加权偏移计算（近行权重 0.96→0.47 递减）
9. Blacking()                              — TFT 显示调试叠加层
```

## 赛道元素识别

| 元素 | 检测方法 | 处理策略 |
|------|---------|---------|
| 直道 | 双边存在，宽度稳定 | 正常 PD 控制 |
| 弯道 | 单边丢失，曲率突变 | 加大 PD 增益 |
| 十字 | 双边同时丢失（有界） | 降速至 150，限幅转向 |
| 斜十字 | 斜向丢边模式 | 专项进入/退出序列 |
| 左/右环岛 | 曲率 + 长时间丢边 | 环岛遍历状态机 |
| 岔路 | 双边分叉 | 路径选择策略 |
| 车库 | 特定缺口模式 | 降速入库机动 |
| 坡道 | 高度变化 | 保速不转向 |

## 控制策略

### 方向 PD（非线性增益）

```
error = -(偏移像素 / 12)
real_kp = error² / Ki + Kp           ← 误差大时增益自动增大
output = real_kp × error + Kd × (error - last_error)
output = 0.8 × output + 0.2 × last_output     ← 低通滤波
servo = clamp(mid + output, mid-75, mid+75)
```

参数：`Kp=5.0, Ki=30, Kd=10`

### 速度 PI（左右独立 + 差速转向）

| 参数 | 左轮 | 右轮 |
|------|:----:|:----:|
| Kp | 8.0 | 3.0 |
| Ki | 3.0 | 0.0 |
| Kd | 5.0 | 0.0 |

差速：`diff = K_diff × direction_out`，K_diff=21

### 赛道限速

| 元素 | 目标速度 |
|------|:-------:|
| 直道 | 3300 |
| 弯道 | 3300 |
| 十字 | 150 |
| 岔路 | 200 |
| 环岛 | 200 |
| 车库 | 3000 |

## 项目结构

```
SmartCar/
├── RT1064/clever_car/             ← 主竞赛固件（22 个算法源文件）
│   ├── code/                      camera/direction/pid/motor/servo/
│   ├── user/                      main.c + isr.c
│   ├── iar/                       IAR EWARM 工程
│   └── mdk/                       Keil MDK 工程
├── RT1064Example/                 ← 官方外设例程（17 个演示项目）
├── rt_keil/                       ← 摄像头 + TFT 显示 Demo
├── smart_car/                     ← 旧版备份
└── seekfree_assistant/            ← PC 调参上位机
    ├── 逐飞助手上位机V1.1.2.exe    串口终端 + 虚拟示波器 + 图像查看 + 参数调试
    └── 【文档】说明书/               使用手册 PDF
```

## 调参工具

Seekfree 上位机提供：
- 实时摄像头图像查看（灰度图）
- 4 通道虚拟示波器（速度、误差、PID 输出）
- 8 路参数滑条实时调整
- 串口终端

## 开发环境

| 工具 | 版本 |
|------|------|
| IAR EWARM | v8.32.4+ |
| Keil MDK | v5.24+ |
| SDK | NXP SDK 2.12 + Seekfree Library v3.7.1 |
| 调试器 | DAP-Link / J-Link V9+ |

## 使用

1. 用 IAR 或 Keil 打开工程文件，编译
2. 通过 DAP-Link 下载到 RT1064 核心板
3. 上电后 TFT 显示摄像头画面
4. 将车放在赛道上，按启动按键开始循迹
