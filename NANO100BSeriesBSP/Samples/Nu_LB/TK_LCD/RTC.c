/******************************************************************************
 * @file     RTC.c
 * @brief    Nano1xx TK_LCD Driver Sample code
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_rtc.h"
#include "nano1xx_lcd.h"

void showTime(uint32_t hour, uint32_t minute);

void planNextRTCInterrupt(S_DRVRTC_TIME_DATA_T *sCurTime)
{
	// plan next interrupt timing
	if(sCurTime->u32cMinute < 59)
		sCurTime->u32cMinute += 1;
	else
	{
		if(sCurTime->u32cHour < 23)
			sCurTime->u32cHour += 1;
		else	// next day 
		{
			sCurTime->u32cHour = 0;

			// new year first day
			if(sCurTime->u32cMonth==12 && sCurTime->u32cDay==31) 
			{
				sCurTime->u32Year += 1;
				sCurTime->u32cMonth = 1;
				sCurTime->u32cDay = 1;
			}
			else if(sCurTime->u32cMonth==1 ||
					sCurTime->u32cMonth==3 ||
					sCurTime->u32cMonth==5 ||
					sCurTime->u32cMonth==7 ||
					sCurTime->u32cMonth==8 ||
					sCurTime->u32cMonth==10 ||
					sCurTime->u32cMonth==12)	// 1,3,5,7,8,10,12 31-day month
			{
				if(sCurTime->u32cDay < 31)
					sCurTime->u32cDay += 1;
				else
				{
					sCurTime->u32cDay = 1;
					sCurTime->u32cMonth += 1;
				}
			}
			else if(sCurTime->u32cMonth==2)	// 2, 28 or 29-day month
			{
				if(RTC_IsLeapYear())	// leap year
				{
					if(sCurTime->u32cDay < 29)
						sCurTime->u32cDay += 1;
					else
					{
						sCurTime->u32cDay = 1;
						sCurTime->u32cMonth += 1;
					}
				}
				else
				{
					if(sCurTime->u32cDay < 28)
						sCurTime->u32cDay += 1;
					else
					{
						sCurTime->u32cDay = 1;
						sCurTime->u32cMonth += 1;
					}
				}
			}
			else if(sCurTime->u32cMonth==4 ||
					sCurTime->u32cMonth==6 ||
					sCurTime->u32cMonth==9 ||
					sCurTime->u32cMonth==11)	// 4,6,9,11 30-day
			{
				if(sCurTime->u32cDay < 30)
					sCurTime->u32cDay += 1;
				else
				{
					sCurTime->u32cDay = 1;
					sCurTime->u32cMonth += 1;
				}
			}			
		}
		
		sCurTime->u32cMinute = 0;
	}
	sCurTime->u32cSecond = 0;
	//printf("Next Interrupt Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime->u32Year,sCurTime->u32cMonth,sCurTime->u32cDay,sCurTime->u32cHour,sCurTime->u32cMinute,sCurTime->u32cSecond);
	RTC_WriteAlarm(sCurTime);
	RTC_EnableInt(RTC_RIER_AIER);
	
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
