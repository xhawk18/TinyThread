/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "nano1xx.h"
#include "nano1xx_tk.h"

extern uint8_t volatile complete;

void TK_IRQHandler(void)
{

	complete = 1;

	// Clear interrupt status.
	TK_CLEAR_INTERRUPT;

	return;
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
