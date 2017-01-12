//*****************************************************************************
// encoders.c
// Author: Mike, Nev, Steve
//*****************************************************************************\

#include <stdio.h>
#include <stdint.h>
#include <string.h>




// fixed number of inches we measured
// total: 204, 186, 191, 195
#define PULSES_PER_INCH 8.255


/******************************************************************************
 * Global Variables
 *****************************************************************************/

// enter the distance (in inches) for which you want to know the pulse count
int pulsesPerDistance(int distance){
	return distance * PULSES_PER_INCH;
}


 /*****************************************************************************/