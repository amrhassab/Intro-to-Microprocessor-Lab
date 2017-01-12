#include "led_controller.h"

//*****************************************************************************
//*****************************************************************************
static void i2cIOInit(void)
{
  
  I2C_CONFIG  i2cIO_config = {IO_I2C_BASE };
  
  // Configure SCL 
  gpioConfigDigitalEnable(IO_I2C_GPIO_BASE, IO_I2C_SCL_PIN);
  gpioConfigAltFunction(IO_I2C_GPIO_BASE, IO_I2C_SCL_PIN);
  gpioConfigPortConfig(IO_I2C_GPIO_BASE, IO_I2C_SCL_PIN_PCTL);
  
  // Configure SDA 
  gpioConfigDigitalEnable(IO_I2C_GPIO_BASE, IO_I2C_SDA_PIN);
  gpioConfigOpenDrainEnable(IO_I2C_GPIO_BASE, IO_I2C_SDA_PIN);
  gpioConfigAltFunction(IO_I2C_GPIO_BASE, IO_I2C_SDA_PIN);
  gpioConfigPortConfig(IO_I2C_GPIO_BASE, IO_I2C_SDA_PIN_PCTL);
 
  initializeI2CMaster(&i2cIO_config);

}

//*****************************************************************************
//*****************************************************************************
i2c_status_t led_controller_byte_write
( 
  uint32_t  i2c_base,
  uint8_t   reg,
  uint8_t   data
)
{
  i2c_status_t status;
  
  // Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(i2c_base)) {};
      
  // Set the I2C address 
  status = i2cSetSlaveAddr(i2c_base, 0xC8, I2C_WRITE);
  
  if ( status != I2C_OK )
  {
    return status;
  }
  
  // Send the register address
  status = i2cSendByte( i2c_base, reg , I2C_MCS_START | I2C_MCS_RUN);
  
  if(status != I2C_OK)
  {
    return status;
  }
  
  // Send the data
  status = i2cSendByte( i2c_base, data , I2C_MCS_RUN | I2C_MCS_STOP);

  return status;

}

//*****************************************************************************
//*****************************************************************************
void ledController_init
( 
  uint32_t  i2c_base
)
{
  i2cIOInit();
  
  // Bring the device out of reset
  led_controller_byte_write(i2c_base, 0x00, 0x01);
  led_controller_byte_write(i2c_base, 0x10, 0x00);
   
  // Enable all 9 LEDs
  led_controller_byte_write(i2c_base, 0x01, 0x77);
  led_controller_byte_write(i2c_base, 0x02, 0x07);
  led_controller_byte_write(i2c_base, 0x10, 0x00);
  
  // Turn on PWM Mode, disable Audio Modulate, Enable AGCE, AGCM = Fast
  led_controller_byte_write(i2c_base, 0x03, 0x00);

  // Configure Device as master, 20mA Current Sensing, 0DB audio gain
  led_controller_byte_write(i2c_base, 0x04, 0x00);
  
  // Disable Ramp Mode
  led_controller_byte_write(i2c_base, 0x05, 0x00);
  
  // Disable Breathing Mark
  led_controller_byte_write(i2c_base, 0x06, 0x00);
  
  // Set the PWM rates of each of the 9 LEDs
  led_controller_byte_write(i2c_base, 0x07, 0x00);
  led_controller_byte_write(i2c_base, 0x08, 0x00);
  led_controller_byte_write(i2c_base, 0x09, 0x00);
  
  led_controller_byte_write(i2c_base, 0x0A, 0xFF);
  led_controller_byte_write(i2c_base, 0x0B, 0x00);
  led_controller_byte_write(i2c_base, 0x0C, 0x00);
  
  led_controller_byte_write(i2c_base, 0x0D, 0x00);
  led_controller_byte_write(i2c_base, 0x0E, 0x00);
  led_controller_byte_write(i2c_base, 0x0F, 0x00);
  
  // Write all the configureation data to the registers
  led_controller_byte_write(i2c_base, 0x10, 0x00);
}
