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

			}
		}
		else if (argc == 3)
		{
			if( strcmp(argv[2], "-r") == 0 && checkZahlenEingabeInt(argv[1],&movement) == 0 )
			{
				//moveRel
			}
			if( strcmp(argv[2], "-a") == 0 && checkZahlenEingabeInt(argv[1],&movement) == 0 )
			{
				//moveAbs
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
			//TODO normale Referenzfahrt Poweroutputs danach aus machen
		}
		else if (argc == 2)
		{
			if( strcmp(argv[1], "-e") == 0 )
			{
				//TODO Referenzfahrt mit poweroutputs anlassen
			}
			if( strcmp(argv[1], "-s") == 0 )
			{
				//Referenzfahrt überspringen
			}
		}
		else if (argc == 3)
		{
			long time;
			if( strcmp(argv[1], "-t") == 0 && checkZahlenEingabeInt(argv[2],&time) == 0)
			{
				//Referenzfahrt mit Timeout
			}
		}
	}
// stepper position
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "position") == 0 )
	{

	}
// stepper cancel
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "cancel") == 0 )
	{

	}
// stepper status
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "status") == 0 )
	{

	}
// stepper config
//-------------------------------------------------------------------------
	else if ( strcmp(argv[0], "config") == 0 )
		{
			if ( strcmp(argv[1], "powerena") == 0 )
			{
				long powerena;
				if (argc == 2)
				{
					//TODO powerenable umschalten bzw. negieren -> wir müssen powerena status getten
				}
				else if (argc == 4 && strcmp(argv[2], "-v") == 0 && checkZahlenEingabeInt(argv[3],&powerena) == 0)
				{
					//TODO powerenable auf Wert in argv[3] setzen
				}
			}
			else if (argc == 4)
			{
				//Int Config Werte
				long integerValue;
				if(strcmp(argv[2], "-v") == 0 && checkZahlenEingabeInt(argv[3],&integerValue))
				{
				// requires integers in command line commands
					// not blocked by active states commands
					if ( strcmp(argv[1], "torque") == 0 )
					{

					}
					if ( strcmp(argv[1], "throvercurr") == 0 )
					{

					}
					if ( strcmp(argv[1], "stepmode") == 0 )
					{

					}
					if ( strcmp(argv[1], "stepsperturn") == 0 )
					{

					}
					//BLOCKED BY ACTIVE STATE CONFIG COMMANDS TODO
					if ( strcmp(argv[1], "timeoff") == 0 )
					{

					}
					if ( strcmp(argv[1], "timeon") == 0 )
					{

					}
					if ( strcmp(argv[1], "timefast") == 0 )
					{

					}
				}
				double doubleValue;
				if(strcmp(argv[2], "-v") == 0 && checkZahlenEingabeFloat(argv[3],&doubleValue))
				{
				// requires floating points in command line commands
					if ( strcmp(argv[1], "mmperturn") == 0 )
					{

					}
					if ( strcmp(argv[1], "posmax") == 0 )
					{

					}
					if ( strcmp(argv[1], "posmin") == 0 )
					{

					}
					if ( strcmp(argv[1], "posref") == 0 )
					{

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




