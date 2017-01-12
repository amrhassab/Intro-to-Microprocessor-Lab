// Copyright (c) 2014, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __ECE353_CIRCULAR_BUFFER__
#define __ECE353_CIRCULAR_BUFFER__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    volatile uint32_t tailIndex;
    volatile int32_t size;
    volatile int32_t count;
    volatile char *buffer;
} CircularBuffer ;


/******************************************************************************
 * cBufInit
 *
 * Description:
 *		Allocates size bytes from the heap and initializes the circular buffer
 *      to be empty.
 *
 *  Retuns 0 on success and a negative number for failure.
 *****************************************************************************/
int32_t		cBufInit(CircularBuffer *bufPtr, int16_t size);

/******************************************************************************
 * cBufAddChar
 *
 * Description:
 *		Adds a character to a circular buffer.  
 *
 *      If there is room in the circular buffer, the character is added to 
 *      the buffer and 0 is returned
 *
 *      If the buffer is full, the oldest entry in the circular buffer is 
 *      overwritten and 1 is returned
 *
 *      All other error conditions should return a negative value.
 *****************************************************************************/
int32_t		cBufAddChar(CircularBuffer *bufPtr, char c);

/******************************************************************************
 * cBufGetChar:
 *		returns the oldest character from the circular buffer.  The head/tail
 *    indexes are updated to reflect that the character was removed from
  *   the circular buffer.  
 *
 *      If there is a character to return, the return value is 0.  
 *
 *      If the circular is empty, it will return a positive number
 *
 *      All other error conditions should return a negative number
 *****************************************************************************/
int32_t		cBufGetChar(CircularBuffer *bufPtr, char *retChar); 

/******************************************************************************
 * cBufGetFreeCount
 *		Returns the number of unused entries in the circular buffer
 *
 *      All other error conditions should return a negative number
 *****************************************************************************/
int32_t		cBufGetFreeCount(CircularBuffer *bufPtr);

#endif
