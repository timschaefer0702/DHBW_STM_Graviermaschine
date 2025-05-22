#include "Stepper.h"
extern SPI_HandleTypeDef hspi1;
//extern L6474_Handle_t hL6474;
extern enum Stepper_StatusCode_t stepper_state;
extern long stepper_abs_pos;

extern stepper_context schmarn_context;




// ------------------------ functions for L6474x_Platform_t -------------------------------------------------------------------------------------------------
 void* StepLibraryMalloc( unsigned int size ){

	return malloc(size);

}

void StepLibraryFree( const void* const ptr ){

	free((void*)ptr);

}


int StepDriverSpiTransfer( void* pIO, char* pRX, const char* pTX, unsigned int length ){


	// byte based access, so keep in mind that only single byte transfers are performed!

	for ( unsigned int i = 0; i < length; i++ ){

		HAL_GPIO_WritePin( GPIOD, STEP_SPI_CS_Pin, GPIO_PIN_RESET );
		//vTaskDelay( 70 );
		HAL_SPI_TransmitReceive( &hspi1, pTX + i, pRX + i, (uint16_t)1, HAL_MAX_DELAY );
		// keep CS on HIGH for 800ns
		HAL_GPIO_WritePin( GPIOD, STEP_SPI_CS_Pin, GPIO_PIN_SET );
		//vTaskDelay( CS_CPUTicks_Wait );

	}



	return 0;
}


void StepDriverReset( void* pGPO, const int ena ){

	HAL_GPIO_WritePin(GPIOF, STEP_RSTN_Pin, !ena); // set/release reset Pin

}


void StepLibraryDelay( unsigned int timems ){
	vTaskDelay(pdMS_TO_TICKS( timems));
}


int StepTimer( void* pPWM, int dir, unsigned int numPulses ){


	HAL_GPIO_WritePin( GPIOF, STEP_DIR_Pin, dir );

	for(int i = 0; i < numPulses; i++){

		HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS( 10 ));
		HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS( 10 ));

	}

	return 0;


}


int StepTimerCancelAsync( void* pPWM ){

	HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_RESET);

	return 0;

}


// ------------------------------- general functions called by Console Commands ------------------------------------------------------------------------------

int StepperReset(){

	if( schmarn_context.stepper_state == scsInit || schmarn_context.stepper_state == scsFLT ){

		int result = OK;

			L6474_BaseParameter_t param;
			result |= L6474_SetBaseParameter(&param);

			// reset and reinitialize, do not setPowerOutput
			result |= L6474_ResetStandBy(schmarn_context.hL6474);
			result |= L6474_Initialize(schmarn_context.hL6474, &param);

			if( result == OK ){

				schmarn_context.stepper_state = scsRef;

			}

			else{

				schmarn_context.stepper_state = scsFLT;

			}

			return result;

	}

	else{
		printf("invalid state\r\n");
		return FAIL;
	}


}

/*
 * 0 - no param
 * 1 - -e (leaves power enabled)
 * 2 - -s (skip ref drive)
 * 3 - -t (with timeout) -> only here time_to_timeout used
 */
