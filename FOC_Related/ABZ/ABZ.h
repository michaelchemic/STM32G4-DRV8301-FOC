#ifndef __ABZ_H
#define __ABZ_H
#include "main.h"

#define ENCODER_MAX_COUNT 4096  // 定时器计数器的最大值 (16位定时器)
#define ENCODER_HALF_COUNT (ENCODER_MAX_COUNT / 2)  // 定时器计数器的中间值

typedef struct{
    int16_t preCnt;
    int16_t cnt;
    int16_t offsetCnt;
    int16_t angle;
    int16_t elecAngle;
    int16_t incCnt;
}ABZ_Encoder;


void TIM2_M1_ABZ(void);
void Count_M1_ABZ(void);

void TIM3_M2_ABZ(void);
void Count_M2_ABZ(void);

int16_t ModifyIncCnt(int16_t delta);
int UpdataEncoderCnt(void);
#endif
