
/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_timer.h"

extern uint32_t volatile u32Timer0Cnt, u32Timer1Cnt, u32Timer2Cnt, u32Timer3Cnt;
extern int volatile wwdt_cnt;
extern uint32_t u32CounterINT;
extern uint8_t volatile b8WDTINT;


void TMR0_IRQHandler(void)
{
	TIMER0->ISR = 3;
	u32Timer0Cnt++;
	u32CounterINT++;
}

void TMR1_IRQHandler(void)
{
	TIMER1->ISR = 3;
	u32Timer1Cnt++;
	u32CounterINT++;
}

void TMR2_IRQHandler(void)
{
	TIMER2->ISR = 3;
	u32Timer2Cnt++;
	u32CounterINT++;
}

void TMR3_IRQHandler(void)
{
	TIMER3->ISR = 3;
	u32Timer3Cnt++;
	u32CounterINT++;
}
// WDT and WWDT share a single IQR handler
void WDT_IRQHandler(void)
{

        uint32_t reg;

        if((WDT->ISR & 1) && (WDT->IER & 1)) {
                reg = WDT->ISR;

		b8WDTINT = TRUE;
		WDT_RESET_TIMER;
		printf("  WDT interrupt !!!\n");

                WDT->ISR = reg;
        }

        if((WWDT->STS & 1) && (WWDT->IER & 1)) {
		WWDT_RELOAD;
                printf("Int %d\n", ++wwdt_cnt);
                WWDT->STS = 1;

        }


        return;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
