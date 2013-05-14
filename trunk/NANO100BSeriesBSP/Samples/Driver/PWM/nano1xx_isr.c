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

extern int32_t	volatile g_bCapInt;
extern uint8_t volatile g_u8PWMCount;
extern uint16_t g_u16Frequency;
extern uint32_t g_u32Pulse;
extern uint8_t u8CapTimer;
extern int32_t	volatile pdma_done;


void PWM0_IRQHandler(void)
{
        uint32_t reg;


        reg = PWM0->INTSTS;

        if(reg & 1) {	// channel 0 interrupt
		if (g_u32Pulse == 1 * g_u16Frequency /10) {
			/*--------------------------------------------------------------------------------------*/
			/* Stop PWM Timer 0 (Recommended procedure method 2)		                            */
			/* Set PWM Timer counter as 0, When interrupt request happen, disable PWM Timer			*/
			/*--------------------------------------------------------------------------------------*/
			PWM_SetCounter(0, 0, 0);
		}

		if (g_u32Pulse == 1 * g_u16Frequency /10 + 1)
			g_u8PWMCount = 0;
		g_u32Pulse++;
        }

        PWM0->INTSTS = reg;

        reg = PWM0->CAPINTSTS;


	if(reg & PWM_CAPINTSTS_CAPIF3) { 	// channel 3 interrupt
		if(reg & PWM_CAPINTSTS_CFLI3)
			g_bCapInt = 1;
		else if(reg & PWM_CAPINTSTS_CRLI3)
			g_bCapInt = 2;

	}

        PWM0->CAPINTSTS = reg;

        return;
}

// Not used in this sample code
void PWM1_IRQHandler(void)
{
        uint32_t reg;


        reg = PWM1->INTSTS;
        PWM1->INTSTS = reg;

        reg = PWM1->CAPINTSTS;
        PWM1->CAPINTSTS = reg;

        return;
}

/**
  * @brief  PDMA_IRQHandler.
  * @param  None.
  * @retval None.
  */
void PDMA_IRQHandler(void)
{
	PDMA1->ISR = PDMA1->ISR;
	pdma_done = 1;

}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
