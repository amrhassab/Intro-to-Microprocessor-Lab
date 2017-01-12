//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "gpioPortF.h"

extern void serialDebugInit(void);

#define   RED     (1 << 1)
#define   BLUE    (1 << 2)
#define   GREEN   (1 << 3)

//*****************************************************************************
// Only modify the following function where you see ADD CODE
//*****************************************************************************
void user_input(void)
{
   char cmd;
   while(1)
   {
      printf("\n\r\n\r");
      printf("***********************\n\r");
      printf(" Supported Commands\n\r");
      printf("    1. R_ON\n\r");
      printf("    2. R_OFF\n\r");
      printf("    3. G_ON\n\r");
      printf("    4. G_OFF\n\r");
      printf("    5. B_ON\n\r");
      printf("    6. B_OFF\n\r");
      printf("\n\r");
      printf(" Command : ");

      cmd = getchar();
      cmd = cmd - 0x30;
     
      switch (cmd)
      {
        case 1: // Turn Red ON, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA |= 0x2;
            break;
        }
        case 2: // Turn Red OFF, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA &= ~0x2;
            break;
        }
        case 3: // Turn Green ON, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA |= 0x8;
            break;
        }
        case 4: // Turn Green OFF, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA &= ~0x8;
            break;
        }
        case 5: // Turn Blue ON, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA |= 0x4;
            break;
        }
        case 6:  // Turn Blue OFF, other colors unmodified.
        {
            //  <ADD CODE>
						GPIOF->DATA &= ~0x4;
            break;
        }
        default:
        {
          printf("\n\r  !!! Unknown command: %d\n\r", cmd);
          break;
        }
      }
        
   }
}


//*****************************************************************************
// Only modify the following function where you see ADD CODE
//*****************************************************************************
int 
main(void)
{
  serialDebugInit();

  printf("\n\r************************************\n\r");
  printf("ECE353 - GPIO In Class Exercise\n\r");
  printf("************************************\n\r");
  
  // Enable PORTF  <ADD CODE>
	portF_enable_port();
  
  // Set the appropriate pins as outputs    <ADD CODE>
	portF_enable_output(0xE);
  
  // Set the appropriate pins as Digital Pins    <ADD CODE>
	portF_digital_enable(0xE);

  
  // Get User Input
  user_input();
  
  // Reach infinite loop
  while(1){};
}
