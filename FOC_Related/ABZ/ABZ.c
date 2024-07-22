// #include "tim.h"
// #include "ABZ.h"

// void TIM2_M1_ABZ(void)
// {
// // 启动编码器模式
// HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

// // 启动索引中断
// HAL_TIMEx_EnableEncoderIndex(&htim2);

// }

// void TIM2_M2_ABZ(void)
// {
// // 启动编码器模式
// HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

// // 启动索引中断
// HAL_TIMEx_EnableEncoderIndex(&htim3);

// }

// void Count_M1_ABZ(void)
// {
//   // Debug：读取编码器计数值
//     volatile int32_t encoder_count = __HAL_TIM_GET_COUNTER(&htim2);

// }



// void Count_M2_ABZ(void)
// {
//   // Debug：读取编码器计数值
//     volatile int32_t encoder_count = __HAL_TIM_GET_COUNTER(&htim3);

// }

// // 中断服务函数
// void HAL_TIMEx_EncoderIndexCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim->Instance == TIM3)
//     {
//         // 索引信号中断处理逻辑
//         // 可以在这里重置编码器计数值，或者记录当前计数值
//         __HAL_TIM_SET_COUNTER(htim, 0);
//         // printf("Encoder Index detected!\r\n");
//     }
// }

// //电机速度计算
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim == &ENCODER_TIMER)
//     {
//         // 获取当前脉冲计数
//         pulse_count = __HAL_TIM_GET_COUNTER(&ENCODER_TIMER);

//         // 计算脉冲增量
//         int32_t pulse_delta = pulse_count - prev_pulse_count;

//         // 计算速度 (PPS: 脉冲每秒)
//         // 假设定时器周期为1秒，则速度为脉冲增量
//         speed = (float)pulse_delta;

//         // 更新上次脉冲计数
//         prev_pulse_count = pulse_count;
//     }
// }

#include "tim.h"
#include "ABZ.h"
#include "foc.h"

    int realCnt = 0;

ABZ_Encoder encoderDef;

// 变量声明
volatile int32_t ABZ_pulse_count = 0;
volatile int32_t ABZ_prev_pulse_count = 0;
volatile float ABZ_speed = 0.0f;
volatile float ABZ_angle = 0.0f;

// 假设编码器的每转脉冲数
#define ENCODER_PULSES_PER_REV 4096

// 定时器周期 (秒)
#define TIMER_PERIOD 1.0f

void TIM2_M1_ABZ(void)
{
    // 启动编码器模式
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);

    // 启动索引中断
    HAL_TIMEx_EnableEncoderIndex(&htim2);
}

void TIM3_M2_ABZ(void)
{
    // 启动编码器模式
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    // 启动索引中断
    HAL_TIMEx_EnableEncoderIndex(&htim3);
}

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

//// 中断服务函数
//void HAL_TIMEx_EncoderIndexCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        // 索引信号中断处理逻辑
//        // 可以在这里重置编码器计数值，或者记录当前计数值
//        __HAL_TIM_SET_COUNTER(htim, 0);
//        // printf("Encoder Index detected!\r\n");
//    }
//}

// 电机速度和角度计算
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        // 清除定时器更新中断标志
        __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
        // 获取当前脉冲计数
        ABZ_pulse_count = __HAL_TIM_GET_COUNTER(&htim3);

        // 计算脉冲增量
        int32_t pulse_delta = ABZ_pulse_count - ABZ_prev_pulse_count;

        // 计算速度 (PPS: 脉冲每秒)
        // 假设定时器周期为1秒，则速度为脉冲增量
        ABZ_speed = (float)pulse_delta / TIMER_PERIOD;

        // 计算角度 (单位：度)
        // 角度 = 当前计数值 / 每转脉冲数 * 360度
        ABZ_angle = (float)(ABZ_pulse_count % ENCODER_PULSES_PER_REV) / ENCODER_PULSES_PER_REV * 360.0f;

        // 更新上次脉冲计数
        ABZ_prev_pulse_count = ABZ_pulse_count;
    }
}

int UpdataEncoderCnt(void)
{   
    // 启动编码器模式
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

    // 启动索引中断
    HAL_TIMEx_EnableEncoderIndex(&htim3);
	//获取电机读数
	
	encoderDef.cnt = (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
    
    encoderDef.preCnt = encoderDef.cnt;
	//计算电机偏移量
	encoderDef.incCnt = ModifyIncCnt(encoderDef.cnt - encoderDef.preCnt);
	// 获取真实的编码器计数值
    realCnt = encoderDef.cnt - encoderDef.offsetCnt;
	// 当计数值为负数时，加上一圈的计数数
	while(realCnt<0)
	{
		realCnt += ENCODER_PULSES_PER_REV * 4;
	}
	// 计算真实的角度值
	encoderDef.angle=encoderDef.cnt*2*PI / ENCODER_PULSES_PER_REV / 4;
    
	// 计算电角度
	encoderDef.elecAngle = _normalizeAngle(encoderDef.angle);
    
	// 返回编码器读数
	return encoderDef.cnt;
}

//// 3. 定义中断服务程序
//void TIM3_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&htim3);
//}


int16_t ModifyIncCnt(int16_t delta)
{
    // 如果增量超过了一半计数范围，说明是溢出，需要进行调整
    if (delta > ENCODER_HALF_COUNT)
    {
        delta -= (ENCODER_MAX_COUNT + 1);
    }
    else if (delta < -ENCODER_HALF_COUNT)
    {
        delta += (ENCODER_MAX_COUNT + 1);
    }
    return delta;
}

