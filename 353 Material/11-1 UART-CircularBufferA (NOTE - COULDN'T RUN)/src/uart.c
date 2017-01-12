#include "uart.h"

#include "pc_buffer.h"

#define UART0_BUFFER_SIZE 80

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

PC_Buffer UART0_Rx_Buffer;
char UART0_Rx_Buffer_Array[UART0_BUFFER_SIZE];

PC_Buffer UART0_Tx_Buffer;
char UART0_Tx_Buffer_Array[UART0_BUFFER_SIZE];


//************************************************************************
// Configure UART0 to be 115200, 8N1.  Data will be sent/recieved using
// polling (Do Not enable interrupts)
//************************************************************************
bool uart0_init_115K(void)
{
    
    // Turn on the UART Clock
    SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R0;
    
    // Wait until the UART is ready
    while( (SYSCTL->PRUART & SYSCTL_PRUART_R0) == 0)
    {
      // busy wait
    }
    
    // Set the baud rate
    UART0->IBRD = 27;
    UART0->FBRD = 9;
    
    // Disable UART
    UART0->CTL &= ~UART_CTL_UARTEN;
    
    // Configure the Line Control for 8N1, FIFOs
    UART0->LCRH =   UART_LCRH_WLEN_8 | UART_LCRH_FEN;

    // Configure the FIFO Interrupt Levels
    UART0->IFLS = UART_IFLS_RX7_8 | UART_IFLS_TX1_8;
    
    // Turn on the UART Interrupts  for Tx, Rx, and Rx Timeout
    UART0->IM = UART_IM_RXIM | UART_IM_TXIM | UART_IM_RTIM;
    
    // Set the priority to 1
    NVIC_SetPriority(UART0_IRQn, 1);
  
    // Enable the NVIC for the watchdog timer
    NVIC_EnableIRQ(UART0_IRQn);
    
    
    // Initialize the circular buffer
    pc_buffer_init(&UART0_Tx_Buffer,UART0_Tx_Buffer_Array, UART0_BUFFER_SIZE); 
    pc_buffer_init(&UART0_Rx_Buffer,UART0_Rx_Buffer_Array, UART0_BUFFER_SIZE); 
        
    // Enable Tx, Rx, and the UART
    UART0->CTL =  UART_CTL_RXE |  UART_CTL_TXE |  UART_CTL_UARTEN;
    
    return true;

}


