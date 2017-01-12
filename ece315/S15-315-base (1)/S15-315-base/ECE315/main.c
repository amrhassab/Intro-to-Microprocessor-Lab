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

#include "adc.h"
#include "TM4C123.h"
#include "boardUtil.h"



//*****************************************************************************
// Global Variables
//*****************************************************************************
volatile bool AlertSysTick = false;
volatile bool AlertUART = false;
  
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
  //variables
	int count = 0;
	int i = 0;
	int distance;
	uint32_t distance_r;
  char printBuff[32];
	uint32_t pulse;
	int pulseCount = 0;
	char distance_l [3];
	int print_count = 0;
	//int distance_l_conv = 0;
	char poll;



	serialDebugInit();
	uartTxPoll(UART0_BASE, "\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
  uartTxPoll(UART0_BASE,"* ECE315 Default Project\n\r");
  uartTxPoll(UART0_BASE,"**************************************\n\r");
	
  initializeBoard();
	
	 
	SysTick_Config(2500);

	 
	
	
	
// Configure PE2 for right sensor and PE3 for middle
gpio_enable_port(GPIOB_BASE);
gpio_config_digital_enable(GPIOB_BASE, PB0);
gpio_config_enable_output(GPIOB_BASE, PB0);

gpio_config_analog_enable(GPIOE_BASE, PE3);
gpio_config_alternate_function(GPIOE_BASE, PE3);
initializeADC(  ADC0_BASE );
	
// Enable the interrupt from UART7 peripheral
NVIC_EnableIRQ(UART7_IRQn);
NVIC_SetPriority(UART7_IRQn, 3);	


  
  
  // Infinite Loop
  while(1)
  {
		
		if(AlertSysTick){
			
			//GPIOB->DATA = 1; //used to show value on scope
			AlertSysTick = false;
			pulse = GPIOE->DATA & PE2;

			// right sensor
			
			if(pulse == 4){
				pulseCount++;
			}
			
			else if (pulseCount != 0){
				
				distance_r = (pulseCount*50)/147;
				pulseCount = 0;
				
			}

			//sprintf(printBuff, "%u", pulse);
			//uartTxPoll(UART0_BASE , printBuff);

			count++;
			print_count++;
			//GPIOB->DATA = 0; //used to show value on scope
		}
	
		//middle sensor
		if(count == 200){
      // GPIOB->DATA = 1;
			 count  = 0;
			 distance = (int) (getADCValue( ADC0_BASE, 0))/6.4;
			 //GPIOB->DATA = 0;
		}
	  if (print_count == 20000){
		  print_count = 0;
			
			    for(i = 0; i < 25; i++){
						uartRxPoll(UART7_BASE, true);
					}
				
					while(uartRxPoll(UART7_BASE, true) != 'R' ){}
					
					for(i = 0; i < 3; i++){
						poll = uartRxPoll(UART7_BASE, true);
						if(poll != 'R' && poll != 0x0D){
							distance_l[i] = poll;
						}else
						//	distance_l[i] = '!';
							i--;
					}
					//uartRxPoll(UART7_BASE, true);
			
				//uartRxPoll(UART7_BASE, true);
				//distance_l[3] = NULL;
				
			//left print
			sprintf(printBuff, "DL = %s\n\r", distance_l);
			uartTxPoll(UART0_BASE, printBuff);
			
			//right print
			sprintf(printBuff, "DR = %u\n\r", distance_r);			
			uartTxPoll(UART0_BASE , printBuff);
			//center print
		  sprintf(printBuff, "DF = %d\n\r", distance);
			uartTxPoll(UART0_BASE , printBuff);
			uartTxPoll(UART0_BASE, "****************************\n\r");
		}
  }
}
