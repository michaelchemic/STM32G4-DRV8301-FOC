#include "tim.h"
#include "ABZ.h"
#include "foc.h"
#include "math.h"
#include "LowPass_Filter.h"

#define RAD_TO_DEG (57.2957795131f) // 弧度转角度常量
#define FULL_ROTATION_RAD (6.28318530718f) // 2π

extern int DIR; // 无刷电机纠偏旋转方向

//此驱动按照AS5600驱动中的函数编写

// ABZ 正交编码器的每转脉冲数
#define ENCODER_PULSES_PER_REV 4096

// 定时器周期 (秒)
#define TIMER_PERIOD 1.0f

int realCnt = 0; // 真实编码器计数值

ABZ_Encoder encoderDef; //encoderDef结构体声明

// 变量声明
volatile int32_t ABZ_pulse_count = 0;
volatile int32_t ABZ_prev_pulse_count = 0;
volatile float ABZ_speed = 0.0f;
volatile float ABZ_angle = 0.0f;

/////////////////////////////////////////////////////////////////////

//此处适用的定时器时间戳计算速度
float ABZ_full_rotations; // 当前旋转圈数

unsigned long ABZ_angle_prev_ts;     // 上次的运行时间
unsigned long ABZ_vel_angle_prev_ts; // 上次的运行时间

float ABZ_angle_prev_Velocity;     // 上次角度(用于速度环)
float ABZ_vel_angle_prev_Velocity; // 上次角度(用于速度环)

float ABZ_encoder_count = 0;      // 读取当前编码器计数值
float ABZ_prev_encoder_count = 0; // 读取上次编码器计数值


void TIM2_M1_ABZ(void)
{
    // 启动编码器模式
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    // 启动索引中断,检测到ABZ编码器Z信号就重置计数器。
    HAL_TIMEx_EnableEncoderIndex(&htim2);
}

void TIM3_M2_ABZ(void)
{
    // 启动编码器模式
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    // 启动索引中断,检测到ABZ编码器Z信号就重置计数器。
    HAL_TIMEx_EnableEncoderIndex(&htim3);
}

/**
 * @brief 编码器调试函数
 * 
 */
void Count_M1_ABZ(void)
{
    // Debug：读取编码器计数值
    volatile int32_t encoder_count = __HAL_TIM_GET_COUNTER(&htim2);
}

void Count_M2_ABZ(void)
{
    // Debug：读取编码器计数值
    volatile int32_t encoder_count = __HAL_TIM_GET_COUNTER(&htim3);
}

// // 电机速度和角度计算
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM3)
//     {
//         // 获取当前脉冲计数
//         ABZ_pulse_count = __HAL_TIM_GET_COUNTER(&htim3);

//         // 计算脉冲增量
//         int32_t pulse_delta = ABZ_pulse_count - ABZ_prev_pulse_count;

//         // 计算速度 (PPS: 脉冲每秒)
//         // 假设定时器周期为1秒，则速度为脉冲增量
//         ABZ_speed = (float)pulse_delta / TIMER_PERIOD;

//         // 计算角度 (单位：度)
//         // 角度 = 当前计数值 / 每转脉冲数 * 360度
//         ABZ_angle = (float)((ABZ_pulse_count % ENCODER_PULSES_PER_REV + ENCODER_PULSES_PER_REV) % ENCODER_PULSES_PER_REV) / ENCODER_PULSES_PER_REV * 360.0f;

//         // 更新上次脉冲计数
//         ABZ_prev_pulse_count = ABZ_pulse_count;

//         // 清除定时器更新中断标志
//         __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
//     }
// }

//定时器中断方法计算速度
// int UpdateEncoderCnt(void)
// {
//     // 启动编码器模式
//     HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

//     // 启动索引中断
//     HAL_TIMEx_EnableEncoderIndex(&htim3);

//     // 获取电机读数
//     encoderDef.cnt = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);

//     // 计算电机偏移量
//     encoderDef.incCnt = ModifyIncCnt(encoderDef.cnt - encoderDef.preCnt);
//     // 更新上次脉冲计数
//     encoderDef.preCnt = encoderDef.cnt;

//     // 获取真实的编码器计数值
//     realCnt = encoderDef.cnt - encoderDef.offsetCnt;

//     // 当计数值为负数时，加上一圈的计数数
//     while (realCnt < 0)
//     {
//         realCnt += ENCODER_PULSES_PER_REV * 4;
//     }

//     // 计算真实的角度值
//     encoderDef.angle = realCnt * 2 * PI / ENCODER_PULSES_PER_REV / 4;

//     // 计算电角度
//     encoderDef.elecAngle = _normalizeAngle(encoderDef.angle);

