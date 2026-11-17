#include "servo.h"

static TIM_HandleTypeDef *servo_tim;

void Servo_Init(TIM_HandleTypeDef *htim)
{
    servo_tim = htim;

    HAL_TIM_PWM_Start(servo_tim, TIM_CHANNEL_1);
}

void Servo_SetAngle(uint8_t angle)
{
    uint16_t pulse;

    if(angle > 180)
    {
        angle = 180;
    }

    pulse = 1000 + (angle * 1000) / 180;

    __HAL_TIM_SET_COMPARE(
        servo_tim,
        TIM_CHANNEL_1,
        pulse);
}
