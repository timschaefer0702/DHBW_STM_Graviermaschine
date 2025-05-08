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
	CONSOLE_RegisterCommand(c, "stepper", "<<stepper>> is used to control a stepper motor.\r\n",
								StepperCommand_Func, NULL);

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

static int StepperCommand_Func( int argc, char** argv, void* ctx)
{
	L6474_Handle_t h = (L6474_Handle_t)ctx;

	if ( argc == 0 )
	{
	printf("invalid number of arguments\r\nFAIL");
	return -1;
	}
//-------------------------------------------------------------------------
	if ( strcmp(argv[0], "reset") == 0 )
	{
		printFuncUnsuccess(StepperReset());
	}
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "move") == 0 )
	{
		if (argc < 2){
			printf("missing further parameters");
			return -1;
		}
		//TODO move funktion einbinden
	}
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "reference") == 0 )
	{
	//TODO referenzfahrt programmieren
		if (argc == 1)
		{
			//TODO normale Referenzfahrt Poweroutputs danach aus machen
		}
		else if ((argc > 1)&& strcmp(argv[1],"-e")== 0)
		{
			//TODO Referenzfahrt mit poweroutputs anlassen
		}
	}
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "config") == 0 )
		{
			if ( strcmp(argv[1], "powerena") == 0 )
			{
				if (argc == 2)
				{
					//TODO powerenable umschalten bzw. negieren -> wir müssen powerena status getten
				}
				else if (argc == 4 && strcmp(argv[2], "-v") == 0)
				{
					//TODO powerenable auf Wert in argv[3] setzen
				}
			}
		}
//-------------------------------------------------------------------------
	else
	{
	printf("invalid subcommand was given as argument\r\nFAIL");
	}


	return 0;
}

// prints OK when function was successfully called - return value 0, or prints FAIL if func was not sucessfully called -> return value !0
static void printFuncUnsuccess(int unsuccess){

	if( unsuccess ){

			printf("\r\nFAIL");

	}

	else{

		printf("\r\nOK");

	}

}




