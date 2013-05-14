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
#include "EBI_LCM_OTM3225A.h"

void TMR0_IRQHandler(void) // Timer0 interrupt subroutine 
{
	static uint16_t TimerCounter=0;
	char string[12];

	TimerCounter+=1;
	sprintf(string,"%d",TimerCounter);
	DrawPartialScreen(193, 33, 97, 32, 0x0000);
	Show_String32x32(193, 33, (uint8_t *)string, 0xf800);
 	TIMER0->ISR = TIMER0->ISR; 	   
}

void RTC_IRQHandler(void)
{
	uint32_t clock;
	char string[15];
	/* tick */
	if(inpw(&RTC->RIIR)&0x2)
 	{
		clock=inpw(&RTC->TLR)&0xFFFFFF;
		sprintf(string,"%02x",(clock>>16)&0xFF);
		sprintf(string+3,"%02x",((clock)>>8)&0xFF);
		sprintf(string+6,"%02x",clock&0xFF);
		string[2]=':';
		string[5]=':';
		DrawPartialScreen(1, 65, 257, 32, 0x0000);
		Show_String32x32(1, 65, (uint8_t *)string, 0xf800);
		outpw(&RTC->RIIR,2);
	}
	/* alarm */
	if(inpw(&RTC->RIIR)&0x1) 
	{
		Show_String32x32(1, 97, "Alarm!!!", 0xf800);
		outpw(&RTC->RIIR,1);
	}
}

void WDT_IRQHandler(void) 
{ 
	UNLOCKREG(); 
	WDT->ISR = WDT_ISR_WDTIS;
	WDT->CTL = WDT_CTL_WTR;
	LOCKREG(); 
	Show_String32x32(1, 129, "WDT int", 0xf800);
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
