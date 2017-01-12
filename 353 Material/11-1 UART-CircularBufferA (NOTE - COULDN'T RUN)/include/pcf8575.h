#ifndef __PCF8575_H__
#define __PCF8575_H__


#include <stdint.h>
#include "i2c.h"

#define PCF8575_DEV_ID         0x70

i2c_status_t pcf8575_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   data
);

#endif
