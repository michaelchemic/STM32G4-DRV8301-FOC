/*
 * �ļ���: pid.c
 * ���ͣ�debug�汾���Դ���
 * ����: MichaelKing
 * ����: 2024-06-26
 * �汾: 1.0
 * ����: ������ٶȻ������뻷����������pid��������
 *
 * ��ʷ��¼:
 * �汾 1.0 - 2024-06-26 - MichaelKing
 *   ��ʼ�汾��
 */
#include "pid.h"

pid pid_angle;   // ����ǶȻ�pid
pid pid_speed;   // �����ٶȻ�pid
pid pid_current; // ���������pid

/**********************
PID���ú���:
**********************/
void PID_init() // ��ʼ��pid����
{
    // λ�û�pid
    pid_angle.Set = 0.0f;      // ����Ŀ��ֵ��ʼ��Ϊ0
    pid_angle.Actual = 0.0f;   // ʵ��ֵ��ʼ��Ϊ0
    pid_angle.err = 0.0f;      // ��ǰ����ʼ��Ϊ0
    pid_angle.err_last = 0.0f; // �ϴ�����ʼ��Ϊ0
    pid_angle.voltage = 0.0f;  // �����ѹ��ʼ��Ϊ0
    pid_angle.integral = 0.0f; // �������ʼ��Ϊ0
    pid_angle.Kp = 0.05f;      // λ�û�����ϵ����ʼ��Ϊ0.043
    pid_angle.Ki = 0.0005f;    // λ�û�����ϵ����ʼ��Ϊ0.0005
    pid_angle.Kd = 0.25f;      // λ�û�΢��ϵ����ʼ��Ϊ0.22
//    
//    // �ٶȻ�pid��AS5600������
//    pid_speed.Set = 0.0f;      // ����Ŀ��ֵ��ʼ��Ϊ0
//    pid_speed.Actual = 0.0f;   // ʵ��ֵ��ʼ��Ϊ0
//    pid_speed.err = 0.0f;      // ��ǰ����ʼ��Ϊ0
//    pid_speed.err_last = 0.0f; // �ϴ�����ʼ��Ϊ0
//    pid_speed.voltage = 0.0f;  // �����ѹ��ʼ��Ϊ0
//    pid_speed.integral = 0.0f; // �������ʼ��Ϊ0
//    pid_speed.Kp = 0.0001f;     // �ٶȻ�����ϵ����ʼ��Ϊ0.11
//    pid_speed.Ki = 0.0005f;    // �ٶȻ�����ϵ����ʼ��Ϊ0.005
//    pid_speed.Kd = 0.00001f;   // �ٶȻ�΢��ϵ����ʼ��Ϊ0.0008
//    
        // �ٶȻ�pid��ABZ������
    pid_speed.Set = 0.0f;      // ����Ŀ��ֵ��ʼ��Ϊ0
    pid_speed.Actual = 0.0f;   // ʵ��ֵ��ʼ��Ϊ0
    pid_speed.err = 0.0f;      // ��ǰ����ʼ��Ϊ0
    pid_speed.err_last = 0.0f; // �ϴ�����ʼ��Ϊ0
    pid_speed.voltage = 0.0f;  // �����ѹ��ʼ��Ϊ0
    pid_speed.integral = 0.0f; // �������ʼ��Ϊ0
    pid_speed.Kp = 0.002f;     // �ٶȻ�����ϵ����ʼ��Ϊ0.11
    pid_speed.Ki = 0.0005f;    // �ٶȻ�����ϵ����ʼ��Ϊ0.005
    pid_speed.Kd = 0.00001f;   // �ٶȻ�΢��ϵ����ʼ��Ϊ0.0008

    // ������pid
    pid_current.Set = 0.0f;      // ����Ŀ��ֵ��ʼ��Ϊ0
    pid_current.Actual = 0.0f;   // ʵ��ֵ��ʼ��Ϊ0
    pid_current.err = 0.0f;      // ��ǰ����ʼ��Ϊ0
    pid_current.err_last = 0.0f; // �ϴ�����ʼ��Ϊ0
    pid_current.voltage = 0.0f;  // �����ѹ��ʼ��Ϊ0
    pid_current.integral = 0.0f; // �������ʼ��Ϊ0
    pid_current.Kp = 0.045f;     // ����������ϵ����ʼ��Ϊ0.03
    pid_current.Ki = 0.005f;     // ����������ϵ����ʼ��Ϊ0.001
    pid_current.Kd = 0.000f;     // ������΢��ϵ����ʼ��Ϊ0.0008
}
