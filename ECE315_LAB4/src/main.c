//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "boardUtil.h"

#include "drv8833.h"

#include "interrupts.h"
#include "adc.h"
#include "encoders.c"

#include "lcd.h"
#include "led_controller.h"

#define FW_VERSION  "0.0.1"

// 147 microseconds/inch
#define AN_SONAR_SCALE_FACTOR .00098
#define AN_SONAR_SCALE_mV 9.766

// LED stuff.
#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define RED 0
#define GREEN 1
#define YELLOW 2

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
	drv8833_gpioInit(); 
	encodersInit();
	UartInit();
	ece315_lcdInit();
	ledController_init(IO_I2C_BASE);
}

//*****************************************************************************
//*****************************************************************************
void setLEDColor(int color, int side)
{
	if (side == LEFT)
	{
		if (color == RED)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x07, 0xFF); // R
			led_controller_byte_write(IO_I2C_BASE, 0x08, 0x00); // G
			led_controller_byte_write(IO_I2C_BASE, 0x09, 0x00); // B
		}
		else if (color == GREEN)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x07, 0x00); // R
			led_controller_byte_write(IO_I2C_BASE, 0x08, 0xFF); // G
			led_controller_byte_write(IO_I2C_BASE, 0x09, 0x00); // B
		}
		else if (color == YELLOW)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x07, 0xFF); // R
			led_controller_byte_write(IO_I2C_BASE, 0x08, 0xFF); // G
			led_controller_byte_write(IO_I2C_BASE, 0x09, 0x00); // B
		}
		// Write all the configuration data to the registers
		led_controller_byte_write(IO_I2C_BASE, 0x10, 0x00);
	}
	else if (side == CENTER)
	{
		if (color == RED)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0A, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0B, 0x00);
			led_controller_byte_write(IO_I2C_BASE, 0x0C, 0x00);
		}
		else if (color == GREEN)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0A, 0x00);
			led_controller_byte_write(IO_I2C_BASE, 0x0B, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0C, 0x00);
		}
		else if (color == YELLOW)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0A, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0B, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0C, 0x00);
		}
		// Write all the configuration data to the registers
		led_controller_byte_write(IO_I2C_BASE, 0x10, 0x00);
	}
	else if (side == RIGHT)
	{
		if (color == RED)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0D, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0E, 0x00);
			led_controller_byte_write(IO_I2C_BASE, 0x0F, 0x00);
		}
		else if (color == GREEN)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0D, 0x00);
			led_controller_byte_write(IO_I2C_BASE, 0x0E, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0F, 0x00);
		}
		else if (color == YELLOW)
		{
			led_controller_byte_write(IO_I2C_BASE, 0x0D, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0E, 0xFF);
			led_controller_byte_write(IO_I2C_BASE, 0x0F, 0x00);
		}
		// Write all the configuration data to the registers
		led_controller_byte_write(IO_I2C_BASE, 0x10, 0x00);
	}
}


	

