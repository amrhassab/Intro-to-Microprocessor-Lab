#ifndef __MPR121_H__
#define __MPR121_H__


#include <stdint.h>
#include "i2c.h"


#define PMR121_DEV_ID         (0x5A << 1)

#define PMR121_TOUCH_STATUS_07_00_R       0x00
#define PMR121_TOUCH_STATUS_11_08_R       0x01
#define PMR121_FILTER_GLOBAL_CDT_R        0x5D


i2c_status_t pmr121_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   address,
  uint8_t   data
);

i2c_status_t pmr121_byte_read
( 
  uint32_t  i2c_base,
  uint8_t  address,
  uint8_t   *data
);


#endif
