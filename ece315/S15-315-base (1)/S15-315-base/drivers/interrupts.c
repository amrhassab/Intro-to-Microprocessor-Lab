#include "interrupts.h"

extern volatile bool AlertSysTick;
extern volatile bool AlertUART;
void SysTick_Handler(void)
{
   uint32_t val;
  
   AlertSysTick = true;
   
   val = SysTick->VAL;
}
