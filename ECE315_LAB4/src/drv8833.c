#include "drv8833.h"

#define PWM_LOAD_VAL    10000
#define PWM_CHANNEL_PWM   (PWM_GEN_ACTCMPAD_LOW | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_LOW   (PWM_GEN_ACTCMPBD_LOW | PWM_GEN_ACTLOAD_LOW | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_HIGH  (PWM_GEN_ACTCMPBD_HIGH | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)

uint32_t compareValue = 0;

//*****************************************************************************
// Initializes the 6 pins needed to control the DRV8833
//*****************************************************************************
void  drv8833_gpioInit(void)
{
		gpioEnablePort(GPIOB_BASE);
		gpioEnablePort(GPIOE_BASE);
		
		gpioConfigDigitalEnable(GPIOB_BASE, PB4 | PB5);
		gpioConfigDigitalEnable(GPIOE_BASE, PE4 | PE5);
	
		gpioConfigPinAsOutput(GPIOB_BASE, PB4 | PB5);
		gpioConfigPinAsOutput(GPIOE_BASE, PE4 | PE5);
	
		gpioConfigAltFunction(GPIOB_BASE, PB4 | PB5);
		gpioConfigAltFunction(GPIOE_BASE, PE4 | PE5);
	
		gpioConfigPortConfig(GPIOB_BASE, GPIO_PCTL_PB4_M0PWM2 | GPIO_PCTL_PB5_M0PWM3);
		gpioConfigPortConfig(GPIOE_BASE, GPIO_PCTL_PE4_M1PWM2 | GPIO_PCTL_PE5_M1PWM3);
	
		gpioEnablePort(GPIOF_BASE);

		gpioConfigDigitalEnable(GPIOF_BASE, PF2 | PF3);
		gpioConfigPinAsInput(GPIOF_BASE, PF2);
		gpioConfigPinAsOutput(GPIOF_BASE, PF3);
	
		// When PF3 = 1, motors are able to run. When PF3 = 0, nope.
		GPIOF->DATA |= PF3;
		//turn on clock for PWM modules 0 and 1
		SYSCTL->RCGCPWM |= 0x03; 
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftForward(uint8_t dutyCycle)
{
	compareValue = PWM_LOAD_VAL - (uint32_t)((PWM_LOAD_VAL * (dutyCycle/100.00) * 10.3));
  pwmConfig(
		PWM0_BASE,
		1, 
		PWM_LOAD_VAL, 
		compareValue,
		compareValue,
		PWM_CHANNEL_PWM,
		PWM_CHANNEL_LOW
	);

}

//*****************************************************************************
//*****************************************************************************
void  drv8833_leftReverse(uint8_t dutyCycle)
{
	compareValue =PWM_LOAD_VAL - (uint32_t)((PWM_LOAD_VAL * (dutyCycle/100.00) * 10.3));
	pwmConfig(
		PWM0_BASE,
		1, 
		PWM_LOAD_VAL, 
		compareValue,
		compareValue,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_PWM
	);
}


//*****************************************************************************
//*****************************************************************************
void  drv8833_rightForward(uint8_t dutyCycle)
{
	compareValue =PWM_LOAD_VAL - (uint32_t)((PWM_LOAD_VAL * (dutyCycle/100.00) * 0.93));
  pwmConfig(
		PWM1_BASE,
		1, 
		PWM_LOAD_VAL, 
		compareValue,
		compareValue,
		PWM_CHANNEL_PWM,
		PWM_CHANNEL_LOW
	);
	
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_rightReverse(uint8_t dutyCycle)
{
	compareValue = PWM_LOAD_VAL - (uint32_t)(PWM_LOAD_VAL * (dutyCycle/100.00) * 0.97);
	pwmConfig(
		PWM1_BASE,
		1, 
		PWM_LOAD_VAL, 
		compareValue,
		compareValue,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_PWM
	);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnLeft(uint8_t dutyCycle)
{
  drv8833_leftForward(dutyCycle);	
	drv8833_rightReverse(dutyCycle);
}

//*****************************************************************************
//*****************************************************************************
void  drv8833_turnRight(uint8_t dutyCycle)
{
	drv8833_rightForward(dutyCycle);
	drv8833_leftReverse(dutyCycle);
}

void drv8833_stop()
{
	pwmConfig(
		PWM0_BASE,
		1, 
		PWM_LOAD_VAL, 
		0, 
		0,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_LOW
	);
	
	pwmConfig(
		PWM1_BASE,
		1, 
		PWM_LOAD_VAL, 
		0, 
		0,
		PWM_CHANNEL_LOW,
		PWM_CHANNEL_LOW
	);
}
