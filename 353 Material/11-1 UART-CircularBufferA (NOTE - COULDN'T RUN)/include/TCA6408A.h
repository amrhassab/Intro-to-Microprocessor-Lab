#ifndef __TCA6408A_H__
#define __TCA6408A_H__


#include <stdint.h>
#include "i2c.h"


#define TCA6408A_DEV_ID         0x40

#define TCA6408A_INPUT_PORT_R       0x00
#define TCA6408A_OUTPUT_PORT_R      0x01
#define TCA6408A_POLARITY_R         0x02
#define TCA6408A_CONFIGURATION_R    0x03


i2c_status_t tca6408A_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   regAddress,
  uint8_t   data
);

i2c_status_t tca6408A_byte_read
( 
  uint32_t  i2c_base,
  uint8_t  address,
  uint8_t   *data
);

i2c_status_t tca6408A_set_as_output
( 
  uint32_t  i2c_base
);

i2c_status_t tca6408A_set_as_input
( 
  uint32_t  i2c_base
);

#endif
