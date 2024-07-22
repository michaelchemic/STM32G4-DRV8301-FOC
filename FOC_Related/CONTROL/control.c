#include "control.h"
#include "pid.h"

float Angle_Out; // �Ƕ�PWM���
float Speed_Out; // �ٶ�PWM���
float Current_Out; // ����PWM���

/**********************
�ǶȻ���PID����

���룺�Ƕ����
������ǶȻ����(��������)
**********************/
float Angle_Control(float Angle_Err)
{
    int PWM_Out; // PWM���

    pid_angle.err = Angle_Err; // ���½ǶȻ��ĵ�ǰ���

    pid_angle.integral += pid_angle.err; // ���������

    // PID��ʽ�������
    PWM_Out = pid_angle.Kp * pid_angle.err + 
              pid_angle.Ki * pid_angle.integral + 
              pid_angle.Kd * (pid_angle.err - pid_angle.err_last);

    // �����޷�����ֹ���������
    pid_angle.integral = pid_angle.integral > 2000 ? 2000 : 
                         (pid_angle.integral < (-2000) ? (-2000) : pid_angle.integral);

    pid_angle.err_last = pid_angle.err; // ���浱ǰ���ֵ

    return PWM_Out; // ����PWM���
}

/**********************
�ٶȻ���PID����

���룺�ٶ����
������ٶȻ����(��������)
**********************/
float Speed_Control(float Speed_Err)
{
    int PWM_Out; // PWM���

    pid_speed.err = Speed_Err; // �����ٶȻ��ĵ�ǰ���

    pid_speed.integral += pid_speed.err; // ���������

    // PID��ʽ�������
    PWM_Out = pid_speed.Kp * pid_speed.err + 
              pid_speed.Ki * pid_speed.integral + 
              pid_speed.Kd * (pid_speed.err - pid_speed.err_last);

    // �����޷�����ֹ���������
    pid_speed.integral = pid_speed.integral > 3000 ? 3000 : 
                         (pid_speed.integral < (-3000) ? (-3000) : pid_speed.integral);

    pid_speed.err_last = pid_speed.err; // ���浱ǰ���ֵ

    return PWM_Out; // ����PWM���
}

/**********************
��������PID����

���룺�������
��������������(��������)
**********************/
float Current_Control(float Current_Err)
{
    int PWM_Out; // PWM���

    pid_current.err = Current_Err; // ���µ������ĵ�ǰ���

    pid_current.integral += pid_current.err; // ���������

    // PID��ʽ�������
    PWM_Out = pid_current.Kp * pid_current.err + 
              pid_current.Ki * pid_current.integral + 
              pid_current.Kd * (pid_current.err - pid_current.err_last);

    // �����޷�����ֹ���������
    pid_current.integral = pid_current.integral > 3000 ? 3000 : 
                           (pid_current.integral < (-3000) ? (-3000) : pid_current.integral);

    pid_current.err_last = pid_current.err; // ���浱ǰ���ֵ

    return PWM_Out; // ����PWM���
}

/**********************
�޷�������(��������)
**********************/
void Moment_limiting(void)
{
    if (Angle_Out > 6)
        Angle_Out = 6; // �ǶȻ���������޷�
    if (Angle_Out < -6)
        Angle_Out = -6;
    if (Speed_Out > 6)
        Speed_Out = 6; // �ٶȻ���������޷�
    if (Speed_Out < -6)
        Speed_Out = -6;
    if (Current_Out > 6)
        Current_Out = 6; // ��������������޷�
    if (Current_Out < -6)
        Current_Out = -6;
}
