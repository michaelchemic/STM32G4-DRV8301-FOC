#include "AS5600.h"
#include "math.h"
#include "LowPass_Filter.h"
#include "foc.h"

extern int DIR; // 无刷电机纠偏旋转方向

float full_rotations; // 当前旋转圈数

float angle_prev;      // 上次角度(用于位置环)
float vel_angle_prev;  // 上次角度(用于速度环)

float angle_prev_Velocity;       // 上次角度(用于速度环)
float vel_angle_prev_Velocity;   // 上次角度(用于速度环)

unsigned long angle_prev_ts;      // 上次的运行时间
unsigned long vel_angle_prev_ts;  // 上次的运行时间

float raw_angle; // 传感器原始值
float angle;     // 传感器角度缩减值

// 读取0x0c地址原始数据
HAL_StatusTypeDef AS5600_ReadRawAngle(I2C_HandleTypeDef *hi2c1, float *raw_angle)
{
    uint8_t data[2]; // 用于存储读取的数据
    HAL_StatusTypeDef ret; // 存储函数返回状态

    // 从AS5600传感器的0x0c地址读取2字节数据
    ret = HAL_I2C_Mem_Read(hi2c1, AS5600_I2C_ADDR << 1, AS5600_REG_RAW_ANGLE, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK)
    {
        return ret; // 如果读取失败，则返回错误状态
    }

    *raw_angle = (data[0] << 8) | data[1]; // 将读取的数据合成为一个16位数
    return HAL_OK; // 返回成功状态
}

// 读取角度数据
HAL_StatusTypeDef AS5600_ReadAngle(I2C_HandleTypeDef *hi2c1, float *angle)
{
    uint8_t data[2]; // 用于存储读取的数据
    HAL_StatusTypeDef ret; // 存储函数返回状态

    // 从AS5600传感器的0x0e地址读取2字节数据
    ret = HAL_I2C_Mem_Read(hi2c1, AS5600_I2C_ADDR << 1, AS5600_REG_ANGLE, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK)
    {
        return ret; // 如果读取失败，则返回错误状态
    }

    *angle = (data[0] << 8) | data[1]; // 将读取的数据合成为一个16位数
    return HAL_OK; // 返回成功状态
}

// 把原始值解算成编码器360°值
float AS5600_GetAngle360(void)
{
    return raw_angle * 0.08789f; // 将原始值转换为0-360°的角度
}

// 读取磁编码器归一化弧度值:(0-6.28)
float AS5600_GetAngle2PI(void)
{
    return raw_angle * 0.08789f / 57.32484f; // 将原始值转换为0-2π的弧度
}

// 磁编码器弧度制角度累计计算:(0-∞)
float AS5600_GetAngle(void)
{
    float val = AS5600_GetAngle2PI(); // 获取当前角度
    float d_angle = val - angle_prev; // 计算角度变化

    // 计算旋转的圈数
    // 通过判断角度变化是否大于80%的一圈(0.8f*6.28318530718f)来判断是否发生了溢出
    // 如果发生了溢出,则将full_rotations增加1(如果d_angle小于0)或减少1(如果d_angle大于0)
    if (fabs(d_angle) > (0.8f * 6.28318530718f))
        full_rotations += (d_angle > 0) ? -1 : 1; // 根据角度变化方向调整旋转圈数

    angle_prev = val; // 更新上次角度
    return (float)full_rotations * 6.28318530718f + angle_prev; // 返回累计角度
}

// 磁编码器速度计算:(0-∞)
float AS5600_GetVelocity(void)
{
    float Ts, vel = 0.0f; // Ts为采样时间，vel为速度

    // 计算采样时间
    angle_prev_ts = SysTick->VAL; // 获取当前时间戳
    if (angle_prev_ts < vel_angle_prev_ts)
        Ts = (float)(vel_angle_prev_ts - angle_prev_ts); // 计算时间差
    else
        Ts = (float)(0xFFFFFF - angle_prev_ts + vel_angle_prev_ts); // 处理时间戳溢出情况

    // 快速修复微小溢出
    if (Ts == 0 || Ts > 0.5f)
        Ts = 1e-3f; // 防止采样时间为0或过大

    angle_prev_Velocity = AS5600_GetAngle(); // 获取当前角度
    vel = (angle_prev_Velocity - vel_angle_prev_Velocity) / Ts; // 计算速度
    vel_angle_prev_Velocity = angle_prev_Velocity; // 更新上次角度
    vel_angle_prev_ts = angle_prev_ts; // 更新上次时间戳

    return vel; // 返回速度
}

//float AS5600_GetVelocity(void)
//{
//    float Ts, vel = 0.0f; // Ts为采样时间，vel为速度

//    // 获取当前时间戳
//    uint32_t current_ts = SysTick->VAL;

//    // 计算采样时间，SysTick->LOAD的值为系统定时器重装值
//    if (current_ts < vel_angle_prev_ts)
//        Ts = (float)(vel_angle_prev_ts - current_ts) / SysTick->LOAD * 1e-6f; // 计算时间差
//    else
//        Ts = (float)(0xFFFFFF - current_ts + vel_angle_prev_ts) / SysTick->LOAD * 1e-6f; // 处理时间戳溢出情况

//    // 快速修复微小溢出
//    if (Ts == 0 || Ts > 0.5f)
//        Ts = 1e-3f; // 防止采样时间为0或过大

//    // 获取当前角度
//    float current_angle = AS5600_GetAngle();

//    // 计算角度差值，处理角度溢出情况（假设角度范围是0到360度）
//    float angle_diff = current_angle - vel_angle_prev_Velocity;
//    if (angle_diff > 180.0f)
//        angle_diff -= 360.0f;
//    else if (angle_diff < -180.0f)
//        angle_diff += 360.0f;

//    // 计算速度
//    vel = angle_diff / Ts;

//    // 更新上次角度和时间戳
//    vel_angle_prev_Velocity = current_angle;
//    vel_angle_prev_ts = current_ts;

//    return vel; // 返回速度
//}


// 磁编码器速度低通滤波计算:(0-∞)
float AS5600_Get_Speed(void)
{
    float vel_M0_ori = AS5600_GetVelocity(); // 速度原始数据采集
    float vel_M0_flit = LowPass_Filter(DIR * vel_M0_ori); // 原始数据低通滤波
    return vel_M0_flit; // 返回滤波后的速度
}
