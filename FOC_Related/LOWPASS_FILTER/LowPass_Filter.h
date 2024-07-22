#ifndef __LOWPASS_FILTER_H
#define __LOWPASS_FILTER_H
#include "adc.h"
#define ADC_BUFFER_SIZE 10 // ���建������С����������Ӧ��Ҫ���˲�ǿ��

typedef struct {
    uint16_t buffer[ADC_BUFFER_SIZE];
    uint8_t index;
    uint32_t sum;
} ADC_Filter_t;

extern float y_prev;

 // ����ȫ�ֱ���
extern ADC_Filter_t adc1_filter_Ia;
extern ADC_Filter_t adc1_filter_Ib;
extern ADC_Filter_t adc1_filter_Ic;

float LowPass_Filter(float x);
void ADC_Filter_Init(ADC_Filter_t *filter) ;
uint16_t ADC_Filter_Update(ADC_Filter_t *filter, uint16_t new_value) ;



#endif
