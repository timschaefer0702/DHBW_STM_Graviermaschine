#include "Stepper.h"
extern SPI_HandleTypeDef hspi1;
extern L6474_Handle_t hL6474;
extern enum Stepper_StatusCode_t stepper_state;
extern long stepper_abs_pos;

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


void StepLibraryDelay( int timems ){
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

	int result = OK;

	L6474_BaseParameter_t param;
	result |= L6474_SetBaseParameter(&param);

	// reset and reinitialize, do not setPowerOutput
	result |= L6474_ResetStandBy(hL6474);
	result |= L6474_Initialize(hL6474, &param);

	if( result == OK ){

		stepper_state = scsRef;

	}

	else{
		stepper_state = scsInit;
	}

	return result;

}

/*
 * 0 - no param
 * 1 - -e (leaves power enabled)
 * 2 - -s (skip ref drive)
 * 3 - -t (with timeout) -> only here time_to_timeout used
 */
int StepperReference(int param, uint16_t time_to_timeout){

	int result = OK;

	if( stepper_state == scsRef ){

			switch (param) {

			 	case 0:

			 		result |= L6474_SetPowerOutputs(hL6474, 1);

			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 	    result |= L6474_StepIncremental(hL6474, -1);

			 		}

			 		if( result == OK ){

			 			stepper_state = scsDIS;
			 			stepper_abs_pos = 0;

			 		}

			 		else{

			 			stepper_state = scsInit;

			 		}



			 		result |= L6474_SetPowerOutputs(hL6474, 0);


			 		break;

			 	case 1:

			 		result |= L6474_SetPowerOutputs(hL6474, 1);

			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 				result |= L6474_StepIncremental(hL6474, -1);

			 		}

			 		if( result == OK ){

			 			stepper_state = scsENA;

			 		}

			 		else{

			 			stepper_state = scsInit;

			 		}

			 		break;

			 	case 2:

			 		result = OK;
			 		stepper_state = scsDIS;
			 		// TODO macht sinn? lol? oder tatsächliche position irgendwoher nehmen?
			 		stepper_abs_pos = 0;

			 		break;

			 	case 3:

			 		// TODO overflow nicht beachtet
			 		TickType_t currentTime = pdTICKS_TO_MS( xTaskGetTickCount() ); // current time in ms
			 		TickType_t endTime = currentTime + time_to_timeout * 1000;

			 		if( endTime < currentTime ){ // overflow

			 		}

			 		result |= L6474_SetPowerOutputs(hL6474, 1);

			 		if( result != OK ) return result; // das wird ja eh nix

			 		// drive till contact
			 		while( HAL_GPIO_ReadPin(REFERENCE_MARK_GPIO_Port, REFERENCE_MARK_Pin) == GPIO_PIN_SET && result == OK ){

			 				result |= L6474_StepIncremental(hL6474, -1);
			 				currentTime = pdTICKS_TO_MS( xTaskGetTickCount() );

			 				if ( currentTime > endTime ) {

			 					L6474_SetPowerOutputs(hL6474, 0);
			 					return FAIL;

			 				}

			 		}

			 		if( result == OK ){

			 			stepper_state = scsDIS;

			 		}

			 		else{

			 			stepper_state = scsInit;

			 		}

			 		result |= L6474_SetPowerOutputs(hL6474, 0);

			 		break;



			}







	}

	else{

		result = FAIL;

	}


	return result;

}

int StepperMove(int abs){
	// TODO LED blinken
	int result = OK;

	// TODO check for borders

	if( abs ){

		return FAIL;

	}

	return result;

}


