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

typedef struct
{
	int richtung;
	TIM_HandleTypeDef timer_pwm;

}gesindel_spindel;

gesindel_spindel spindle_context;

void SpindleInit(TIM_HandleTypeDef timer_pwm)
{
	SpindlePhysicalParams_t s;
	s.maxRPM = 9000.0f;
	s.minRPM = -9000.0f;
	s.absMinRPM = 1600.0f;
	s.setDirection = SPINDLE_SetDirection;
	s.setDutyCycle = SPINDLE_SetDutyCycle;
	s.enaPWM = SPINDLE_EnaPWM;
	s.context = &spindle_context;
	hSpindle = SPINDLE_CreateInstance( 4*configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 3, cH, &s);

	spindle_context.richtung = 0;
	spindle_context.timer_pwm = timer_pwm;


}

void SPINDLE_SetDirection (SpindleHandle_t h, void* context, int backward)
{
	gesindel_spindel* spindle_context = (gesindel_spindel*)context;

	spindle_context->richtung =(backward != 0) ? 1 : 0;
}

void SPINDLE_SetDutyCycle (SpindleHandle_t h, void* context, float dutyCycle)
{
	gesindel_spindel* spindle_context = (gesindel_spindel*)context;

	uint32_t timer_arr = TIM2->ARR;

	if (spindle_context->richtung)
	{
		TIM2->CCR3 = 0;
		TIM2->CCR4 = (uint32_t)(((float)TIM2->ARR)*dutyCycle);
	}
	else
	{
		TIM2->CCR4 = 0;
		TIM2->CCR3 = (uint32_t)(((float)TIM2->ARR)*dutyCycle);
	}
}

void SPINDLE_EnaPWM (SpindleHandle_t h, void* context, int ena)
{
	gesindel_spindel* spindle_context = (gesindel_spindel*)context;

	HAL_GPIO_WritePin(SPINDLE_ENA_L_GPIO_Port, SPINDLE_ENA_L_Pin, ena);
	HAL_GPIO_WritePin(SPINDLE_ENA_R_GPIO_Port, SPINDLE_ENA_R_Pin, ena);

	if (ena) {
			HAL_TIM_PWM_Start(&spindle_context->timer_pwm, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&spindle_context->timer_pwm, TIM_CHANNEL_4);
		}
		else {
			HAL_TIM_PWM_Stop(&spindle_context->timer_pwm, TIM_CHANNEL_3);
			HAL_TIM_PWM_Stop(&spindle_context->timer_pwm, TIM_CHANNEL_4);
		}
}



