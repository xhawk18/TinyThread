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
#include "nano1xx_rtc.h"

extern void RTC_TickHandle(void);
extern void RTC_AlarmHandle(void);
extern void RTC_SnooperHandle(void);


/**
  * @brief  RTC ISR to handle interrupt event
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{ 
   
	if ( (RTC->RIER & RTC_RIER_TIER) && (RTC->RIIR & RTC_RIIR_TIS) )		/* tick interrupt occurred */
	{
 		  RTC->RIIR = 0x2;

          RTC_TickHandle();
    }

 	if ( (RTC->RIER & RTC_RIER_AIER) && (RTC->RIIR & RTC_RIIR_AIS) )		/* alarm interrupt occurred */
    {          
		  RTC->RIIR = 0x1;
		  
		  RTC_AlarmHandle();
    }

	if ( (RTC->RIER & RTC_RIER_SNOOPIER) && (RTC->RIIR & RTC_RIIR_SNOOPIS) )	/* snooper interrupt occurred */
    {          
		  RTC->RIIR = 0x4;
		  
		  RTC_SnooperHandle();
    }
}




/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