int StepperReference(int param, uint16_t time_to_timeout){

	int result = OK;

	if( schmarn_context.stepper_state == scsRef ){

			switch (param) {

			 	case 0:

			 		result |= L6474_SetPowerOutputs(schmarn_context.hL6474, 1);

			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 	    result |= L6474_StepIncremental(schmarn_context.hL6474, -1);

			 		}

			 		if( result == OK ){

			 			schmarn_context.stepper_state = scsDIS;
			 			schmarn_context.stepper_abs_pos = 0;
			 			result |= L6474_SetAbsolutePosition(schmarn_context.hL6474,0);


			 		}

			 		else{

			 			schmarn_context.stepper_state = scsFLT;

			 		}



			 		result |= L6474_SetPowerOutputs(schmarn_context.hL6474, 0);


			 		break;

			 	case 1:

			 		result |= L6474_SetPowerOutputs(schmarn_context.hL6474, 1);

			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 				result |= L6474_StepIncremental(schmarn_context.hL6474, -1);

			 		}

			 		if( result == OK ){

			 			schmarn_context.stepper_state = scsENA;

			 		}

			 		else{

			 			schmarn_context.stepper_state = scsFLT;

			 		}
			 		result |= L6474_SetAbsolutePosition(schmarn_context.hL6474,0);

			 		break;

			 	case 2:

			 		result = OK;
			 		schmarn_context.stepper_state = scsDIS;
			 		result |= L6474_SetAbsolutePosition(schmarn_context.hL6474,0);
			 		// TODO macht sinn? lol? oder tatsächliche position irgendwoher nehmen?
			 		schmarn_context.stepper_abs_pos = 0;

			 		break;

			 	case 3:

			 		// TODO overflow nicht beachtet
			 		TickType_t currentTime = pdTICKS_TO_MS( xTaskGetTickCount() ); // current time in ms
			 		TickType_t endTime = currentTime + time_to_timeout * 1000;


			 		if( endTime < currentTime ){ // overflow

			 		}

			 		result |= L6474_SetPowerOutputs(schmarn_context.hL6474, 1);


			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 				result |= L6474_StepIncremental(schmarn_context.hL6474, -1);
			 				currentTime = pdTICKS_TO_MS( xTaskGetTickCount() );

			 				if ( currentTime > endTime ) {

			 					L6474_SetPowerOutputs(schmarn_context.hL6474, 0);
			 					return FAIL;

			 				}

			 		}

			 		if( result == OK ){

			 			schmarn_context.stepper_state = scsDIS;

			 		}

			 		else{

			 			schmarn_context.stepper_state = scsFLT;

			 		}

			 		result |= L6474_SetPowerOutputs(schmarn_context.hL6474, 0);

			 		break;
			}
	}

	else{

		result = FAIL;
	}
	result |= L6474_SetAbsolutePosition(schmarn_context.hL6474,0);
	return result;
}

//param:
//		0 - move abs
//		1 - move rel
//		2 - speed


int StepperMove(int movement, int param){
	// TODO LED blinken
	int result = OK;

	if (schmarn_context.stepper_state != scsENA )
	{
		return FAIL;
	}
	int jetzige_pos;
	int ziel;
	result |= L6474_GetAbsolutePosition(schmarn_context.hL6474, &jetzige_pos);

	switch (param)
	{

	case 0:
		 ziel = (movement * schmarn_context.stepper_stepsPturn * schmarn_context.stepper_resolution)
					/(60 * schmarn_context.stepper_mmPturn);



			ziel = ziel - jetzige_pos;

			if (ziel > schmarn_context.stepper_maxSteps || ziel < schmarn_context.stepper_minSteps )
			{
				printf("no valid step goal");
				return -1;
			}

			result |= L6474_StepIncremental(schmarn_context.hL6474, ziel);
			break;

	case 1:
		 ziel = (movement * schmarn_context.stepper_stepsPturn * schmarn_context.stepper_resolution)
					/(60 * schmarn_context.stepper_mmPturn);

		if (ziel + jetzige_pos > schmarn_context.stepper_maxSteps || ziel + jetzige_pos < schmarn_context.stepper_minSteps )
					{
						printf("no valid step goal");
						return -1;
					}
		result |= L6474_StepIncremental(schmarn_context.hL6474, ziel);

			break;

	case 2:

			break;


	default:

		return -1;

	}


	if( result ) schmarn_context.stepper_state = scsFLT;

	return result;

}

int StepperStatus()
{
	int result = 0;
	uint32_t driver_status = 0;
	L6474_Status_t dS;
	result |= L6474_GetStatus(schmarn_context.hL6474, &dS);

	driver_status = (dS.DIR)|(dS.HIGHZ<<1)|(dS.NOTPERF_CMD<<2)|(dS.OCD<<3)|(dS.ONGOING<<4)
					|(dS.TH_SD<<5)|(dS.TH_WARN<<6)|(dS.UVLO<<7)|(dS.WRONG_CMD<<8);

	printf("0x%x\r\n0x%x\r\n", schmarn_context.stepper_state, ( unsigned int )driver_status);

	printf("0\r\n"); //running einfügen

	if( result ) schmarn_context.stepper_state = scsFLT;

	return result;
}

int StepperPoweroutputs(int minusV, int value)
{
	if( schmarn_context.stepper_state == scsENA || schmarn_context.stepper_state == scsDIS ){

		int result = 0;
			if (!minusV)
			{
				int onoff = (schmarn_context.stepper_state == scsDIS) ? 1 : 0 ;
				printf("Powerenable: %d\r\n",onoff);

			}
			else
			{
				result |= L6474_SetPowerOutputs(schmarn_context.hL6474, value);
				schmarn_context.stepper_state = (value == 1) ? scsENA : scsDIS;
			}

			if( result ) schmarn_context.stepper_state = scsFLT;

			return result;

	}
	printf(" invalid state \r\n ");

	return -1;

}


