// Copyright (c) 2014, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>



#include "TM4C123.h"
#include "boardUtil.h" // ADDED adc.h, interrupts.h to includes


// Pin 0 is TIVA_SONAR_RX input     -- J8,  Left Facing
// Pin 1 is TIVA_SONAR_TX output    
// Pin 2 is SONAR_PW                -- J7,  Right Facing
// Pin 3 is SONAR_AN                -- J11, Center Facing
#define TIVA_SONAR_RX (1 << 0)
#define TIVA_SONAR_TX (1 << 1)
#define SONAR_PW (1 << 2)
#define SONAR_AN (1 << 3)


//*****************************************************************************
// Global Variables
//*****************************************************************************

// Used with SysTick_Handler
volatile bool AlertSysTick; // Every 50us
volatile bool AlertRoutine; // Every 10ms
volatile bool AlertPrint;   // Every 1s

// GPIOE_Handler
volatile bool edgeChange;

// Used with UART7_Handler
volatile char buffer[4];

  
//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
  serialDebugInit();
  EnableInterrupts();
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	uint32_t dist_analog;
	uint32_t dist_pulsewidth;
	//uint32_t dist_serial; // ASCII Characters in buffer
	uint32_t pulseWidth;
	
	uint8_t signals[2];		
	bool capture;
	char printBuffer[40];
	
  initializeBoard();

  uartTxPoll(UART0_BASE, "\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  uartTxPoll(UART0_BASE,"* ECE315 Default Project\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  
	
	// Initialize GPIOE
	// Pin 0 is TIVA_SONAR_RX input     -- J8,  Left Facing
	// Pin 1 is TIVA_SONAR_TX output    -- This doesn't need to be used   
	// Pin 2 is SONAR_PW      input     -- J7,  Right Facing
	// Pin 3 is SONAR_AN      input     -- J11, Center Facing
	
	// Enable the port
	gpio_enable_port(GPIOE_BASE);
	// Configure the UART RX pin and SONAR_PW pin as digital
	gpio_config_digital_enable(GPIOE_BASE, TIVA_SONAR_RX | SONAR_PW);
	// Configure the SONAR_PW pin as analog
	gpio_config_analog_enable(GPIOE_BASE, SONAR_AN);
	// Configure the three pins as inputs
	gpio_config_enable_input(GPIOE_BASE, TIVA_SONAR_RX | SONAR_PW | SONAR_AN);
	// Configure the UART RX pin as alternate function
	gpio_config_alternate_function(GPIOE_BASE, TIVA_SONAR_RX);
	// Configure the port control to direct the UART RX pin to UART7
	gpio_config_port_control(GPIOE_BASE, GPIO_PCTL_PE0_U7RX);

	
	// Initialize UART7 to 9600 8N1, with RX interrupts enabled at 1/4 full
	uart_init_9600(UART7_BASE, SYSCTL_RCGCUART_R7, SYSCTL_PRUART_R7);

	// Enable the interrupt from UART7 peripheral, then set the priority
	NVIC_EnableIRQ(UART7_IRQn);
	NVIC_SetPriority(UART7_IRQn, 3);	


	// Initialize ADC0
	initializeADC(ADC0_BASE);
	
	// Set SysTick to interrupt every 50us
	SysTick_Config(2500);
	
	
	edgeChange = false;
	capture = false;
	pulseWidth = 0;
  // Infinite Loop
	

  while(1)
  {
		if(AlertSysTick){
			// Poll the SONAR_PW pin (GPIOE pin 2)
			signals[1] = signals[0];
			signals[0] = ((GPIOE->DATA & SONAR_PW) >> 2);
			
			if(signals[0] && !signals[1]){
				pulseWidth = 50;
			}
			else if(signals[0] && signals[1]){
				pulseWidth += 50;
			}
			else if(!signals[0] && signals[1]){
				dist_pulsewidth = pulseWidth / 147;
				pulseWidth = 0;
			}
			else{
				pulseWidth = 0;
			}
			// End polling of SONAR_PW

			
			AlertSysTick = false;
		}
		if(AlertRoutine){
			dist_analog = getADCValue(ADC0_BASE, 0) / 9.8;
			AlertRoutine = false;
		}
		if(AlertPrint){
			sprintf(printBuffer, "--------------------\n\r");
			uartTxPoll(UART0_BASE, printBuffer);
			// Print the center facing J11 analog distance
			sprintf(printBuffer, "Center: dist_analog: %d\n\r", dist_analog);
			uartTxPoll(UART0_BASE, printBuffer);
			// Print the right facing J7 pulsewidth distance
			sprintf(printBuffer, "Right: dist_pulsewidth: %d\n\r", dist_pulsewidth);
			uartTxPoll(UART0_BASE, printBuffer);
			// Print the left facing J8 serial distance
			sprintf(printBuffer, "Left: dist_serial: %s\n\r", buffer);
			uartTxPoll(UART0_BASE, printBuffer);

			
			AlertPrint = false;
		}
  }
}
