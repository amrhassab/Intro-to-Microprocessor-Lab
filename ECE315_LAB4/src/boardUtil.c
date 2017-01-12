#include "TM4C123GH6PM.h"
#include "boardUtil.h"
#include "TM4C123GH6PM.h"

#include "adc.h"
#include "sysctrl.h"
//#include "uart.h"

#define CLK_DIV 16.0 //HSE is not set so ClkDiv should be 16
#define BAUD_RATE 9600


void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

//*****************************************************************************
//*****************************************************************************
void serialDebugInit(void)
{
  UART_CONFIG serialDebugConfig = {
    SERIAL_DEBUG_UART_BASE,
    ENABLED,                    // UART
    ENABLED,                    // UARTRx
    ENABLED,                    // UARTTx
    _8BITS,                     // WordLen
    9600,                       // BaudRate
    ENABLED,                    // FIFO
    DISABLED,                   // _2Stop
    DISABLED,                   // Parity
    DISABLED,                   // EvenParity
    DISABLED,                   // HighSpeed
    RX_ONE_EIGHTH,              // RxIntFIFO
    TX_ONE_EIGHTH,              // TxIntFIFO
    ENABLED,                    // RxIntMask
    ENABLED,                    // TxIntMask
    ENABLED,                    // RxTimeOutIntMask
    DISABLED,                   // OvrrIntMask
    DISABLED,                   // ParityErrIntMask
    DISABLED                    // FramingErrIntMask  
  };
  volatile uint32_t delay;
	
  // Configure Rx Pins
  gpioConfigDigitalEnable(SERIAL_DBG_BASE, SERIAL_DBG_RX_PIN);
  gpioConfigAltFunction(SERIAL_DBG_BASE, SERIAL_DBG_RX_PIN);
  gpioConfigPortConfig(SERIAL_DBG_BASE, SERIAL_DBG_RX_PCTL);
  
  // Configure Tx Pins
  gpioConfigDigitalEnable(SERIAL_DBG_BASE, SERIAL_DBG_TX_PIN);
  gpioConfigAltFunction(SERIAL_DBG_BASE, SERIAL_DBG_TX_PIN);
  gpioConfigPortConfig(SERIAL_DBG_BASE, SERIAL_DBG_TX_PCTL);
	
	// Initialize UART0 for serial debug
  InitializeUART(&serialDebugConfig);
	
	//***************************************************************************
	// Mike, Nev, & Steve's work below.
	//***************************************************************************
	
	// ADC setup (before sequencer).
	// 1) Enable ADC clock using RCGCADC register
	// 2) Enable the clock to the appropriate GPIO modules via RCGCGPIO register (?)
	// 3) Set AFSEL bits as input 
	// 4) Configure AINx signals to be analog by clearing DEN bits in GPIO Digital Enable (GPIODEN) register
	// 5) Disable the analog isolation circuit for all ADC input pins that are to be used by writing a 1 to the appropriate bits of the GPIOAMSEL register
	// 6) Do sequencer setup (done for us)
	
	// (1) Enable the ADC's clock.
	SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
	delay = SYSCTL->RCGCADC;
	
	// (2) Enable the clock of PORT E.
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R4;
	delay = SYSCTL->RCGCGPIO;
	
	// This does the same thing as (2)- yes
	gpioEnablePort(GPIOE_BASE);
	
	// (3) Set alternate function select bits as input.
	GPIOE->AFSEL |= PE3;
	
	// (4) Clear DEN bits.
	GPIOE->DEN &= ~PE3;
	
	// (5) Disable the analog isolation circuit.
	GPIOE->AMSEL |= PE3;
	
	// Configure SONAR_AN (PE3) as an analog input.
	gpioConfigPinAsInput(GPIOE_BASE, PE3);
	gpioConfigAnalogEnable(GPIOE_BASE, PE3);
	
	// (6) Configure ADC0 sequencer and (1)
	initializeADC(ADC0_BASE);
	
	// Configure SONAR_PW (PE2) as a digital input.
	gpioConfigPinAsInput(GPIOE_BASE, PE2);
	gpioConfigDigitalEnable(GPIOE_BASE, PE2);
	
	// Configure SysTick to interrupt at 50 microseconds.
	SysTick_Config(2500);
	
	
	
	/////////////////
	// configure PE1 for testing as digital pin
	/////////////////

	// Configure PE1 as input.
	gpioConfigPinAsOutput(GPIOE_BASE, PE1);
	
	// Set PE1's DEN
	gpioConfigDigitalEnable(GPIOE_BASE, PE1);

	// Configure PE3 for ADC.
	// Turn on clock of PORT E.
	//gpioCheckBase(GPIOE_BASE);
	
	// Turn on clock of PORT E (again).
	//gpioCheckClock(GPIOE_BASE);

	//gpioConfigDigitalEnable(GPIOE_BASE, PE3);
	
	//gpioConfigPortConfig(GPIOE_BASE, GPIO_PCTL_PE3_M);
	
	// Configure SONAR_PW (PE2) as a digital input.
	//gpio_status_t  gpioConfigDigitalEnable(uint32_t gpioBase, uint8_t pins);
	//gpio_status_t  gpioConfigPinAsInput(uint32_t gpioBase, uint8_t pins);
	
}


