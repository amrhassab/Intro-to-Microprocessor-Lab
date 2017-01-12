
#include "interrupts.h"

volatile bool sysTickUpdate = false;
volatile int countEveryFour = 0;
volatile int pulseCount, risingFallingCountF, risingFallingCountC = 0;
volatile int distance = 0;
volatile bool validDistance = false;

 /****************************************************************************
 * The SysTick Handler 
 *
 * Register Information on the SysTick timer can be found on pages 129-132 
 * of the MCU data sheet.
 *
 * Register Information on the Watchdog timer can be found on pages 749-754.
 *
 ****************************************************************************/
void SysTick_Handler(void)
{
	// Clear SysTick interrupt.
	int a = SysTick->VAL;
	
	// Flag the interrupt.
	sysTickUpdate = true;
}

void GPIOF_Handler(void) 
{
	if(countEveryFour == 4 && pulseCount >= 0){
		 pulseCount--;
		 countEveryFour = 0;
	}
	countEveryFour++;

	
	GPIOF->ICR |= 0x03; // PortF Pins 0 and 1
  GPIOF->IM |= 0x03;
}

void GPIOC_Handler(void) 
{
	if(countEveryFour == 4 && pulseCount >= 0){
		 pulseCount--;
		 countEveryFour = 0;
	}
	
	countEveryFour++;
	
	GPIOC->ICR |= 0x60; // PortC Pins 5 and 6
	GPIOC->IM |= 0x60; 
	
		
}

void UART7_Handler(void) 
{
	// Take 8 LSB bits, top 4 MSB are flags.
  distance = (UART7->DR & 0x0FF);

	// valid distance has been reached
	if (distance > 6 && distance < 256){
		validDistance = true;
	}
	
	// clear the interrupt
	UART7->ICR |= 0x01;
	
}

