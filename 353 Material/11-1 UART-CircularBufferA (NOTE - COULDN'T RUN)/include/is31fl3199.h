#ifndef __IS31FL3199_H__
#define __IS31FL3199_H__


#include <stdint.h>
#include "i2c.h"

#define PCF8575_DEV_ID         0x70

i2c_status_t IS31FL3199_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   reg,
  uint8_t   data
);

i2c_status_t IS31FL3199_init
( 
  uint32_t  i2c_base
);

#endif