void encodersInit(void) {
	// PF0 is a special purpose pin, so we have to unlock it
	//unlock the commit register
	
	// Enable the port clk
	gpioEnablePort(GPIOC_BASE);
	//gpioEnablePort(GPIOF_BASE);		// Do not reinitialize this port clock.
																	// It was already done in  drv8833.c
	
	GPIOF->LOCK = GPIO_LOCK_KEY;
 // GPIOF->CR = 0x01; // enabling GPIOF Pin 0 DEN to be written

	
	// configure as input
	gpioConfigPinAsInput(GPIOF_BASE, (PF0 | PF1));
	gpioConfigPinAsInput(GPIOC_BASE, (PC5 | PC6));
	
	gpioConfigDigitalEnable(GPIOF_BASE, (PF0 | PF1));
	gpioConfigDigitalEnable(GPIOC_BASE, (PC5 | PC6));
	
	gpioConfigInterruptRisingEdges(GPIOF_BASE, (PF0 | PF1));
  gpioConfigInterruptRisingEdges(GPIOC_BASE, (PC5 | PC6));
	
  gpioConfigInterruptFallingEdges(GPIOF_BASE, (PF0 | PF1));
  gpioConfigInterruptFallingEdges(GPIOC_BASE, (PC5 | PC6));
	
	// Lock the GPIOF->LOCK to ensure nobody else does something stupid
	GPIOF->LOCK = GPIO_LOCK_M;
	// set up the NVIC interrupt control
	NVIC->ISER[0] = 0x40000004;
}

/*
  Todo: Get the Clk val & Baud rate
  and calculate the baud rate register's
  values inside the init function.
*/
void UartInit(void)
{
	
	float BRD;

	//PE0: TIVA_SONAR_RX
	//PE1: TIVA_SONAR_TX
	
  UART_CONFIG j8config = {
    UART7_BASE,                 // Using UART7 from schematic
    ENABLED,                    // UART
    ENABLED,                    // UARTRx
    ENABLED,                    // UARTTx
    _8BITS,                     // WordLen
    BAUD_RATE,                  // BaudRate
    ENABLED,                    // FIFO
    DISABLED,                   // _2Stop
    DISABLED,                   // Parity
    DISABLED,                   // EvenParity
    DISABLED,                   // HighSpeed
    RX_ONE_EIGHTH,              // RxIntFIFO - interrupt after 8 bits received
    TX_ONE_EIGHTH,              // TxIntFIFO
    ENABLED,                    // RxIntMask
    ENABLED,                    // TxIntMask
    ENABLED,                    // RxTimeOutIntMask
    DISABLED,                   // OvrrIntMask
    DISABLED,                   // ParityErrIntMask
    DISABLED                    // FramingErrIntMask  
  };
	

	// BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * BaudRate)
	BRD = UART_CC_CS_M/ (CLK_DIV * BAUD_RATE);

	// Configure Rx Pins
	gpioConfigPinAsInput(GPIOE_BASE, PE0);
  gpioConfigDigitalEnable(GPIOE_BASE, PE0);
  gpioConfigAltFunction(GPIOE_BASE, PE0);
  gpioConfigPortConfig(GPIOE_BASE, GPIO_PCTL_PE0_U7RX);
  
  // Configure Tx Pins
	gpioConfigPinAsOutput(GPIOE_BASE, PE1);
  gpioConfigDigitalEnable(GPIOE_BASE, PE1);
  gpioConfigAltFunction(GPIOE_BASE, PE1);
  gpioConfigPortConfig(GPIOE_BASE, GPIO_PCTL_PE1_U7TX);
	
	InitializeUART(&j8config);
}

