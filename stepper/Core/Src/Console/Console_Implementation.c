/*
 * Console_Implementation.c
 *
 *  Created on: Apr 30, 2025
 *      Author: timsc
 */
#include "Console_Implementation.h"

#define PRINT_RESULT(res) ((res) != 0 ? printf("\r\nFAIL") : printf("\r\nOK"))

extern ConsoleHandle_t cH;

extern stepper_context schmarn_context;

uint32_t Console_init ()
{
	cH = CONSOLE_CreateInstance( 4*configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 5 );

	CONSOLE_RegisterCommand(cH, "capability", "prints a specified string of capability bits",
							CapabilityFunc, NULL);
	CONSOLE_RegisterCommand(cH, "stepper", "<<stepper>> is used to control a stepper motor.\r\n",
							StepperCommand_Func, &schmarn_context);

}

static int CapabilityFunc( int argc, char** argv, void* ctx )
// --------------------------------------------------------------------------------------------------------------------
{
	(void)argc;
	(void)argv;
	(void)ctx;
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\nOK",
	    1, // has spindle
		1, // has spindle status
		1, // has stepper
		1, // has stepper move relative
		0, // has stepper move speed
		0, // has stepper move async
		1, // has stepper status
		1, // has stepper refrun
		1, // has stepper refrun timeout
		1, // has stepper refrun skip
		1, // has stepper refrun stay enabled
		1, // has stepper reset
		1, // has stepper position
		1, // has stepper config
		1, // has stepper config torque
		1, // has stepper config throvercurr
		1, // has stepper config powerena
		1, // has stepper config stepmode
		1, // has stepper config timeoff
		1, // has stepper config timeon
		1, // has stepper config timefast
		1, // has stepper config mmperturn
		1, // has stepper config posmax
		1, // has stepper config posmin
		1, // has stepper config posref
		1, // has stepper config stepsperturn
		0  // has stepper cancel
	);
	return 0;
}

static int StepperCommand_Func( int argc, char** argv, void* ctx)
{
	L6474_Handle_t h = (L6474_Handle_t)ctx;
// kein subcommand
	if ( argc == 0 )
	{
	printf("invalid number of arguments\r\nFAIL");
	return -1;
	}
// stepper reset
//-------------------------------------------------------------------------
	if ( strcmp(argv[0], "reset") == 0 )
	{
		printFuncUnsuccess(StepperReset());
	}
// stepper move
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "move") == 0 )
	{
		long movement;

		if (argc < 2)
		{
			printf("missing further parameters");
			return -1;
		}
		else if (argc == 2)
		{
			if(checkZahlenEingabeInt(argv[1],&movement) == 0 )
			{
				printFuncUnsuccess(StepperMove(movement,0));
			}
		}
		else if (argc == 3)
		{
			if( strcmp(argv[2], "-r") == 0 && checkZahlenEingabeInt(argv[1],&movement) == 0 )
			{
				printFuncUnsuccess(StepperMove(movement,1));
			}
			if( strcmp(argv[2], "-a") == 0 && checkZahlenEingabeInt(argv[1],&movement) == 0 )
			{
				//moveAbs async
			}
		}
		else if (argc == 4)
		{
			long speed;
			if( strcmp(argv[2], "-s") == 0 && checkZahlenEingabeInt(argv[1],&movement) == 0
				&& checkZahlenEingabeInt(argv[3],&speed) == 0)
			{
				//moveAbs mit Speed
			}
		}
		//TODO move funktion einbinden
	}
// stepper reference
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "reference") == 0 )
	{
	//TODO referenzfahrt programmieren
		if (argc == 1)
		{
			printFuncUnsuccess( StepperReference(0, 0) );
		}
		else if (argc == 2)
		{
			if( strcmp(argv[1], "-e") == 0 )
			{
				printFuncUnsuccess( StepperReference(1, 0) );
			}
			if( strcmp(argv[1], "-s") == 0 )
			{
				printFuncUnsuccess( StepperReference(2, 0) );
			}
		}
		else if (argc == 3)
		{
			long time;
			if( strcmp(argv[1], "-t") == 0 && checkZahlenEingabeInt(argv[2],&time) == 0)
			{
				printFuncUnsuccess( StepperReference(3, (uint16_t) time) );
			}
		}
	}