//     // 返回编码器读数
//     return encoderDef.cnt;
// }

// int16_t ModifyIncCnt(int16_t delta)
// {
//     // 如果增量超过了一半计数范围，说明是溢出，需要进行调整
//     if (delta > ENCODER_HALF_COUNT)
//     {
//         delta -= (ENCODER_MAX_COUNT + 1);
//     }
//     else if (delta < -ENCODER_HALF_COUNT)
//     {
//         delta += (ENCODER_MAX_COUNT + 1);
//     }
//     return delta;
// }

///////////////////////////////////////////////////////////////////////////////

// 把原始值解算成编码器360°值
//float ABZ_GetAngle360(void)
//{
//    ABZ_encoder_count = __HAL_TIM_GET_COUNTER(&htim3);
//    float ABZ_raw_angle = ABZ_encoder_count;
//    return ABZ_raw_angle * 0.08789f; // 将原始值转换为0-360°的角度,4096*0.08789≈360°
//}

    float debug_ABZ_encoder_count;//debug 参数
// 读取磁编码器归一化弧度值:(0-6.28)
float ABZ_GetAngle2PI(void)
{
    //读取脉冲数
    ABZ_encoder_count = __HAL_TIM_GET_COUNTER(&htim3);
    //debug 参数
     debug_ABZ_encoder_count = ABZ_encoder_count * 0.08789f / 57.32484f;

    return ABZ_encoder_count * 0.08789f / 57.32484f; // 将原始值转换为0-2π的弧度
}


 float ABZ_val; //Debug：当前角度
float ABZ_current_angle;//当前角度变化值
float ABZ_full_rotations_Debug;//累计旋转角度

// 磁编码器弧度制角度累计计算:(0-∞)
float ABZ_GetAngle(void)
{
     ABZ_val = ABZ_GetAngle2PI();                // 获取当前弧度
     ABZ_current_angle = ABZ_val - ABZ_prev_encoder_count; // 计算角度变化

    // 计算旋转的圈数
    // 通过判断角度变化是否大于80%的一圈(0.8f*6.28318530718f)来判断是否发生了溢出
    // 如果发生了溢出,则将full_rotations增加1(如果d_angle小于0)或减少1(如果d_angle大于0)
    if (fabs(ABZ_current_angle) > (0.8f * 6.28318530718f))
        ABZ_full_rotations += (ABZ_current_angle > 0) ? -1 : 1; // 根据角度变化方向调整旋转圈数

    ABZ_prev_encoder_count = ABZ_val;                                               // 更新上次角度
    ABZ_full_rotations_Debug=ABZ_full_rotations * 6.28318530718f + ABZ_prev_encoder_count;
    return (float)ABZ_full_rotations * 6.28318530718f + ABZ_prev_encoder_count; // 返回累计角度
}


 float ABZ_Ts, ABZvel = 0.0f; // Ts为采样时间，vel为速度

// 磁编码器速度计算:(0-∞)
float ABZ_GetVelocity(void)
{
    //float Ts, vel = 0.0f; // Ts为采样时间，vel为速度

    // 计算采样时间
    ABZ_angle_prev_ts = SysTick->VAL; // 获取当前时间戳

    if (ABZ_angle_prev_ts < ABZ_vel_angle_prev_ts)
        ABZ_Ts = (float)(ABZ_vel_angle_prev_ts - ABZ_angle_prev_ts); // 计算时间差
    else
        ABZ_Ts = (float)(0xFFFFFF - ABZ_angle_prev_ts + ABZ_vel_angle_prev_ts); // 处理时间戳溢出情况

    // 快速修复微小溢出
    if (ABZ_Ts == 0 || ABZ_Ts > 0.5f)
        ABZ_Ts = 1e-3f; // 防止采样时间为0或过大

    ABZ_angle_prev_Velocity = ABZ_GetAngle();                           // 获取当前角度
    ABZvel = (ABZ_angle_prev_Velocity - ABZ_vel_angle_prev_Velocity) / ABZ_Ts; // 计算速度
    ABZ_vel_angle_prev_Velocity = ABZ_angle_prev_Velocity;              // 更新上次角度
    ABZ_vel_angle_prev_ts = ABZ_angle_prev_ts;                          // 更新上次时间戳

    return ABZvel; // 返回速度
}

float ABZ_vel_M0_flit;
// 磁编码器速度低通滤波计算:(0-∞)
float ABZ_Get_Speed(void)
{
    float vel_M0_ori = ABZ_GetVelocity(); // 速度原始数据采集
     ABZ_vel_M0_flit = LowPass_Filter(DIR * vel_M0_ori); // 原始数据低通滤波
    return ABZ_vel_M0_flit; // 返回滤波后的速度
}
