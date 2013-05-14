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


__IO uint32_t u32PWDU_WakeFlag = 0;
__IO uint32_t u32WDT_Ticks = 0;


/**
  * @brief  WDT_IRQ Handler. 
  * @param  None.
  * @return None.
  */
void WDT_IRQHandler(void)
{
	__IO uint32_t u32IntSts;

	u32IntSts = WDT->ISR;

	if (u32IntSts & WDT_ISR_WDTWAKEIS)
		printf("     WDT Wake Up Interrupt Occurs.\n");
	else
		printf("     WDT Interrupt Occurs <%d>.\n", u32WDT_Ticks);

	if (u32IntSts & WDT_ISR_WDTIS)
		u32WDT_Ticks++;

	WDT->ISR = u32IntSts; /* clear interrupts */
}


/**
  * @brief  PDWU_IRQ Handler. 
  * @param  None.
  * @return None.
  */
void PDWU_IRQHandler(void)
{
  printf("     PDWU_IRQHandler running...\n");

  u32PWDU_WakeFlag = 1;
  CLK->WK_INTSTS = CLK_WK_INTSTS_IS; /* clear interrupt */
  
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