int main(void)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	char distanceString[10];
	char directionString[9];
	
	int fiftyMicroSecondCount = 0;
	int tenMilliSecondCount = 0;
	int secondCount, twoSecondCount, secondCount2 = 0, secondCount3 = 0, secondCount4 = 0;

	bool leftTurn = true;
	bool rightTurn = false;
	
	// leader = true -> C leading; leader = false -> F leading
	bool leader = false; 
	
	bool firstTime = true;
	float numPulsesNextLCDUpdate = 0;
	
	// object in the way
	volatile bool stop = false;
	
	int dataLeftToTake = 0;
	int leftData = 0;
	int runningDataSum = 0;
	
	uint8_t phase = 0;
	uint8_t nextPhase = 3;
	uint8_t phaseCounter = 0;
	
	float distanceCovered, totalDistanceCovered = 0.0;
	
	uint8_t numberOfTotalSeconds = 0;
	
	int dataVal = 0;
	int highCount = 0;
	int adcVal = 0;
	float inches, inches_adc, inch_sum, inchesOut = 0;
	int numOf49SecondIncrements = 0;
	float numberOfMilliSeconds = 0;
	uint8_t dutyCycle;

	bool thisistrue = true;
	bool sensorNOTsporatic = true;
  initializeBoard();

  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE315 Base Project                 \n\r");
  printf("* FW Version:%s\n\r",FW_VERSION);
  printf("**************************************\n\r");
	
	distanceString[0] = 'D';
	distanceString[1] = 'I';
	distanceString[2] = 'S';
	distanceString[3] = 'T';
	distanceString[4] = ':';
	distanceString[5] = ' ';
	directionString[0] = 'D';
	directionString[1] = 'I';
	directionString[2] = 'R';
	directionString[3] = ':';
	directionString[4] = ' ';
	
	while(1)
	{
		// A valid distance is read form left sensor.
		if (validDistance)
		{
			validDistance = false;
			// Parse the UART's data.
			if (distance == 'R')
			{
				// Take next three pieces of data, these will be the value.
				dataLeftToTake = 3;
				runningDataSum = 0;
			}
			else if (dataLeftToTake == 3)
			{
				if (distance != 13){ 
					runningDataSum += (100 * (distance - 48));
				}
					dataLeftToTake--;
			}
			else if (dataLeftToTake == 2)
			{
				if (distance != 13)
				{
					runningDataSum += (10 * (distance - 48));
				}
				dataLeftToTake--;
			}
			else if (dataLeftToTake == 1)
			{ 
				if (distance != 13)
				{
					runningDataSum += (1 * (distance - 48));
				}
				dataLeftToTake--;
				
				leftData = runningDataSum;
			}
		}
		
		// REMEMBER TO CHECK THE DISTANCES AND THE TIMES!!!!!!!!!!!!!
		switch(phase)
		{
						case 0:															// Stop all motors
							drv8833_stop();
							if(secondCount2 == 20000) // 1 second
							{
								phase = nextPhase;
								secondCount2 = 0;
								countEveryFour = 0;
								
							}
							break;
						case 1:		
							if(secondCount2 < 19500 && secondCount2 > 7200)
							{  // turn left for 1 second
								
								directionString[5] = 'L';
								directionString[6] = 'T';
  							directionString[7] = 'R';
    					  directionString[8] = 'N';
								
								drv8833_leftReverse(95);
								drv8833_rightForward(95);
								
							}
							else if (secondCount2 == 21000)
							{
								phase = nextPhase;
								secondCount2 = 0;
								countEveryFour = 0;
							}

							break;	
						case 2:
							if(secondCount2 < 19500 && secondCount2 > 7200)
							{  // turn right for 0.5 second
								
								directionString[5] = 'R';
								directionString[6] = 'T';
  							directionString[7] = 'R';
    					  directionString[8] = 'N';

								drv8833_rightReverse(90);
								drv8833_leftForward(90);

							}
							else if (secondCount2 == 21000)
							{
								phase = nextPhase;
								secondCount2 = 0;
								countEveryFour = 0;
							}

							break;
						case 3:															// Go forwards 94 inches
						if (firstTime)
							{
								pulseCount = pulsesPerDistance(94);
								
								// Set the first time we will have travelled an inch.
								numPulsesNextLCDUpdate = pulseCount - PULSES_PER_INCH;
								
								// Don't enter here again.
								firstTime = false;
							}
							
							// If we have moved another inch.
							if (pulseCount <= numPulsesNextLCDUpdate)
							{
								// Update dist travelled, we went another inch.
								totalDistanceCovered += 1;
								
								// Update when we will have travelled another inch.
								numPulsesNextLCDUpdate -= PULSES_PER_INCH;
							}
							
							// Update display.
							directionString[5] = 'F';
							directionString[6] = 'W';
  						directionString[7] = 'D';
    					directionString[8] = ' ';
							
							// if object is in front of robot, don't move robot...
							if (sensorNOTsporatic && stop)
							{
								drv8833_stop();
								return(0);
							}else
							{
								// Make it go straight.
								drv8833_leftForward(90);
								drv8833_rightForward(90);
							}

							if (pulseCount <= 0)
							{
								phase = 1;
								nextPhase = 4;
								secondCount2 = 0;
								firstTime = true;
							}
							
							break;
							case 4:															// Go forwards 55 inches
								if (firstTime)
								{
										pulseCount = pulsesPerDistance(55);

										// Set the first time we will have travelled an inch.
										numPulsesNextLCDUpdate = pulseCount - PULSES_PER_INCH;
								
										// Don't enter here again.
										firstTime = false;
								}

									// If we have moved another inch.
								if (pulseCount <= numPulsesNextLCDUpdate)
								{
										// Update dist travelled, we went another inch.
										totalDistanceCovered += 1;
								
										// Update when we will have travelled another inch.
										numPulsesNextLCDUpdate -= PULSES_PER_INCH;
									
								
								}

									// Update display.
								directionString[5] = 'F';
								directionString[6] = 'W';
								directionString[7] = 'D';
								directionString[8] = ' ';
						
								// if object is in front of robot, don't move
								if (sensorNOTsporatic && stop)
								{
									drv8833_stop();
									return(0);
								}else
								{
									// Make it go straight.
									drv8833_leftForward(90);
									drv8833_rightForward(90);
								}

								if (pulseCount <= 0)
								{
										phase = 1;
										nextPhase = 5;
										secondCount2 = 0;
										firstTime = true;
								}
							
							break;
						case 5:								 

							// Get number of pulses it will take to travel 47 inches.
							if (firstTime)
							{
								pulseCount = pulsesPerDistance(47);
								
								// Set the first time we will have travelled an inch.
								numPulsesNextLCDUpdate = pulseCount - PULSES_PER_INCH;
								
								// Don't enter here again.
								firstTime = false;
							}
							
							// If we have moved another inch.
							if (pulseCount <= numPulsesNextLCDUpdate)
							{
								// Update dist travelled, we went another inch.
								totalDistanceCovered += 1;
								
								// Update when we will have travelled another inch.
								numPulsesNextLCDUpdate -= PULSES_PER_INCH;
							}

							// Update display.
							directionString[5] = 'F';
							directionString[6] = 'W';
  						directionString[7] = 'D';
    					directionString[8] = ' ';
						
							// if object is in front of robot, don't move
							if (sensorNOTsporatic && stop)
							{
								drv8833_stop();
								return(0);
							}else
							{
								// Make it go straight.
								drv8833_leftForward(90);
								drv8833_rightForward(90);
							}

							// Go to next case when the pulses run out.
							if (pulseCount <= 0)
							{
								phase = 2;
							  nextPhase = 6;
								secondCount2 = 0;
								firstTime = true;
							}
							
							break;
							
							
						case 6:															// Go forwards 137 inches
							// Get number of pulses it will take to travel 137 inches.
							if (firstTime)
							{ //printf("true");
								pulseCount = pulsesPerDistance(137);
								
								// Set the first time we will have travelled an inch.
								numPulsesNextLCDUpdate = pulseCount - PULSES_PER_INCH;
								
								// Don't enter here again.
								firstTime = false;
							}

							// If we have moved another inch.
							if (pulseCount <= numPulsesNextLCDUpdate)
							{
								// Update dist travelled, we went another inch.
								totalDistanceCovered += 1;
								
								// Update when we will have travelled another inch.
								numPulsesNextLCDUpdate -= PULSES_PER_INCH;
							}
							
							// Update display.
							directionString[5] = 'F';
							directionString[6] = 'W';
  						directionString[7] = 'D';
    					directionString[8] = ' ';
						
							// if object is in front of robot, don't move
							if (sensorNOTsporatic && stop)
							{
								drv8833_stop();
								return(0);
							}else
							{
								// Make it go straight.
								drv8833_leftForward(90);
								drv8833_rightForward(90);
							}
								
								if (pulseCount <= 0)
							{
								phase = 2;
								nextPhase = 7;
								secondCount2 = 0;
								firstTime = true;
							}

							break;
							
						case 7:
							// Get number of pulses it will take to travel 80 inches.
							if (firstTime)
							{
								pulseCount = pulsesPerDistance(80);
								
								// Set the first time we will have travelled an inch.
								numPulsesNextLCDUpdate = pulseCount - PULSES_PER_INCH;
								
								// Don't enter here again.
								firstTime = false;
							}
	
							// If we have moved another inch.
							if (pulseCount <= numPulsesNextLCDUpdate)
							{
								// Update dist travelled, we went another inch.
								totalDistanceCovered += 1;
								
								// Update when we will have travelled another inch.
								numPulsesNextLCDUpdate -= PULSES_PER_INCH;
							}
							
							// Update display.
							directionString[5] = 'F';
							directionString[6] = 'W';
  						directionString[7] = 'D';
    					directionString[8] = ' ';
						
							if(sensorNOTsporatic && stop)
							{
								drv8833_stop();
								return(0);
							}else
							{
								// Make it go straight.
								drv8833_leftForward(90);
								drv8833_rightForward(90);
							}
								
							if (pulseCount <= 0)
							{ 
									phase = 0;
									nextPhase = 0;
									secondCount2 = 0;
								  firstTime = true;
								
							}
						break;
							
						default:
						break;
		}
		
		// Loop runs every 50 micro seconds
		if (sysTickUpdate)
		{
			// Disable interrupt flag.
			sysTickUpdate = false;
			// See if pulse width is high. If so, increment count.
			dataVal = (GPIOE->DATA & PE2);
			if (dataVal != 0)
			{
				highCount++;
			}
			
			if((secondCount == 5000) || (secondCount == 10000))
				{
					if (leftData <= 8)
					{
						// Make center LED red.
						setLEDColor(RED, LEFT);
					}
					else if (leftData <= 15)
					{
						// Make center LED yellow.
						setLEDColor(YELLOW, LEFT);
					}
					else
					{
						// Make center LED green.
						setLEDColor(GREEN, LEFT);
					}
				
					if (inches_adc <= 8)
						{ 
							stop = true;
							// Make center LED red.
							setLEDColor(RED, CENTER);
							// stop the robot ************** need to implement

						}
						else if (inches_adc <= 15)
						{
							stop = false;
							// Make center LED yellow.
							setLEDColor(YELLOW, CENTER);
						}
						else
						{
							stop = false;
							// Make center LED green.
							setLEDColor(GREEN, CENTER);
						}
						
						inch_sum = 0;
						numOf49SecondIncrements = 0;
						
						if (inches <= 8)
						{
							// Make center LED red.
							setLEDColor(RED, RIGHT);
						}
						else if (inches <= 15)
						{
							// Make center LED yellow.
							setLEDColor(YELLOW, RIGHT);
						}
						else
						{
							// Make center LED green.
							setLEDColor(GREEN, RIGHT);
						}
				}
				if (secondCount == 5000)
				{
					secondCount = 0;
					// Print new data to the LCD screen every 1.2 seconds
					sprintf((distanceString + 6), "%f", totalDistanceCovered);
					// Write the new data to the LCD.
					ece315_lcdClear();
					ece315_lcdWriteString(0, distanceString);
					ece315_lcdWriteString(1, directionString);
				}	
				
				if (tenMilliSecondCount == 200)
				{
					tenMilliSecondCount = 0;
					// Acquire center sensor data.
					adcVal = getADCValue(ADC0_BASE, 0);	
					inches_adc = (float)adcVal/ AN_SONAR_SCALE_mV;
				}
			
				// Run every 49 milliseconds.
				if (fiftyMicroSecondCount == 980)
				{
					fiftyMicroSecondCount = 0;
					numOf49SecondIncrements++;
					// Get the number of milliseconds the pulse was high for.
					numberOfMilliSeconds = 0.05 * highCount;

					// Use: (1 inch / 147 micro second).
					inches = (1000.0 / 147.0) * numberOfMilliSeconds;
					inch_sum = inch_sum + inches;
					inchesOut = inch_sum/numOf49SecondIncrements;
					highCount = 0;
				}

			secondCount4++;
			secondCount3++;
			secondCount2++;
			secondCount++;
			tenMilliSecondCount++;
			fiftyMicroSecondCount++;			
		} // systickupdate		
	} // while
} // main

