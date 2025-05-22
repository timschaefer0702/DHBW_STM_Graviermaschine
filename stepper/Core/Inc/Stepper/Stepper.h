#include "main.h"
#define CS_CPUTicks_Wait 200
void* StepLibraryMalloc( unsigned int size );

void StepLibraryFree( const void* const ptr );

int StepDriverSpiTransfer( void* pIO, char* pRX, const char* pTX, unsigned int length );


void StepDriverReset(void* pGPO, const int ena);

void StepLibraryDelay( unsigned int timems );

int StepTimer( void* pPWM, int dir, unsigned int numPulses );

int StepTimerCancelAsync( void* pPWM );

int StepperReset();

int StepperReference(int param, uint16_t time_to_timeout);

int StepperMove(int movement, int param);

int StepperStatus();

int StepperPoweroutputs( int minusV, int value);
