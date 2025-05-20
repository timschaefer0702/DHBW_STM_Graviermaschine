/*
 * Spindle_Implementation.h
 *
 *  Created on: May 13, 2025
 *      Author: timsc
 */
#include "main.h"




void SpindleInit(TIM_HandleTypeDef timer_pwm);
void SPINDLE_SetDirection (SpindleHandle_t h, void* context, int backward);
void SPINDLE_SetDutyCycle (SpindleHandle_t h, void* context, float dutyCycle);
void SPINDLE_EnaPWM (SpindleHandle_t h, void* context, int ena);


