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

extern uint32_t volatile g_Spi0IntFlag;
extern uint32_t volatile g_Spi1IntFlag;
extern uint32_t volatile g_PdmaIntFlag;

extern int volatile spiIntDone;

/* ToDo: add interrupt service routines. */
void SPI0_IRQHandler(void)
{
	if( SPI0->STATUS & SPI_STATUS_INTSTS ) /* One transaction done interrupt */
    {
		// write '1' to clear SPI0 interrupt flag
		SPI0->STATUS |= SPI_STATUS_INTSTS;
		g_Spi0IntFlag = 1;
	}
}

void SPI1_IRQHandler(void)
{
	if( SPI1->STATUS & SPI_STATUS_INTSTS ) /* One transaction done interrupt */
    {
		// write '1' to clear SPI1 interrupt flag
		SPI1->STATUS |= SPI_STATUS_INTSTS;
		g_Spi1IntFlag = 1;	
	}
}

void PDMA_IRQHandler(void)
{
	uint32_t u32ISR;

	u32ISR = PDMA1->ISR;
	if(u32ISR & 0x02)	  
	{							/* Transfer Done INT and clear */
		PDMA1->ISR = 0x2;	
		g_PdmaIntFlag = TRUE;
	}
	
	u32ISR = PDMA2->ISR;
	if(u32ISR & 0x02)	  
		PDMA2->ISR = 0x2;	
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
