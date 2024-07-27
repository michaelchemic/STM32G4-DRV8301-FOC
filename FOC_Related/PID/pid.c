/*
 * 文件名: pid.c
 * 类型：debug版本测试代码
 * 作者: MichaelKing
 * 日期: 2024-06-26
 * 版本: 1.0
 * 描述: 电机的速度环，距离环，电流环的pid控制器。
 *
 * 历史记录:
 * 版本 1.0 - 2024-06-26 - MichaelKing
 *   初始版本。
 */
#include "pid.h"

pid pid_angle;   // 定义角度环pid
pid pid_speed;   // 定义速度环pid
pid pid_current; // 定义电流环pid

/**********************
PID配置函数:
**********************/
void PID_init() // 初始化pid变量
{
    // 位置环pid
    pid_angle.Set = 0.0f;      // 设置目标值初始化为0
    pid_angle.Actual = 0.0f;   // 实际值初始化为0
    pid_angle.err = 0.0f;      // 当前误差初始化为0
    pid_angle.err_last = 0.0f; // 上次误差初始化为0
    pid_angle.voltage = 0.0f;  // 输出电压初始化为0
    pid_angle.integral = 0.0f; // 积分项初始化为0
    pid_angle.Kp = 0.05f;      // 位置环比例系数初始化为0.043
    pid_angle.Ki = 0.0005f;    // 位置环积分系数初始化为0.0005
    pid_angle.Kd = 0.25f;      // 位置环微分系数初始化为0.22
//    
//    // 速度环pid，AS5600编码器
//    pid_speed.Set = 0.0f;      // 设置目标值初始化为0
//    pid_speed.Actual = 0.0f;   // 实际值初始化为0
//    pid_speed.err = 0.0f;      // 当前误差初始化为0
//    pid_speed.err_last = 0.0f; // 上次误差初始化为0
//    pid_speed.voltage = 0.0f;  // 输出电压初始化为0
//    pid_speed.integral = 0.0f; // 积分项初始化为0
//    pid_speed.Kp = 0.0001f;     // 速度环比例系数初始化为0.11
//    pid_speed.Ki = 0.0005f;    // 速度环积分系数初始化为0.005
//    pid_speed.Kd = 0.00001f;   // 速度环微分系数初始化为0.0008
//    
        // 速度环pid，ABZ编码器
    pid_speed.Set = 0.0f;      // 设置目标值初始化为0
    pid_speed.Actual = 0.0f;   // 实际值初始化为0
    pid_speed.err = 0.0f;      // 当前误差初始化为0
    pid_speed.err_last = 0.0f; // 上次误差初始化为0
    pid_speed.voltage = 0.0f;  // 输出电压初始化为0
    pid_speed.integral = 0.0f; // 积分项初始化为0
    pid_speed.Kp = 0.002f;     // 速度环比例系数初始化为0.11
    pid_speed.Ki = 0.0005f;    // 速度环积分系数初始化为0.005
    pid_speed.Kd = 0.00001f;   // 速度环微分系数初始化为0.0008

    // 电流环pid
    pid_current.Set = 0.0f;      // 设置目标值初始化为0
    pid_current.Actual = 0.0f;   // 实际值初始化为0
    pid_current.err = 0.0f;      // 当前误差初始化为0
    pid_current.err_last = 0.0f; // 上次误差初始化为0
    pid_current.voltage = 0.0f;  // 输出电压初始化为0
    pid_current.integral = 0.0f; // 积分项初始化为0
    pid_current.Kp = 0.045f;     // 电流环比例系数初始化为0.03
    pid_current.Ki = 0.005f;     // 电流环积分系数初始化为0.001
    pid_current.Kd = 0.000f;     // 电流环微分系数初始化为0.0008
}
