#ifndef __GPIO_PORT_F_H__
#define __GPIO_PORT_F_H__

#include <stdbool.h>
#include <stdint.h>
#include "TM4C123.h"
#include "../include/sysctrl.h"

#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))

//*****************************************************************************
// Enabling PortF will require the following steps                          
//    1. Turn on the clock for PORTF in RCGCGPIO
//
//        ONLY modify the bit for PORTF!
//
//        Search the datahseet for RCGCGPIO to determine
//        the correct value to set each register to.
//
//        ../include/sysctrl.h contains many useful
//        #defines that will make your code more readable and less mistake prone.
//        Search for SYSCTL_RCGCGPIO in that file.
//
//        Example 
//        SYSCTL->RCGCGPIO = val;
//
//    2. Wait until the clock is on by checking PRGPIO
//
//      Search the datahseet for PRGPIO to determine
//      the correct value to set each register to.
//
//
//        ../include/sysctrl.h contains many useful
//        #defines that will make your code more readable and less mistake prone.
//        Search for SYSCTL_PRGPIO in that file.
//
//        Example 
//        val = SYSCTL->PRGPIO;
//
//    Steps 3 & 4 are only necessary for Ports F and D.
//
//    3. We need to set the LOCK register to 0x4C4F434B.
//
//       Example code:
//       GPIO_PORTF_LOCK_R = 0x4C4F434B ;
//
//    4. Set the commit register
//      
//        Example Code:
//        GPIO_PORTF_CR_R = 0xFF;
//*****************************************************************************
void  portF_enable_port(void);


//*****************************************************************************
// Setting a GPIOF pins as a digital enable requires writing to DEN register
//
// Paramters
//    pins  -   A bit mask indicating which pins should be configured as digital
//              pins.  Modify only the bits where the bitmask is equal to 1.
//
// Use section 10.5 of the TM4C123 data sheet to determine the bits to set in 
// the DEN register.
//
//  Code Example
//  GPIOx->DEN = val;
//*****************************************************************************
void  portF_digital_enable(uint8_t pins);


//*****************************************************************************
// Setting a GPIO pin as an output requires setting the DIR register
//
// Paramters
//    pins  -   A bit mask indicating which pins should be configured as output
//              pins.  Modify only the bits where the bitmask is equal to 1.
//
// Use section 10.5 of the TM4C123 data sheet to determine the bits to set in 
// the DIR register.
//
//  Code Example
//  GPIOx->DIR = val;
//*****************************************************************************
void  portF_enable_output(uint8_t pins);

#endif
