//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "boardUtil.h"
#include "pc_buffer.h"


/******************************************************************************
 * Global Variables
 *****************************************************************************/

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
  char input[80];
  initializeBoard();
  
  //uartTxPoll(UART0_BASE, "Hello World\n\r");
  printf("**************************************\n\r");
  printf("* Week11 - Advanced UART Features A   \n\r");
  printf("**************************************\n\r");
  
  // Infinite Loop
  while(1){
  memset(input,0,80);
  printf("\n\rEnter a string: ");
  scanf("%79[^\n]", input);
  printf("You entered: %s\n\r",input);
  };
}
