#include "AS5600.h"
#include "math.h"
#include "LowPass_Filter.h"
#include "foc.h"

extern int DIR; // ��ˢ�����ƫ��ת����

float full_rotations; // ��ǰ��תȦ��

float angle_prev;      // �ϴνǶ�(����λ�û�)
float vel_angle_prev;  // �ϴνǶ�(�����ٶȻ�)

float angle_prev_Velocity;       // �ϴνǶ�(�����ٶȻ�)
float vel_angle_prev_Velocity;   // �ϴνǶ�(�����ٶȻ�)

unsigned long angle_prev_ts;      // �ϴε�����ʱ��
unsigned long vel_angle_prev_ts;  // �ϴε�����ʱ��

float raw_angle; // ������ԭʼֵ
float angle;     // �������Ƕ�����ֵ

// ��ȡ0x0c��ַԭʼ����
HAL_StatusTypeDef AS5600_ReadRawAngle(I2C_HandleTypeDef *hi2c1, float *raw_angle)
{
    uint8_t data[2]; // ���ڴ洢��ȡ������
    HAL_StatusTypeDef ret; // �洢��������״̬

    // ��AS5600��������0x0c��ַ��ȡ2�ֽ�����
    ret = HAL_I2C_Mem_Read(hi2c1, AS5600_I2C_ADDR << 1, AS5600_REG_RAW_ANGLE, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK)
    {
        return ret; // �����ȡʧ�ܣ��򷵻ش���״̬
    }

    *raw_angle = (data[0] << 8) | data[1]; // ����ȡ�����ݺϳ�Ϊһ��16λ��
    return HAL_OK; // ���سɹ�״̬
}

// ��ȡ�Ƕ�����
HAL_StatusTypeDef AS5600_ReadAngle(I2C_HandleTypeDef *hi2c1, float *angle)
{
    uint8_t data[2]; // ���ڴ洢��ȡ������
    HAL_StatusTypeDef ret; // �洢��������״̬

    // ��AS5600��������0x0e��ַ��ȡ2�ֽ�����
    ret = HAL_I2C_Mem_Read(hi2c1, AS5600_I2C_ADDR << 1, AS5600_REG_ANGLE, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK)
    {
        return ret; // �����ȡʧ�ܣ��򷵻ش���״̬
    }

    *angle = (data[0] << 8) | data[1]; // ����ȡ�����ݺϳ�Ϊһ��16λ��
    return HAL_OK; // ���سɹ�״̬
}

// ��ԭʼֵ����ɱ�����360��ֵ
float AS5600_GetAngle360(void)
{
    return raw_angle * 0.08789f; // ��ԭʼֵת��Ϊ0-360��ĽǶ�
}

// ��ȡ�ű�������һ������ֵ:(0-6.28)
float AS5600_GetAngle2PI(void)
{
    return raw_angle * 0.08789f / 57.32484f; // ��ԭʼֵת��Ϊ0-2�еĻ���
}

// �ű����������ƽǶ��ۼƼ���:(0-��)
float AS5600_GetAngle(void)
{
    float val = AS5600_GetAngle2PI(); // ��ȡ��ǰ�Ƕ�
    float d_angle = val - angle_prev; // ����Ƕȱ仯

    // ������ת��Ȧ��
    // ͨ���жϽǶȱ仯�Ƿ����80%��һȦ(0.8f*6.28318530718f)���ж��Ƿ��������
    // ������������,��full_rotations����1(���d_angleС��0)�����1(���d_angle����0)
    if (fabs(d_angle) > (0.8f * 6.28318530718f))
        full_rotations += (d_angle > 0) ? -1 : 1; // ���ݽǶȱ仯���������תȦ��

    angle_prev = val; // �����ϴνǶ�
    return (float)full_rotations * 6.28318530718f + angle_prev; // �����ۼƽǶ�
}

// �ű������ٶȼ���:(0-��)
float AS5600_GetVelocity(void)
{
    float Ts, vel = 0.0f; // TsΪ����ʱ�䣬velΪ�ٶ�

    // �������ʱ��
    angle_prev_ts = SysTick->VAL; // ��ȡ��ǰʱ���
    if (angle_prev_ts < vel_angle_prev_ts)
        Ts = (float)(vel_angle_prev_ts - angle_prev_ts); // ����ʱ���
    else
        Ts = (float)(0xFFFFFF - angle_prev_ts + vel_angle_prev_ts); // ����ʱ���������

    // �����޸�΢С���
    if (Ts == 0 || Ts > 0.5f)
        Ts = 1e-3f; // ��ֹ����ʱ��Ϊ0�����

    angle_prev_Velocity = AS5600_GetAngle(); // ��ȡ��ǰ�Ƕ�
    vel = (angle_prev_Velocity - vel_angle_prev_Velocity) / Ts; // �����ٶ�
    vel_angle_prev_Velocity = angle_prev_Velocity; // �����ϴνǶ�
    vel_angle_prev_ts = angle_prev_ts; // �����ϴ�ʱ���

    return vel; // �����ٶ�
}

//float AS5600_GetVelocity(void)
//{
//    float Ts, vel = 0.0f; // TsΪ����ʱ�䣬velΪ�ٶ�

//    // ��ȡ��ǰʱ���
//    uint32_t current_ts = SysTick->VAL;

//    // �������ʱ�䣬SysTick->LOAD��ֵΪϵͳ��ʱ����װֵ
//    if (current_ts < vel_angle_prev_ts)
//        Ts = (float)(vel_angle_prev_ts - current_ts) / SysTick->LOAD * 1e-6f; // ����ʱ���
//    else
//        Ts = (float)(0xFFFFFF - current_ts + vel_angle_prev_ts) / SysTick->LOAD * 1e-6f; // ����ʱ���������

//    // �����޸�΢С���
//    if (Ts == 0 || Ts > 0.5f)
//        Ts = 1e-3f; // ��ֹ����ʱ��Ϊ0�����

//    // ��ȡ��ǰ�Ƕ�
//    float current_angle = AS5600_GetAngle();

//    // ����ǶȲ�ֵ������Ƕ�������������Ƕȷ�Χ��0��360�ȣ�
//    float angle_diff = current_angle - vel_angle_prev_Velocity;
//    if (angle_diff > 180.0f)
//        angle_diff -= 360.0f;
//    else if (angle_diff < -180.0f)
//        angle_diff += 360.0f;

//    // �����ٶ�
//    vel = angle_diff / Ts;

//    // �����ϴνǶȺ�ʱ���
//    vel_angle_prev_Velocity = current_angle;
//    vel_angle_prev_ts = current_ts;

//    return vel; // �����ٶ�
//}


// �ű������ٶȵ�ͨ�˲�����:(0-��)
float AS5600_Get_Speed(void)
{
    float vel_M0_ori = AS5600_GetVelocity(); // �ٶ�ԭʼ���ݲɼ�
    float vel_M0_flit = LowPass_Filter(DIR * vel_M0_ori); // ԭʼ���ݵ�ͨ�˲�
    return vel_M0_flit; // �����˲�����ٶ�
}
