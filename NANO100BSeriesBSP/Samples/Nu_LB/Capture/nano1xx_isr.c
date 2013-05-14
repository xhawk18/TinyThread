/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/


#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_pwm.h"

extern uint32_t	CaptureValue[2];

void PWM0_IRQHandler(void) // PWM interrupt subroutine 
{
	if(PWM0->CAPINTSTS | PWM_CAPINTSTS_CAPIF0)
	{
		if(PWM0->CAPINTSTS| PWM_CAPINTSTS_CFLI0)//Calculate High Level
		{
			CaptureValue[0]=(0xFFFF - PWM0->CFL0);//usec
			PWM0->CAPINTSTS = PWM_CAPINTSTS_CFLI0;
		}
		if(PWM0->CAPINTSTS | PWM_CAPINTSTS_CRLI0)//Calculate Low Level
		{
			CaptureValue[1]=(0xFFFF - PWM0->CRL0);//usec
			PWM0->CAPINTSTS = PWM_CAPINTSTS_CRLI0;	
		}
		PWM0->CAPINTSTS = PWM_CAPINTSTS_CAPIF0;
	}
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
