#include "pwm.h"
#include "sysctrl.h"
#include "drv8833.h"


// From system_TM4C123.c
#define CFG_RCC_USEPWMDIV 1

//*****************************************************************************
//*****************************************************************************
uint8_t pwmConfig(
  uint32_t base,
  uint8_t pwm_generator, 
  uint32_t load, 
  uint32_t cmpa, 
  uint32_t cmpb,
  uint32_t gena,
  uint32_t genb
)
{
	if(base == PWM0_BASE)
	{
		switch(pwm_generator){
			case 0:
				PWM0->_0_CTL = 0x00000000;
				PWM0->_0_LOAD = load;
			  PWM0->_0_CMPA = cmpa;
				PWM0->_0_CMPB = cmpb;
			  PWM0->_0_GENA = gena;
			  PWM0->_0_GENB = genb;
				PWM0->_0_CTL = 0x00000001;
				PWM0->ENABLE |= 0x03;
			  break;
			case 1:
				PWM0->_1_CTL = 0x00000000;
				PWM0->_1_LOAD = load;
			  PWM0->_1_CMPA = cmpa;
				PWM0->_1_CMPB = cmpb;
			  PWM0->_1_GENA = gena;
			  PWM0->_1_GENB = genb;
				PWM0->_1_CTL = 0x00000001;
				PWM0->ENABLE |= 0x0C;
				break;
			case 2:
				PWM0->_2_CTL = 0x00000000;
				PWM0->_2_LOAD = load;
			  PWM0->_2_CMPA = cmpa;
				PWM0->_2_CMPB = cmpb;
			  PWM0->_2_GENA = gena;
			  PWM0->_2_GENB = genb;
				PWM0->_2_CTL = 0x00000001;
				PWM0->ENABLE |= 0x30;
				break;
			case 3:
				PWM0->_3_CTL = 0x00000000;
				PWM0->_3_LOAD = load;
			  PWM0->_3_CMPA = cmpa;
				PWM0->_3_CMPB = cmpb;
			  PWM0->_3_GENA = gena;
			  PWM0->_3_GENB = genb;
				PWM0->_3_CTL = 0x00000001;
				PWM0->ENABLE |= 0xC0;
				break;
		}
	}
	
	if(base == PWM1_BASE)
	{
		switch(pwm_generator){
			case 0:
				PWM1->_0_CTL = 0x00000000;
				PWM1->_0_LOAD = load;
			  PWM1->_0_CMPA = cmpa;
				PWM1->_0_CMPB = cmpb;
			  PWM1->_0_GENA = gena;
			  PWM1->_0_GENB = genb;
				PWM1->_0_CTL = 0x00000001;
				PWM1->ENABLE |= 0x03;
				break;
			case 1:
				PWM1->_1_CTL = 0x00000000;
				PWM1->_1_LOAD = load;
			  PWM1->_1_CMPA = cmpa;
				PWM1->_1_CMPB = cmpb;
			  PWM1->_1_GENA = gena;
			  PWM1->_1_GENB = genb;
				PWM1->_1_CTL = 0x00000001;
				PWM1->ENABLE |= 0x0C;
				break;
			case 2:
				PWM1->_2_CTL = 0x00000000;
				PWM1->_2_LOAD = load;
			  PWM1->_2_CMPA = cmpa;
				PWM1->_2_CMPB = cmpb;
			  PWM1->_2_GENA = gena;
			  PWM1->_2_GENB = genb;
				PWM1->_2_CTL = 0x00000001;
				PWM1->ENABLE |= 0x30;
				break;
			case 3:
				PWM1->_3_CTL = 0x00000000;
				PWM1->_3_LOAD = load;
			  PWM1->_3_CMPA = cmpa;
				PWM1->_3_CMPB = cmpb;
			  PWM1->_3_GENA = gena;
			  PWM1->_3_GENB = genb;
				PWM1->_3_CTL = 0x00000001;
				PWM1->ENABLE |= 0xC0;
				break;
		}
	}
	return 0;
}
