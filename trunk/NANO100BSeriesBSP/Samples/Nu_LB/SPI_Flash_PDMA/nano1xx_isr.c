
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
#include <string.h>
#include "nano1xx.h"

extern uint32_t volatile PDMA1_INT_Flag;
extern uint32_t volatile PDMA2_INT_Flag;

void PDMA_IRQHandler(void)
{
	uint32_t u32ISR;

	u32ISR = PDMAGCR->ISR;
	if (u32ISR & 0x2)
	{				
		PDMA1->ISR = 0x02;		//clear flag
		PDMA1_INT_Flag = 1;
	}
	else if(u32ISR & 0x4)
	{
		PDMA2->ISR = 0x02;		//clear flag
		PDMA2_INT_Flag = 1;
	}
}
		
/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
