#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdint.h>
#include <stdbool.h>

#include "TM4C123.h"
#include "boardUtil.h"

// SysTick handler flag.
extern volatile bool sysTickUpdate, validDistance;
extern volatile int pulseCount, countEveryFour, risingFallingCountF, risingFallingCountC;
extern volatile int distance;

void TIMER0A_Handler(void);
void SysTick_Handler(void);

void UART7_Handler(void);

void GPIOC_Handler(void);
void GPIOF_Handler(void);


#endif
