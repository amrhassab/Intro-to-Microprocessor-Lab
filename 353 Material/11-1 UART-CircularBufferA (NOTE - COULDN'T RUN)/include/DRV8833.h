#ifndef __DRV8833_H__
#define __DRV8833_H__

#include "TM4C123.h"
#include "pwm.h"

#define PWM_LOAD_VAL    10000
#define PWM_CHANNEL_PWM   (PWM_GEN_ACTCMPAD_LOW | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_LOW   (PWM_GEN_ACTCMPBD_LOW | PWM_GEN_ACTLOAD_LOW | PWM_GEN_ACTZERO_NOTHING)
#define PWM_CHANNEL_HIGH  (PWM_GEN_ACTCMPBD_HIGH | PWM_GEN_ACTLOAD_HIGH | PWM_GEN_ACTZERO_NOTHING)

typedef struct {
  uint32_t    leftBaseAddr;
  uint8_t     leftPwmGenerator;
  uint32_t    rightBaseAddr;
  uint32_t    rightPwmGenerator;
} DRV8833_CONFIG;

/******************************************************************************
 *  
 *****************************************************************************/
 void configureMotorControl(
  uint32_t    leftBaseAddr,
  uint8_t     leftPwmGenerator,
  uint32_t    rightBaseAddr,
  uint32_t    rightPwmGenerator
);
 
/******************************************************************************
 *  
 *****************************************************************************/
void leftForward(uint8_t  dutyCyclePercentOn );

/******************************************************************************
 *  
 *****************************************************************************/
void rightForward(uint8_t  dutyCyclePercentOn );

/******************************************************************************
 *  
 *****************************************************************************/
void leftReverse(uint8_t  dutyCyclePercentOn);

/******************************************************************************
 *  
 *****************************************************************************/
void rightReverse(uint8_t  dutyCyclePercentOn);

/******************************************************************************
 *  
 *****************************************************************************/
void rightCoast(void);


/******************************************************************************
 *  
 *****************************************************************************/
void leftCoast(void);

#endif
