#ifndef __FOC_H
#define __FOC_H

#define _constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
// 如果amt小于low返回low，如果amt大于high则返回high，否则返回amt本身，限制在low和high范围内。

#include "usart.h"
void Systick_CountMode(void);
float _electricalAngle(void);
float _normalizeAngle(float angle);
void setPWM(float Ua, float Ub, float Uc);
void setTorque(float Uq, float angle_el);
void FOC_Init(float power_supply);
void FOC_AS5600_Init(int _PP, int _DIR);
void Set_Angle(float Angle);
void Set_Speed(float Speed);
void Ratchet_Wheel(float Angle);
void Boundary_Moment_limitation(float angle_range);
void calibrateOffsets(void);
void Current_Loop(float Uq, float angle_el);
void Current_Speed(float Uq,float speed);
void Open_Loop_Control(float Uq, float speed);
void Feedback_Current(void);

void Send_ADC1_Values(UART_HandleTypeDef *huart);

#endif
