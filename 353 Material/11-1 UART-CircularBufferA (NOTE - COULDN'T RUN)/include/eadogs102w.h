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

#ifndef __EADOGS102W_H__
#define __EADOGS102W_H__

#include <stdint.h>
#include "spi.h"
#include "fonts.h"

#define NUM_PAGES   8
#define NUM_COLS    102

typedef struct {
  uint32_t  spiBase;
  uint32_t  resetPinBase;
  uint8_t   resetPinNum;
  uint32_t  cmdPinBase;
  uint8_t   cmdPinNum;
} EADOGS102W_CONFIG;


//*****************************************************************************
//*****************************************************************************
void lcd_set_pin_config (
  uint32_t  spiBase, 
  uint32_t  resetPinBase, 
  uint8_t  resetPinNum,
  uint32_t  cmdPinBase,
  uint8_t  cmdPinNum
) ;

//*****************************************************************************
//*****************************************************************************
bool lcdInitialize(void);

  //*****************************************************************************
//*****************************************************************************
  bool lcdSetPage(uint8_t   page);
  
//*****************************************************************************
//*****************************************************************************
bool lcdSetColumn(uint8_t   column);
  

//*****************************************************************************
//*****************************************************************************
  bool lcdWriteData(uint8_t   data);

//*****************************************************************************
//*****************************************************************************
 bool lcdWriteChar( uint8_t page, char c);

//*****************************************************************************
//*****************************************************************************
 bool lcdWriteChar_8pts( uint8_t page, char c, uint8_t colStart);
 

//*****************************************************************************
//*****************************************************************************
 bool lcdWriteChar_10pts( uint8_t page, char c, uint8_t colStart);
 
 //*****************************************************************************
//*****************************************************************************
 void lcdClear(void);
 

//*****************************************************************************
//*****************************************************************************
  void lcdWriteString_10pts( uint8_t line, char *string);
 
#endif
