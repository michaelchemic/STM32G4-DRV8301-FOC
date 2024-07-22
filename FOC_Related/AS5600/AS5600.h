#ifndef AS5600_H
#define AS5600_H

#include "stm32G4xx_hal.h"

#define AS5600_I2C_ADDR 0x36

// Register addresses
#define AS5600_REG_RAW_ANGLE 0x0C
#define AS5600_REG_ANGLE 0x0E

// I2C_HandleTypeDef *hi2c1;

HAL_StatusTypeDef AS5600_ReadRawAngle(I2C_HandleTypeDef *hi2c1, float *raw_angle);
HAL_StatusTypeDef AS5600_ReadAngle(I2C_HandleTypeDef *hi2c1, float *angle);

float AS5600_GetAngle360(void);
float AS5600_GetAngle2PI(void);
float AS5600_GetAngle(void);
float AS5600_GetVelocity(void);
float AS5600_Get_Speed(void);

#endif // AS5600_H
