/*
 * Spindle_Implementation.c
 *
 *  Created on: May 13, 2025
 *      Author: timsc
 */
#include "Spindle_Implementation.h"

extern ConsoleHandle_t cH;
extern SpindleHandle_t hSpindle;
extern uint16_t spindle_direction;

void SpindleInit()
{
	SpindlePhysicalParams_t s;
	s.maxRPM = 9000.0f;
	s.minRPM = -9000.0f;
	s.absMinRPM = 1600.0f;
	s.setDirection = SPINDLE_SetDirection;
	s.setDutyCycle = SPINDLE_SetDutyCycle;
	s.enaPWM = SPINDLE_EnaPWM;
	s.context = NULL;
	hSpindle = SPINDLE_CreateInstance( 4*configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 3, cH, &s);
}

void SPINDLE_SetDirection (SpindleHandle_t h, void* context, int backward)
{
	spindle_direction = (backward != 0) ? 1 : 0;
}

void SPINDLE_SetDutyCycle (SpindleHandle_t h, void* context, float dutyCycle)
{

}

void SPINDLE_EnaPWM (SpindleHandle_t h, void* context, int ena)
{

}



