#include "control.h"
#include "pid.h"

float Angle_Out; // 角度PWM输出
float Speed_Out; // 速度PWM输出
float Current_Out; // 电流PWM输出

/**********************
角度环：PID控制

输入：角度误差
输出：角度环输出(控制力矩)
**********************/
float Angle_Control(float Angle_Err)
{
    int PWM_Out; // PWM输出

    pid_angle.err = Angle_Err; // 更新角度环的当前误差

    pid_angle.integral += pid_angle.err; // 计算积分项

    // PID公式计算输出
    PWM_Out = pid_angle.Kp * pid_angle.err + 
              pid_angle.Ki * pid_angle.integral + 
              pid_angle.Kd * (pid_angle.err - pid_angle.err_last);

    // 积分限幅，防止积分项过大
    pid_angle.integral = pid_angle.integral > 2000 ? 2000 : 
                         (pid_angle.integral < (-2000) ? (-2000) : pid_angle.integral);

    pid_angle.err_last = pid_angle.err; // 保存当前误差值

    return PWM_Out; // 返回PWM输出
}

/**********************
速度环：PID控制

输入：速度误差
输出：速度环输出(控制力矩)
**********************/
float Speed_Control(float Speed_Err)
{
    int PWM_Out; // PWM输出

    pid_speed.err = Speed_Err; // 更新速度环的当前误差

    pid_speed.integral += pid_speed.err; // 计算积分项

    // PID公式计算输出
    PWM_Out = pid_speed.Kp * pid_speed.err + 
              pid_speed.Ki * pid_speed.integral + 
              pid_speed.Kd * (pid_speed.err - pid_speed.err_last);

    // 积分限幅，防止积分项过大
    pid_speed.integral = pid_speed.integral > 3000 ? 3000 : 
                         (pid_speed.integral < (-3000) ? (-3000) : pid_speed.integral);

    pid_speed.err_last = pid_speed.err; // 保存当前误差值

    return PWM_Out; // 返回PWM输出
}

/**********************
电流环：PID控制

输入：电流误差
输出：电流环输出(控制力矩)
**********************/
float Current_Control(float Current_Err)
{
    int PWM_Out; // PWM输出

    pid_current.err = Current_Err; // 更新电流环的当前误差

    pid_current.integral += pid_current.err; // 计算积分项

    // PID公式计算输出
    PWM_Out = pid_current.Kp * pid_current.err + 
              pid_current.Ki * pid_current.integral + 
              pid_current.Kd * (pid_current.err - pid_current.err_last);

    // 积分限幅，防止积分项过大
    pid_current.integral = pid_current.integral > 3000 ? 3000 : 
                           (pid_current.integral < (-3000) ? (-3000) : pid_current.integral);

    pid_current.err_last = pid_current.err; // 保存当前误差值

    return PWM_Out; // 返回PWM输出
}

/**********************
限幅函数：(限制力矩)
**********************/
void Moment_limiting(void)
{
    if (Angle_Out > 6)
        Angle_Out = 6; // 角度环力矩输出限幅
    if (Angle_Out < -6)
        Angle_Out = -6;
    if (Speed_Out > 6)
        Speed_Out = 6; // 速度环力矩输出限幅
    if (Speed_Out < -6)
        Speed_Out = -6;
    if (Current_Out > 6)
        Current_Out = 6; // 电流环力矩输出限幅
    if (Current_Out < -6)
        Current_Out = -6;
}
