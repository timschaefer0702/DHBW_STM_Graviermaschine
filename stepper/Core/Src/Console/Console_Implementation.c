/*
 * Console_Implementation.c
 *
 *  Created on: Apr 30, 2025
 *      Author: timsc
 */
#include "Console_Implementation.h"


extern L6474_Handle_t hL6474;

uint32_t Console_init ()
{
	ConsoleHandle_t c = CONSOLE_CreateInstance( 4*configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 5 );

	CONSOLE_RegisterCommand(c, "capability", "prints a specified string of capability bits",
								CapabilityFunc, NULL);
	CONSOLE_RegisterCommand(c, "schmarn", "resets the stepper motor",
								StepperReset_Func, NULL);
}

static int CapabilityFunc( int argc, char** argv, void* ctx )
// --------------------------------------------------------------------------------------------------------------------
{
	(void)argc;
	(void)argv;
	(void)ctx;
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\nOK",
	    0, // has spindle
		0, // has spindle status
		0, // has stepper
		0, // has stepper move relative
		0, // has stepper move speed
		0, // has stepper move async
		0, // has stepper status
		0, // has stepper refrun
		0, // has stepper refrun timeout
		0, // has stepper refrun skip
		0, // has stepper refrun stay enabled
		0, // has stepper reset
		0, // has stepper position
		0, // has stepper config
		0, // has stepper config torque
		0, // has stepper config throvercurr
		0, // has stepper config powerena
		0, // has stepper config stepmode
		0, // has stepper config timeoff
		0, // has stepper config timeon
		0, // has stepper config timefast
		0, // has stepper config mmperturn
		0, // has stepper config posmax
		0, // has stepper config posmin
		0, // has stepper config posref
		0, // has stepper config stepsperturn
		0  // has stepper cancel
	);
	return 0;
}

static int StepperReset_Func( int argc, char** argv, void* ctx )
{
	if(argc < 0)
	{
		printf("Schmarn passiert");
	}
	//hier sauber stepper resetten
	printf("kein Schmarn passiert");
	L6474_ResetStandBy(hL6474);
	return 0;
}