// stepper position
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "position") == 0 )
	{
		int position = -1;
		printFuncUnsuccess( L6474_GetAbsolutePosition(schmarn_context.hL6474, &position ));
		printf("%d\r\n", position);

	}
// stepper cancel
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "cancel") == 0 )
	{
		printFuncUnsuccess(StepTimerCancelAsync(NULL));

	}
// stepper status
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "status") == 0 )
	{
		printFuncUnsuccess(StepperStatus());
	}
// stepper config
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "config") == 0 )
		{
		int result;
			if ( strcmp(argv[1], "powerena") == 0 )
			{
				long powerena;
				if (argc == 2)
				{
					printFuncUnsuccess(StepperPoweroutputs(0,0));
				}
				else if (argc == 4 && strcmp(argv[2], "-v") == 0 && checkZahlenEingabeInt(argv[3],&powerena) == 0)
				{
					printFuncUnsuccess(StepperPoweroutputs(1,powerena));
				}
			}
			else if(argc == 2)
			{
				int value_to_read = 0;


				if ( strcmp(argv[1], "torque") == 0 )
				{
					printFuncUnsuccess(L6474_GetProperty(h, L6474_PROP_TORQUE, &value_to_read));
					printf("%d\r\n", value_to_read);

				}
				if ( strcmp(argv[1], "throvercurr") == 0 )
				{
					printFuncUnsuccess(L6474_GetProperty(h, L6474_PROP_OCDTH, &value_to_read));
					printf("%d\r\n", value_to_read);

				}
				if ( strcmp(argv[1], "stepmode") == 0 )
				{
					int stepmode;
					printFuncUnsuccess(L6474_GetStepMode(h, &stepmode));
					printf("%d\r\n", stepmode);


				}
				if ( strcmp(argv[1], "stepsperturn") == 0 )
				{
					printf("OK\r\n");
					printf("%d\r\n", schmarn_context.stepper_stepsPturn);
				}
				if ( strcmp(argv[1], "timeoff") == 0 )
				{
					printFuncUnsuccess(L6474_GetProperty(h, L6474_PROP_TOFF, &value_to_read));
					printf("%d\r\n", value_to_read);

				}
				if ( strcmp(argv[1], "timeon") == 0 )
				{
					printFuncUnsuccess(L6474_GetProperty(h, L6474_PROP_TON, &value_to_read));
					printf("%d\r\n", value_to_read);
				}
				if ( strcmp(argv[1], "timefast") == 0 )
				{
					printFuncUnsuccess(L6474_GetProperty(h, L6474_PROP_TFAST, &value_to_read));
					printf("%d\r\n", value_to_read);
				}
				if ( strcmp(argv[1], "mmperturn") == 0 )
				{
					printf("OK\r\n");
					printf("%f\r\n", schmarn_context.stepper_mmPturn);
				}
				if ( strcmp(argv[1], "posmax") == 0 )
				{
					printf("OK\r\n");
					printf("%f\r\n", (float)(schmarn_context.stepper_maxSteps * schmarn_context.stepper_mmPturn)
									/ (float)(schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution));

				}
				if ( strcmp(argv[1], "posmin") == 0 )
				{
					printf("OK\r\n");
					printf("%f\r\n", (float)(schmarn_context.stepper_minSteps * schmarn_context.stepper_mmPturn)
									/ (float)(schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution));
				}
				if ( strcmp(argv[1], "posref") == 0 )
				{
					printf("OK\r\n");
					printf("%f\r\n", (float)(schmarn_context.stepper_refSteps * schmarn_context.stepper_mmPturn)
									/ (float)(schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution));
				}


			}
			else if (argc == 4)
			{
				//Int Config Werte
				long integerValue;
				if((strcmp(argv[2], "-v") == 0) && checkZahlenEingabeInt(argv[3],&integerValue) == 0)
				{
				// requires integers in command line commands
					// not blocked by active states commands
					if ( strcmp(argv[1], "torque") == 0 )
					{
						printFuncUnsuccess(L6474_SetProperty(h, L6474_PROP_TORQUE, integerValue));
					}
					if ( strcmp(argv[1], "throvercurr") == 0 )
					{
						printFuncUnsuccess(L6474_SetProperty(h, L6474_PROP_OCDTH, integerValue));
					}
					if ( strcmp(argv[1], "stepmode") == 0 )
					{
						L6474x_StepMode_t sm;
						switch (integerValue)
						{
						case 0:
							sm = smFULL;
							break;
						case 1:
							sm = smHALF;
							break;
						case 2:
							sm = smMICRO4;
							break;
						case 3:
							sm = smMICRO8;
							break;
						case 4:
							sm = smMICRO16;
							break;
						default:
							printf("Invalid step mode\r\n");
							return -1;
						}
						schmarn_context.stepper_resolution = integerValue;
						printFuncUnsuccess(L6474_SetStepMode(schmarn_context.hL6474, sm));

					}
					if ( strcmp(argv[1], "stepsperturn") == 0 )
					{
						printf("OK\r\n");
						schmarn_context.stepper_stepsPturn = integerValue;
					}
					if(schmarn_context.stepper_state < scsENA)
					{
						//BLOCKED BY ACTIVE STATE CONFIG COMMANDS
						if ( strcmp(argv[1], "timeoff") == 0 )
						{
							printFuncUnsuccess(L6474_SetProperty(h, L6474_PROP_TOFF, integerValue));
						}
						if ( strcmp(argv[1], "timeon") == 0 )
						{
							printFuncUnsuccess(L6474_SetProperty(h, L6474_PROP_TON, integerValue));
						}
						if ( strcmp(argv[1], "timefast") == 0 )
						{
							printFuncUnsuccess(L6474_SetProperty(h, L6474_PROP_TFAST, integerValue));
						}
					}

				}
				double doubleValue;
				if(strcmp(argv[2], "-v") == 0 && checkZahlenEingabeFloat(argv[3],&doubleValue))
				{
				// requires floating points in command line commands
					if ( strcmp(argv[1], "mmperturn") == 0 )
					{
						printf("OK\r\n");
						schmarn_context.stepper_mmPturn = doubleValue;
					}
					else if ( strcmp(argv[1], "posmax") == 0 )
					{
						printf("OK\r\n");
						schmarn_context.stepper_maxSteps = (doubleValue * schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution)
															/ schmarn_context.stepper_mmPturn;
					}
					else if ( strcmp(argv[1], "posmin") == 0 )
					{
						printf("OK\r\n");
						schmarn_context.stepper_minSteps = (doubleValue * schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution)
																					/ schmarn_context.stepper_mmPturn;
					}
					else if ( strcmp(argv[1], "posref") == 0 )
					{
						printf("OK\r\n");
						schmarn_context.stepper_refSteps = (doubleValue * schmarn_context.stepper_stepsPturn  * schmarn_context.stepper_resolution)
																											/ schmarn_context.stepper_mmPturn;
					}
					else
					{
						printf("invalid parameters\r\nFAIL");
					}
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

int checkZahlenEingabeInt (char* arg, long* numberVal)
{
	char* endOfNumber;
	long val = strtol(arg, &endOfNumber, 10);

	if (arg == endOfNumber)
	{
		return -1;
	}
	else
	{
		*numberVal = val;
		return 0;
	}
}

int checkZahlenEingabeFloat (char* arg, double* numberVal)
{
	char* endOfNumber;
	double val = strtod(arg, &endOfNumber);

	if (arg == endOfNumber)
	{
		return -1;
	}
	else
	{
		*numberVal = val;
		return 0;
	}
}




