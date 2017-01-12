#include "lcd.h"
#include "spi.h"
#include "fonts.h"
#include "TM4C123GH6PM.h"

	// For communicating w/ SPI.
  uint8_t *dataOut;
	
	// SPI initialization commands
	uint8_t initWithSPIcommands[] = {0x40,0xA1,0xC0,0xA4,0xA6,0xA2,0x2F,0x27,0x81,0x10,0xFA,0x90,0xAF};
	uint8_t *arrayPtr;

//*****************************************************************************
// Initializes the pins needed to communicate with the LCD and issues the 
// initialization sequence found in the LCD datasheet via the SPI interface
//*****************************************************************************
void ece315_lcdInit(void)
{
	uint8_t testData = 0xFF;
	uint32_t i = 0;
	uint32_t j = 0;
	
SPI_CONFIG LCD_config = {
    LCD_SPI_BASE,
    DISABLED,                   //EndOfTransEn
    SSI_MASTER_EN,              //MasSlvSelect
    SSI_EN,                     //SSIEnable
    DISABLED,                   //LoopbackModeEn
    10,                         //ClkPrescaleDiv
    SSI_SPH_SECOND,             //SerialCPHA
    SSI_SPO_HIGH,               //SerialCPOL
    SSI_CR0_FRF_MOTO,           //FrameFormatSelect
    SSI_CR0_DSS_8,              //DataSizeSelect
    DISABLED,                   //TxFifoIntMask
    DISABLED,                   //RxFifoIntMask
    DISABLED,                   //RxTimeoutIntMask
    DISABLED,                   //RxOverrunIntMask
    DISABLED,                   //TxDMAEn
    DISABLED,                   //RxDMAEn
  };
 
	gpioEnablePort(GPIOD_BASE);

  // Configure SPI CLK
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_CLK_PIN_PCTL);
    
  // Configure SPI CS
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_CS_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_CS_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_CS_PIN_PCTL);

  // Configure SPI MOSI
  gpioConfigDigitalEnable(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpioConfigAltFunction(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpioConfigPortConfig(LCD_GPIO_BASE, LCD_MOSI_PIN_PCTL);
  
	////////////////////
	// OUR CODE BELOW //
	////////////////////
	
  // Configure CD and Configure RST_N, and set CD and RST_N and GPIO pins
	GPIOD->LOCK = GPIO_LOCK_KEY;
	GPIOD->CR |= 0x80;
  gpioConfigDigitalEnable(GPIO_LCD_CD_BASE, LCD_CD_PIN);
  gpioConfigPinAsOutput(GPIO_LCD_CD_BASE, LCD_CD_PIN);
  gpioConfigDigitalEnable(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
  gpioConfigPinAsOutput(GPIO_LCD_RST_N_BASE, LCD_RST_N_PIN);
	GPIOD->AFSEL &= 0x3F;
	// Lock the GPIOD->LOCK to ensure nobody else does something stupid
	GPIOD->LOCK = GPIO_LOCK_M;
	
  InitializeSPI(&LCD_config);

	// Bring out of reset. Reset pin (LCD_RST_N_PIN) is connected to PD6 
	GPIOD->DATA |= 0x40;
				
  // Use spiTx() from the ece315 driver library to issue the sequence of 
  // commands in the LCD data sheet to initialize the LCD.
	
	// Making CD low to send commands.
	GPIOD->DATA &= 0x7F;
	
	arrayPtr = &initWithSPIcommands[0];
	spiTx(LCD_SPI_BASE, arrayPtr, 1, dataOut); 
  spiTx(LCD_SPI_BASE, arrayPtr + 1, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 2, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 3, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 4, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 5, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 6, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 7, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 8, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 9, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 10, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 11, 1, dataOut);
  spiTx(LCD_SPI_BASE, arrayPtr + 12, 1, dataOut);
	
	ece315_lcdClear();
	
	// Making CD low to send commands.
	GPIOD->DATA &= 0x7F;
	
//	for (i = 0; i < 3000000; i++)
//		; // Spin
//	
//	// Traverse columns and pages. Set each (col, page) combo's data to zero.
//	for (i = 0; i < 8; i++)
//	{
//		// Set page (0 - 7).
//		ece315_lcdSetPage(i);
//		for (j = 0; j < 102; j++)
//		{
//			// Set column (0 - 101).
//			ece315_lcdSetColumn(j);
//			
//			// Turn all pixels on for this (page, col) combination.
//			ece315_lcdWriteData(0xFF);
//		}
//	}
//	
//	for (i = 0; i < 3000000; i++)
//		; // Spin
//	
//	ece315_lcdClear();
	
//	printf("LCD screen cleared.\n");
}
  
 //****************************************************************************
// Sets the currently active page
//*****************************************************************************
  void ece315_lcdSetPage(uint8_t page)
  {
		//Set Page Address command format: 0_1011 PA[3..0]
		uint8_t commandCodeMSB;
		uint8_t commandCodeLSB;
		uint8_t command;
		
		//since the input is 8bits, and PA is 4 bits, you will have to send two messages, MSB first
		uint8_t pageMSB;
		uint8_t pageLSB;
		
		pageMSB = (page >> 4); // 0x0?
		pageLSB = 0x0F & page; // 0x0?

		// Set CD to 0
		GPIOD->DATA &= 0x7F;
		
		command = 0xB0; // D[7:4] == 1011
		commandCodeMSB = command | pageMSB;
		commandCodeLSB = command | pageLSB;
		
		spiTx(LCD_SPI_BASE, &commandCodeMSB, 1, dataOut); 
		spiTx(LCD_SPI_BASE, &commandCodeLSB, 1, dataOut);

  }
  
//*****************************************************************************
// Sets the currently active column
//*****************************************************************************
void ece315_lcdSetColumn(uint8_t column)
{
		// Set Column Address MSB CD:0;     0 0 0 1 CA[7..4]
		// Set Column Address LSB CD:0;     0 0 0 0 CA[3..0] 
		uint8_t commandCodeMSB;
		uint8_t commandCodeLSB;
		uint8_t command;
		uint8_t command2;
		
		// spiTx requires dataIn to be a pointer.
		uint8_t *columnPtr;
	
		uint8_t columnMSB;
		uint8_t columnLSB;

		// Set CD to 0
		GPIOD->DATA &= 0x7F;
		
		//since the input is 8bits, and CA is 4 bits, you will have to send two messages, MSB first
		//columnPtr = &column;
		
		columnMSB = (column >> 4); // 0x0?
		columnLSB = 0x0F & column; // 0x0?
		
		command = 0x10; // D[7:4] == 0001
		command2 = 0x00; // D[7:4] == 0000
		commandCodeMSB = command | columnMSB;
		commandCodeLSB = command2 | columnLSB;
		
		spiTx(LCD_SPI_BASE, &commandCodeMSB, 1, dataOut);
		spiTx(LCD_SPI_BASE, &commandCodeLSB, 1, dataOut);

}
  
//*****************************************************************************
// Writes 8-bits of data to the current column of the LCD
//*****************************************************************************
  void ece315_lcdWriteData(uint8_t data)
  {
		// Write command requires CD = 1
		GPIOD->DATA |= 0x80;
		
		// Write the data into memory
		spiTx(LCD_SPI_BASE, &data, 1, dataOut);
  }
  
//*****************************************************************************
// Erases the LCD screen.
//*****************************************************************************
 void ece315_lcdClear(void)
 {
	int i, j = 0; 

	// Making CD low to send commands.
	GPIOD->DATA &= 0x7F;
	
	// Traverse columns and pages. Set each (col, page) combo's data to zero.
	for (i = 0; i < 8; i++)
	{
		// Set page (0 - 7).
		ece315_lcdSetPage(i);
		for (j = 0; j < 102; j++)
		{
			// Set column (0 - 101).
			ece315_lcdSetColumn(j);
			
			// Turn all pixels on for this (page, col) combination.
			ece315_lcdWriteData(0x00);
		}
	}
 }

//*****************************************************************************
// Each character is 10 columns wide.  The colStart is the column number where
// the first column will be printed.
//
// font.c contains a lookup table for printing out characters to the LCD screen.
// You should note that each character is 16 pixels high and 10 pixels wide.
// For each character, you will have to write to two different pages to print
// out a single character.  
//*****************************************************************************
void ece315_lcdWriteChar(uint8_t page, char c, uint8_t colStart)
{
		uint8_t data;
		int i;
		int pos;
		pos = ((c - 32) * 20);
	 
		ece315_lcdSetPage(page);
		for(i = 0; i < 10; i++)
		{
				data = courierNew_10ptBitmaps[pos + i];
				ece315_lcdSetColumn(colStart + i);
				ece315_lcdWriteData(data);
		}
		
		ece315_lcdSetPage(page + 1);
		for(i = 0; i < 10; i++)
		{
				data = courierNew_10ptBitmaps[pos + i + 10];
				ece315_lcdSetColumn(colStart + i);
				ece315_lcdWriteData(data);
		}
}
 
//*****************************************************************************
// Write a string of characters out to the LCD screen.  Only the first 
// 10 characters will be printed.  The function will also terminate when
// a null character is encountered.
//*****************************************************************************
void ece315_lcdWriteString(uint8_t line, char *string)
{
	int i = 0;
	char *stringCopy = string;
	while (*stringCopy != NULL)
	{
		ece315_lcdWriteChar(line, *stringCopy, (i * 10));
		// Advance to next character.
		stringCopy = stringCopy + 1;
		
		i++;
	}
}  

