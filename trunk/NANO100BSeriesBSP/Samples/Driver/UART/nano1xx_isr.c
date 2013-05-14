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
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_uart.h"

extern volatile int32_t g_bWait;

extern void UART_INT_HANDLE(uint32_t u32IntStatus);
extern void UART_ABAUD_INT_HANDLE(void);
extern void LIN_HANDLE(void) ;
extern void RS485_INT_HANDLE(void);


/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Handler                                                                                 	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    uint32_t u32uart0IntStatus;

    u32uart0IntStatus = UART0->ISR;

	UART_INT_HANDLE(u32uart0IntStatus);

}


void UART1_IRQHandler(void)
{

	if((UART1->FUN_SEL & 0x3) == FUN_LIN)
	{
		LIN_HANDLE();
	}
	else if((UART1->FUN_SEL & 0x3) == FUN_RS485)
	{
		RS485_INT_HANDLE();
	}
	else
	{
		if(UART1->ISR & DRVUART_ABAUDINT)
		{
			UART_ABAUD_INT_HANDLE();
		}
	}
	

}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

