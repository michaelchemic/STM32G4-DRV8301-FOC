/*
 * 文件名: LowPass_Filter.c
 * 作者: MichaelKing
 * 日期: 2024-06-26
 * 版本: 1.0
 * 描述: 该函数每次调用时，都会根据当前输入值 x 和前一个输出值 y_prev 计算新的输出值 y。
 * 公式 y = 0.9 * y_prev + 0.1 * x 是一阶递归低通滤波器的实现。这里的系数 0.9 和 0.1 控制了滤波器的特性。系数之和为 1，保持信号幅度的一致性。
 *
 * 历史记录:
 * 版本 1.0 - 2024-06-26 - MichaelKing
 *   初始版本。
 */
#include "LOWPASS_FILTER.h"
#include "adc.h"


float y_prev;  // 上一个输出值

//对应三相电流
ADC_Filter_t adc1_filter_Ia;
ADC_Filter_t adc1_filter_Ib;
ADC_Filter_t adc1_filter_Ic;

float LowPass_Filter(float x)
{
    // 使用低通滤波器公式计算当前输出
    float y = 0.9f * y_prev + 0.1f * x;
    
    // 更新上一个输出值
    y_prev = y;
    
    // 返回当前输出值
    return y;
}

void ADC_Filter_Init(ADC_Filter_t *filter) {
    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        filter->buffer[i] = 0;
    }
    filter->index = 0;
    filter->sum = 0;
}

uint16_t ADC_Filter_Update(ADC_Filter_t *filter, uint16_t new_value) {
    // 从sum中减去最老的值
    filter->sum -= filter->buffer[filter->index];

    // 将新值加入缓冲区，并加入sum
    filter->buffer[filter->index] = new_value;
    filter->sum += new_value;

    // 更新缓冲区索引
    filter->index++;
    if (filter->index >= ADC_BUFFER_SIZE) {
        filter->index = 0;
    }

    // 返回平均值
    return (uint16_t)(filter->sum / ADC_BUFFER_SIZE);
}

