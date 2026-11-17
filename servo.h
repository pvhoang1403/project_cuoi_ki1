#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_hal.h"

void Servo_Init(TIM_HandleTypeDef *htim);
void Servo_SetAngle(uint8_t angle);

#endif
