#ifndef __ABZ_H
#define __ABZ_H
#include "main.h"

#define ENCODER_MAX_COUNT 4096  // 定时器计数器的最大值 (16位定时器)
#define ENCODER_HALF_COUNT (ENCODER_MAX_COUNT / 2)  // 定时器计数器的中间值

typedef struct{
    int32_t preCnt;
    int32_t cnt;
    int32_t offsetCnt;
    int32_t angle;
    int32_t elecAngle;
    int32_t incCnt;
}ABZ_Encoder;


void TIM2_M1_ABZ(void);
void Count_M1_ABZ(void);

void TIM3_M2_ABZ(void);
void Count_M2_ABZ(void);

//int16_t ModifyIncCnt(int16_t delta);
//int UpdateEncoderCnt(void);

float ABZ_GetAngle360(void);
float ABZ_GetAngle2PI(void);
float ABZ_GetAngle(void);
float ABZ_GetVelocity(void);
float ABZ_Get_Speed(void);

#endif
