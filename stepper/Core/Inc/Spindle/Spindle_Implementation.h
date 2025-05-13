/*
 * Spindle_Implementation.h
 *
 *  Created on: May 13, 2025
 *      Author: timsc
 */
#include "main.h"
#ifndef INC_SPINDLE_SPINDLE_IMPLEMENTATION_H_
#define INC_SPINDLE_SPINDLE_IMPLEMENTATION_H_


void SpindleInit();
void SPINDLE_SetDirection (SpindleHandle_t h, void* context, int backward);
void SPINDLE_SetDutyCycle (SpindleHandle_t h, void* context, float dutyCycle);
void SPINDLE_EnaPWM (SpindleHandle_t h, void* context, int ena);

#endif /* INC_SPINDLE_SPINDLE_IMPLEMENTATION_H_ */
