#ifndef __LED_CONTROLLER_H__
#define __LED_CONTROLLER_H__

#include <stdint.h>
#include "i2c.h"
#include "gpio.h"

#define PB2       (1<<2)
#define PB3       (1<<3)

//*****************************************************************************
// Fill out the #defines below to configure which pins are connected to
// the I2C Bus
//*****************************************************************************
#define   IO_I2C_GPIO_BASE      GPIOB_BASE
#define   IO_I2C_BASE           I2C0_BASE
#define   IO_I2C_SCL_PIN        PB2
#define   IO_I2C_SDA_PIN        PB3
#define   IO_I2C_SCL_PIN_PCTL   GPIO_PCTL_PB2_I2C0SCL
#define   IO_I2C_SDA_PIN_PCTL   GPIO_PCTL_PB3_I2C0SDA

#define   LED_DRIVER_GPIO_BASE  GPIOB_BASE
#define   LED_DRIVER_GPIO_PERIH GPIOB
#define   LED_DRIVER_GPIO_PIN   PB6

//*****************************************************************************
//*****************************************************************************
void ledController_init(uint32_t  i2c_base);


//*****************************************************************************
//*****************************************************************************
i2c_status_t led_controller_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   reg,
  uint8_t   data
);

#endif