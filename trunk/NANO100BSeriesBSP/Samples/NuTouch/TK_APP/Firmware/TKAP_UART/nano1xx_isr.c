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

extern uint16_t data[16];
extern uint16_t f, t;
extern uint8_t volatile complete;
extern uint8_t set;


void TK_IRQHandler(void)
{

	uint8_t fail, th;


	TK_ReadStatus(NULL, &fail, &th);
	if(fail)
		f |= fail << (8 * set);
	if(th)
		t |= th << (8 * set);

	// read result from register directly
	if(set == 0) {
		memcpy(&data[0], (void *)&TK->DATA1, 16);
	} else {
		memcpy(&data[8], (void *)&TK->DATA1, 16);
	}

	complete = 1;	

	// Clear interrupt status.
	TK_CLEAR_INTERRUPT;

	return;
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
