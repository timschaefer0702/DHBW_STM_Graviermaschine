#include "Stepper.h"
extern SPI_HandleTypeDef hspi1;

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

	HAL_GPIO_WritePin(GPIOF, STEP_RSTN_Pin, !ena);

}


void StepLibraryDelay( int timems ){
	vTaskDelay(pdMS_TO_TICKS( timems));
}


int StepTimer( void* pPWM, int dir, unsigned int numPulses ){


	HAL_GPIO_WritePin( GPIOF, STEP_DIR_Pin, dir );

	for(int i = 0; i < numPulses; i++){

		HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS( 100 ));
		HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS( 100 ));

	}


}


int StepTimerCancelAsync( void* pPWM ){

	HAL_GPIO_WritePin(GPIOD, STEP_PULSE_Pin, GPIO_PIN_RESET);

}


