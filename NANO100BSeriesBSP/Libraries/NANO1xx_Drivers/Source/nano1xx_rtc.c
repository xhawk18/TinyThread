/******************************************************************************
 * @file     nano1xx_rtc.c
 * @brief    NANO1xx RTC driver source file.
 *           Real Time Clock (RTC) unit provides user the real time and calendar
 *           message. The RTC uses a 32.768 KHz external crystal. A built in RTC
 *           is designed to generate the periodic interrupt signal. The period 
 *           can be 0.25/ 0.5/ 1/ 2/ 4/ 8 second. There is RTC overflow counter
 *           and it can be adjusted by software.
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_rtc.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_RTC_Driver RTC Driver
  @{
*/

/// @cond
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define RTC_GLOBALS

//#define RTC_DEBUG
#ifdef RTC_DEBUG
#define RTCDEBUG     printf
#else
#define RTCDEBUG(...)
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/                  
static char g_chHourMode = 0;

static volatile uint32_t g_u32Reg, g_u32Reg1,g_u32hiYear,g_u32loYear,g_u32hiMonth,g_u32loMonth,g_u32hiDay,g_u32loDay;
static volatile uint32_t g_u32hiHour,g_u32loHour,g_u32hiMin,g_u32loMin,g_u32hiSec,g_u32loSec;

/// @endcond

/** @addtogroup NANO1xx_RTC_EXPORTED_FUNCTIONS RTC Exported Functions
  @{
*/

/**
 *  @brief Set Frequency Compensation Data
 *
 *  @param[in] i32FrequencyX100      Specify the RTC clock X100, ex: 3277365 means 32773.65.
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return    
 *            E_SUCCESS: Success
 *            E_DRVRTC_ERR_FCR_VALUE: Wrong Compensation value
 *
 *  @par Example
 *  @code  
 *  //If the measured RTC crystal frequency is 32773.65Hz.
 *  RTC_SetFrequencyCompensation(3277365);
 *  @endcode
 *
 */
int32_t RTC_SetFrequencyCompensation(int32_t i32FrequencyX100)
{
    int32_t i32RegInt,i32RegFra ;

	/* Compute Interger and Fraction for RTC register*/
    i32RegInt = (i32FrequencyX100/100) - DRVRTC_FCR_REFERENCE;
    i32RegFra = (((i32FrequencyX100%100)) * 60) / 100;
    
	/* Judge Interger part is reasonable */
    if ( (i32RegInt < 0) | (i32RegInt > 15) )
    {
        return E_DRVRTC_ERR_FCR_VALUE ;
    }
	
	RTC_WriteEnable();

	RTC->FCR = (uint32_t)((i32RegInt<<8) | i32RegFra);

    return E_SUCCESS;
}

/**
 *  @brief Access Password to AER to make access other register enable
 *
 *  @param     None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return               
 *            E_SUCCESS: Success
 *            E_DRVRTC_ERR_FAILED : Failed.
 *
 *  @note After write a password to AER register, TLR / CLR /DWR / TAR /CAR register can be written.
 *
 *  @par Example
 *  @code   
 *  // Before you want to set the value in TLR / CLR /DWR / TAR /CAR register, using the function to open access account.
 *  RTC_WriteEnable (void) ;
 *  @endcode
 *
 */
int32_t RTC_WriteEnable (void)
{
    int32_t i32i = 0;

	int i32retry = 100;
    
	/*-------------------------------------------------------------------------------------------------*/
    /* After 200ms, Access enable wiil auto-clear. As soon as possible to do your setting              */
    /*-------------------------------------------------------------------------------------------------*/
	RETRY:

	i32i = 0;
	
	RTC->AER = DRVRTC_WRITE_KEY;
	
    for (i32i = 0 ; i32i < DRVRTC_WAIT_COUNT ; i32i++)
	{
        /*-------------------------------------------------------------------------------------------------*/
        /* check RTC_AER[16] to find out RTC write enable                                                  */
        /*-------------------------------------------------------------------------------------------------*/
		RTC->AER = DRVRTC_WRITE_KEY;
		
		if (RTC->AER & RTC_AER_ENF)
		    break;
	}

	if (i32i == DRVRTC_WAIT_COUNT)
    {
        RTCDEBUG ("\nRTC: DrvRTC_WriteEnable, set write enable FAILED!\n");

		i32retry--;

        if(!i32retry)
	   		return E_DRVRTC_ERR_FAILED;
						
		goto RETRY;
    }

	return E_SUCCESS;
}

/**
 *  @brief Initial RTC. It consists of enable 32K clock and RTC clock and write initial key to let RTC start count.
 *
 *  @param     None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *            E_SUCCESS: Success
 *            E_DRVRTC_ERR_EIO: Initial RTC Failed.
 *
 *  @par Example
 *  @code   
 *  //In the beginning, call the function to initial RTC
 *  RTC_Init(void) 
 *  @endcode
 *
 */
int32_t RTC_Init (void)
{
    int32_t i32i =0;
	volatile int32_t i32delay=1000;
	
    /*-----------------------------------------------------------------------------------------------------*/
    /* Initial time data struct and some parameters.                                                       */
    /*-----------------------------------------------------------------------------------------------------*/

	/* un-lock */
	/* Enable 32K Clock */
	CLK->PWRCTL |= CLK_PWRCTL_LXT_EN;
	  
	/* Waiting for 32K stable */
  	while(i32delay--);
	
	/* Enable RTC Clock */
	CLK->APBCLK |= CLK_APBCLK_RTC_EN;
	
    /*-----------------------------------------------------------------------------------------------------*/
    /* When RTC is power on, write 0xa5eb1357 to RTC_INIR to reset all logic.                              */
    /*-----------------------------------------------------------------------------------------------------*/
	
	RTC->INIR = DRVRTC_INIT_KEY;

    for (i32i = 0 ; i32i < DRVRTC_WAIT_COUNT ; i32i++)
    {
		if(RTC->INIR == 0x1)  /* Check RTC_INIR[0] to find out RTC reset signal */
        { 
            break;
        }
    }

    if (i32i == DRVRTC_WAIT_COUNT)
    {
        RTCDEBUG("\nRTC: DrvRTC_Init, initial RTC FAILED!\n");
        return E_DRVRTC_ERR_EIO;
    }

    return E_SUCCESS;
}

/**
 *  @brief Set Current time (Year/Month/Day, Hour/Minute/Sec and day of week)
 *
 *  @param[in]     sPt \n
 *                     Specify the time property and current time. It includes: \n                   
 *                     u8cClockDisplay: DRVRTC_CLOCK_12 / DRVRTC_CLOCK_24       \n
 *                     u8cAmPm: DRVRTC_AM / DRVRTC_PM                           \n
 *                     u32cSecond: Second value                                 \n
 *                     u32cMinute: Minute value                                 \n
 *                     u32cHour: Hour value                                     \n
 *                     u32cDayOfWeek: Day of week                               \n
 *                     u32cDay: Day value                                       \n
 *                     u32cMonth: Month value                                   \n
 *                     u32Year: Year value                                      \n
 *                     u8IsEnableWakeUp: Enable or not Wakeup function when time alarm happen 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *           E_SUCCESS: Success
 *           E_DRVRTC_ERR_EIO: Initial RTC Failed
 *
 *  @par Example
 *  @code   
 *  //Start RTC count from 2012.July.1, 13:20:00
 *  S_DRVRTC_TIME_DATA_T sInitTime;
 *  sInitTime.u32Year         = 2012;
 *  sInitTime.u32cMonth       = 7;	 
 *  sInitTime.u32cDay		  = 1;
 *  sInitTime.u32cHour        = 13;
 *  sInitTime.u32cMinute	  = 20;
 *  sInitTime.u32cSecond	  = 0;
 *  sInitTime.u32cDayOfWeek   = DRVRTC_MONDAY;
 *  sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;
 *  if(RTC_Open(&sInitTime) !=E_SUCCESS){
 *     printf("RTC Open Fail!!\n");}
 *  @endcode
 *
 */
int32_t RTC_Open (S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Reg;
	
	volatile int32_t i32delay=1000;
        
    /*-----------------------------------------------------------------------------------------------------*/
    /* DO BASIC JUDGEMENT TO Check RTC time data value is reasonable or not.                               */
    /*-----------------------------------------------------------------------------------------------------*/
	assert_param(	!( ((sPt->u32Year - DRVRTC_YEAR2000) > 99)|
							((sPt->u32cMonth == 0) || (sPt->u32cMonth > 12))|
							((sPt->u32cDay   == 0) || (sPt->u32cDay   > 31))));

	assert_param( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) || (sPt->u8cClockDisplay==DRVRTC_CLOCK_24));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) &&
							((sPt->u32cHour == 0) || (sPt->u32cHour > 12))));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_24) &&
							(sPt->u32cHour > 23)));

	assert_param(	!((sPt->u32cMinute > 59) |
							(sPt->u32cSecond > 59) |
							(sPt->u32cSecond > 59)));

	assert_param( !(sPt->u32cDayOfWeek > 6) );


    /*-----------------------------------------------------------------------------------------------------*/
    /* Important, call DrvRTC_WriteEnable() before write data into any register.                              */
    /* User should be write data as soon as possible.Access enable wiil clear after 200ms                  */
	/*-----------------------------------------------------------------------------------------------------*/
    g_u32Reg = RTC_WriteEnable();
    if (g_u32Reg != 0)
    {
        return E_DRVRTC_ERR_FAILED;
    }
    
    /*-----------------------------------------------------------------------------------------------------*/
    /* Second, set RTC 24/12 hour setting                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
    {
        RTC_WriteEnable();
		RTC->TSSR &= ~RTC_TSSR_24hr;

        /*-------------------------------------------------------------------------------------------------*/
        /* important, range of 12-hour PM mode is 21 upto 32                                               */
        /*-------------------------------------------------------------------------------------------------*/
        if (sPt->u8cAmPm == DRVRTC_PM)
            sPt->u32cHour += 20;
    }
    else                                                                               /* DRVRTC_CLOCK_24 */
    {
        RTC_WriteEnable();
		RTC->TSSR |= RTC_TSSR_24hr;
        RTCDEBUG ("RTC: 24-hour\n");
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set RTC Calender Loading                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    u32Reg    = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
    u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
    u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
    u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
    u32Reg    |= ((sPt->u32cDay    / 10) << 4);
    u32Reg    |= (sPt->u32cDay     % 10);
    g_u32Reg = u32Reg;
	
	RTC_WriteEnable();
	RTC->TSSR |= RTC_TSSR_24hr;
    RTC->CLR = (uint32_t)g_u32Reg;

	/*-----------------------------------------------------------------------------------------------------*/
    /* Set RTC Time Loading                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    u32Reg     = ((sPt->u32cHour / 10) << 20);
    u32Reg    |= ((sPt->u32cHour % 10) << 16);
    u32Reg    |= ((sPt->u32cMinute / 10) << 12);
    u32Reg    |= ((sPt->u32cMinute % 10) << 8);
    u32Reg    |= ((sPt->u32cSecond / 10) << 4);
    u32Reg    |= (sPt->u32cSecond % 10);
    g_u32Reg = u32Reg;

 	RTC_WriteEnable();
    RTC->TLR = (uint32_t)g_u32Reg;

	RTC->DWR = sPt->u32cDayOfWeek;

	RTC->TTR &= ~RTC_TTR_TWKE;
	RTC->TTR |= sPt->u8IsEnableWakeUp ? RTC_TTR_TWKE:0;

	/* Waiting for RTC settings stable */
  	while(i32delay--);

    return E_SUCCESS;
}

/**
 *  @brief Read current date/time from RTC setting
 *
 *  @param[in]     sPt \n
 *                     Specify the time property and current time. It includes: \n                   
 *                     u8cClockDisplay: DRVRTC_CLOCK_12 / DRVRTC_CLOCK_24       \n
 *                     u8cAmPm: DRVRTC_AM / DRVRTC_PM                           \n
 *                     u32cSecond: Second value                                 \n
 *                     u32cMinute: Minute value                                 \n
 *                     u32cHour: Hour value                                     \n
 *                     u32cDayOfWeek: Day of week                               \n
 *                     u32cDay: Day value                                       \n
 *                     u32cMonth: Month value                                   \n
 *                     u32Year: Year value                                      
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *           E_SUCCESS: Success
 *           E_DRVRTC_ERR_ENOTTY: Parameter is wrong
 *
 *  @par Example
 *  @code   
 *  //Condition: You want to get current RTC calendar and time 
 *  S_DRVRTC_TIME_DATA_T sCurTime;
 *  RTC_Read(&sCurTime);
 *  printf("Current Time:%d/%02d/%02d	%02d:%02d:%02d\n",
 *  sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
 *  @endcode
 *
 */
int32_t RTC_Read (S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Tmp;
    
	sPt->u8cClockDisplay = RTC->TSSR & RTC_TSSR_24hr;    /* 12/24-hour */
    sPt->u32cDayOfWeek   = RTC->DWR & RTC_DWR_DWR;       /* Day of week */

	g_u32hiYear  = (RTC->CLR & RTC_CLR_TEN_YEAR) >> 20;
    g_u32loYear  = (RTC->CLR & RTC_CLR_YEAR) >> 16;
    g_u32hiMonth = (RTC->CLR & RTC_CLR_TEN_MON) >> 12;
    g_u32loMonth = (RTC->CLR & RTC_CLR_MON) >> 8;
    g_u32hiDay   = (RTC->CLR & RTC_CLR_TEN_DAY) >> 4;
    g_u32loDay   = (RTC->CLR & RTC_CLR_DAY);

	g_u32hiHour =  (RTC->TLR & RTC_TLR_TEN_HR) >> 20;
    g_u32loHour =  (RTC->TLR & RTC_TLR_HR) >> 16;
    g_u32hiMin  =  (RTC->TLR & RTC_TLR_TEN_MIN) >> 12;
    g_u32loMin  =  (RTC->TLR & RTC_TLR_MIN) >> 8;
    g_u32hiSec  =  (RTC->TLR & RTC_TLR_TEN_SEC) >> 4;
    g_u32loSec  =  (RTC->TLR & RTC_TLR_SEC);	

    u32Tmp = (g_u32hiYear * 10);									/* Compute to 20XX year */
    u32Tmp+= g_u32loYear;
    sPt->u32Year   =   u32Tmp  + DRVRTC_YEAR2000;
    
    u32Tmp = (g_u32hiMonth * 10);									/* Compute 0~12 month */
    sPt->u32cMonth = u32Tmp + g_u32loMonth;
    
    u32Tmp = (g_u32hiDay * 10);										/* Compute 0~31 day */
    sPt->u32cDay   =  u32Tmp  + g_u32loDay;

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)					/* Compute12/24 hout */
    {
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour = u32Tmp;                                		/* AM: 1~12. PM: 21~32. */

        if (sPt->u32cHour >= 21)
        {
            sPt->u8cAmPm = DRVRTC_PM;
            sPt->u32cHour -= 20;
        }
        else
        {
            sPt->u8cAmPm = DRVRTC_AM;
        }
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;

    }
    else
    {   															/* DRVRTC_CLOCK_24 */
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour   = u32Tmp;
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;
    }

    return E_SUCCESS;

}

/**
 *  @brief Read alarm date/time from RTC setting
 *
 *  @param[in]     sPt \n
 *                     Specify the time property and current time. It includes: \n                   
 *                     u8cClockDisplay: DRVRTC_CLOCK_12 / DRVRTC_CLOCK_24       \n
 *                     u8cAmPm: DRVRTC_AM / DRVRTC_PM                           \n
 *                     u32cSecond: Second value                                 \n
 *                     u32cMinute: Minute value                                 \n
 *                     u32cHour: Hour value                                     \n
 *                     u32cDayOfWeek: Day of week                               \n
 *                     u32cDay: Day value                                       \n
 *                     u32cMonth: Month value                                   \n
 *                     u32Year: Year value                                      
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *           E_SUCCESS: Success
 *           E_DRVRTC_ERR_ENOTTY: Parameter is wrong
 *
 *  @par Example
 *  @code   
 *  //Condition: You want to get alarm RTC calendar and time 
 *  S_DRVRTC_TIME_DATA_T sCurTime;
 *  RTC_ReadAlarm(&sCurTime);
 *  printf("Current Time:%d/%02d/%02d	%02d:%02d:%02d\n",
 *  sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
 *  @endcode
 *
 */
int32_t RTC_ReadAlarm (S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Tmp;
    
	sPt->u8cClockDisplay = RTC->TSSR & RTC_TSSR_24hr;    /* 12/24-hour */
    sPt->u32cDayOfWeek   = RTC->DWR & RTC_DWR_DWR;       /* Day of week */

	g_u32hiYear  = (RTC->CAR & RTC_CAR_TEN_YEAR) >> 20;
	g_u32loYear  = (RTC->CAR & RTC_CAR_YEAR) >> 16;
	g_u32hiMonth = (RTC->CAR & RTC_CAR_TEN_MON) >> 12;
	g_u32loMonth = (RTC->CAR & RTC_CAR_MON) >> 8;
	g_u32hiDay	 = (RTC->CAR & RTC_CAR_TEN_DAY) >> 4;
	g_u32loDay	 = (RTC->CAR & RTC_CAR_DAY);
	
	g_u32hiHour =  (RTC->TAR & RTC_TAR_TEN_HR) >> 20;
	g_u32loHour =  (RTC->TAR & RTC_TAR_HR) >> 16;
	g_u32hiMin	=  (RTC->TAR & RTC_TAR_TEN_MIN) >> 12;
	g_u32loMin	=  (RTC->TAR & RTC_TAR_MIN) >> 8;
	g_u32hiSec	=  (RTC->TAR & RTC_TAR_TEN_SEC) >> 4;
	g_u32loSec	=  (RTC->TAR & RTC_TAR_SEC);

    u32Tmp = (g_u32hiYear * 10);									/* Compute to 20XX year */
    u32Tmp+= g_u32loYear;
    sPt->u32Year   =   u32Tmp  + DRVRTC_YEAR2000;
    
    u32Tmp = (g_u32hiMonth * 10);									/* Compute 0~12 month */
    sPt->u32cMonth = u32Tmp + g_u32loMonth;
    
    u32Tmp = (g_u32hiDay * 10);										/* Compute 0~31 day */
    sPt->u32cDay   =  u32Tmp  + g_u32loDay;

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)					/* Compute12/24 hout */
    {
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour = u32Tmp;                                		/* AM: 1~12. PM: 21~32. */

        if (sPt->u32cHour >= 21)
        {
            sPt->u8cAmPm = DRVRTC_PM;
            sPt->u32cHour -= 20;
        }
        else
        {
            sPt->u8cAmPm = DRVRTC_AM;
        }
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;

    }
    else
    {   															/* DRVRTC_CLOCK_24 */
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour   = u32Tmp;
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;
    }

    return E_SUCCESS;

}



/**
 *  @brief Update current date/time to RTC
 *
 *  @param[in]     sPt \n
 *                     Specify the time property and current time. It includes: \n                   
 *                     u8cClockDisplay: DRVRTC_CLOCK_12 / DRVRTC_CLOCK_24       \n
 *                     u8cAmPm: DRVRTC_AM / DRVRTC_PM                           \n
 *                     u32cSecond: Second value                                 \n
 *                     u32cMinute: Minute value                                 \n
 *                     u32cHour: Hour value                                     \n
 *                     u32cDayOfWeek: Day of week                               \n
 *                     u32cDay: Day value                                       \n
 *                     u32cMonth: Month value                                   \n
 *                     u32Year: Year value                                      
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *            E_SUCCESS: Success
 *            E_DRVRTC_ERR_ENOTTY: Parameter is wrong.
 *            E_DRVRTC_ERR_FAILED: Initial RTC failed
 *
 *  @par Example
 *  @code   
 *  //Condition: Update current the second of time to zero 
 *  S_DRVRTC_TIME_DATA_T sCurTime;
 *  RTC_Read(&sCurTime);
 *  sCurTime.u32cSecond = 0;
 *  RTC_Write(&sCurTime);
 *  @endcode
 *
 */
int32_t RTC_Write(S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Reg;
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check RTC time data value is reasonable or not.                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
	assert_param(	!( ((sPt->u32Year - DRVRTC_YEAR2000) > 99)|
							((sPt->u32cMonth == 0) || (sPt->u32cMonth > 12))|
							((sPt->u32cDay   == 0) || (sPt->u32cDay   > 31))));

	assert_param( !( (sPt->u32cMonth == 0) || (sPt->u32cMonth > 12) ));
    
	assert_param( !( (sPt->u32cDay == 0) || (sPt->u32cDay > 31) ));

	assert_param( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) || (sPt->u8cClockDisplay==DRVRTC_CLOCK_24));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) &&
							((sPt->u32cHour == 0) || (sPt->u32cHour > 12))));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_24) &&
							(sPt->u32cHour > 23)));

	assert_param( !(sPt->u32cMinute > 59));
	assert_param( !(sPt->u32cSecond > 59));
	assert_param( !(sPt->u32cDayOfWeek > 6));


    /*-----------------------------------------------------------------------------------------------------*/
    /* Important, call DrvRTC_Open() before write data into any register.                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    g_u32Reg = RTC_WriteEnable();
    if (g_u32Reg != 0)
    {
        return E_DRVRTC_ERR_FAILED;
    }

	/*---------------------------------------------------------------------------------------------*/
    /* Second, set RTC time data.                                                                  */
    /*---------------------------------------------------------------------------------------------*/

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
    {
    	g_chHourMode = DRVRTC_CLOCK_12;
		RTC->TSSR &= ~RTC_TSSR_24hr;
        RTCDEBUG ("RTC: 12-hour\n");
        /*-----------------------------------------------------------------------------------------*/
        /* important, range of 12-hour PM mode is 21 upto 32                                       */
        /*-----------------------------------------------------------------------------------------*/
        if (sPt->u8cAmPm == DRVRTC_PM)
        	sPt->u32cHour += 20;
    }
    else                                                                  /* DRVRTC_CLOCK_24 */
    {
    	g_chHourMode = DRVRTC_CLOCK_24;
		RTC->TSSR |= RTC_TSSR_24hr;
        RTCDEBUG ("RTC: 24-hour\n");
    }

	RTC->DWR = sPt->u32cDayOfWeek & RTC_DWR_DWR;
		    
	/*---------------------------------------------------------------------------------------------*/
    /* Second, set RTC time data.                                                                  */
    /*---------------------------------------------------------------------------------------------*/

	u32Reg     = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
	u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
	u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
	u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
	u32Reg    |= ((sPt->u32cDay    / 10) << 4);
	u32Reg    |= (sPt->u32cDay     % 10);
    g_u32Reg = u32Reg;
	RTC_WriteEnable();
          
	RTC->CLR = (uint32_t)g_u32Reg;
	RTCDEBUG ("RTC: REG_RTC_CLR[0x%08x]\n", RTC->CLR);  
            
	u32Reg     = ((sPt->u32cHour / 10) << 20);
	u32Reg    |= ((sPt->u32cHour % 10) << 16);
	u32Reg    |= ((sPt->u32cMinute / 10) << 12);
	u32Reg    |= ((sPt->u32cMinute % 10) << 8);
	u32Reg    |= ((sPt->u32cSecond / 10) << 4);
	u32Reg    |= (sPt->u32cSecond % 10);
	g_u32Reg = u32Reg;
		
	RTC_WriteEnable();
    RTC->TLR = (uint32_t)g_u32Reg;
	RTCDEBUG ("RTC: REG_RTC_TLR[0x%08x]\n", RTC->TLR);   
		
    return E_SUCCESS;	

}


/**
 *  @brief Update alarm date/time to RTC
 *
 *  @param[in]     sPt \n
 *                     Specify the time property and current time. It includes: \n                   
 *                     u8cClockDisplay: DRVRTC_CLOCK_12 / DRVRTC_CLOCK_24       \n
 *                     u8cAmPm: DRVRTC_AM / DRVRTC_PM                           \n
 *                     u32cSecond: Second value                                 \n
 *                     u32cMinute: Minute value                                 \n
 *                     u32cHour: Hour value                                     \n
 *                     u32cDayOfWeek: Day of week                               \n
 *                     u32cDay: Day value                                       \n
 *                     u32cMonth: Month value                                   \n
 *                     u32Year: Year value                                      
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                          
 *            E_SUCCESS: Success
 *            E_DRVRTC_ERR_ENOTTY: Parameter is wrong.
 *            E_DRVRTC_ERR_FAILED: Initial RTC failed
 *
 *  @par Example
 *  @code   
 *  //Condition: Update current the second of time to zero 
 *  S_DRVRTC_TIME_DATA_T sCurTime;
 *  RTC_ReadAlarm(&sCurTime);
 *  sCurTime.u32cSecond = 0;
 *  RTC_WriteAlarm(&sCurTime);
 *  @endcode
 *
 */
int32_t RTC_WriteAlarm(S_DRVRTC_TIME_DATA_T *sPt)
//int32_t RTC_Write(E_DRVRTC_TIME_SELECT eTime, S_DRVRTC_TIME_DATA_T *sPt)
{
    __IO uint32_t u32Reg;
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check RTC time data value is reasonable or not.                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
	assert_param(	!( ((sPt->u32Year - DRVRTC_YEAR2000) > 99)|
							((sPt->u32cMonth == 0) || (sPt->u32cMonth > 12))|
							((sPt->u32cDay   == 0) || (sPt->u32cDay   > 31))));

	assert_param( !( (sPt->u32cMonth == 0) || (sPt->u32cMonth > 12) ));
    
	assert_param( !( (sPt->u32cDay == 0) || (sPt->u32cDay > 31) ));

	assert_param( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) || (sPt->u8cClockDisplay==DRVRTC_CLOCK_24));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_12) &&
							((sPt->u32cHour == 0) || (sPt->u32cHour > 12))));

	assert_param(	!( (sPt->u8cClockDisplay==DRVRTC_CLOCK_24) &&
							(sPt->u32cHour > 23)));

	assert_param( !(sPt->u32cMinute > 59));
	assert_param( !(sPt->u32cSecond > 59));
	assert_param( !(sPt->u32cDayOfWeek > 6));


    /*-----------------------------------------------------------------------------------------------------*/
    /* Important, call DrvRTC_Open() before write data into any register.                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    g_u32Reg = RTC_WriteEnable();
    if (g_u32Reg != 0)
    {
        return E_DRVRTC_ERR_FAILED;
    }

	/*---------------------------------------------------------------------------------------------*/
    /* Set Calender alarm time data.                                                               */
    /*---------------------------------------------------------------------------------------------*/
	u32Reg     = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
	u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
	u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
	u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
	u32Reg    |= ((sPt->u32cDay    / 10) << 4);
	u32Reg    |= (sPt->u32cDay     % 10);
    g_u32Reg = u32Reg;
	RTC_WriteEnable();
			
	RTC->CAR = (uint32_t)g_u32Reg;
	RTCDEBUG ("RTC: REG_RTC_CAR[0x%08x]\n", RTC->CAR);  
			
						 
    if (g_chHourMode == DRVRTC_CLOCK_12)
    {
    	if (sPt->u8cAmPm == DRVRTC_PM)       /* important, range of 12-hour PM mode is 21 upto 32 */
        	sPt->u32cHour += 20;
    }

	/*---------------------------------------------------------------------------------------------*/
    /* Set Time alarm time data.                                                                   */
    /*---------------------------------------------------------------------------------------------*/
	u32Reg     = ((sPt->u32cHour / 10) << 20);
	u32Reg    |= ((sPt->u32cHour % 10) << 16);
	u32Reg    |= ((sPt->u32cMinute / 10) << 12);
	u32Reg    |= ((sPt->u32cMinute % 10) << 8);
	u32Reg    |= ((sPt->u32cSecond / 10) << 4);
	u32Reg    |= (sPt->u32cSecond % 10);

	g_u32Reg = u32Reg;
    RTC_WriteEnable();
    RTC->TAR = (uint32_t)g_u32Reg;
	RTCDEBUG ("RTC: REG_RTC_TAR[0x%08x]\n", RTC->TAR);   

	/*---------------------------------------------------------------------------------------------*/
    /* Finally, enable alarm interrupt.                                                            */
    /*---------------------------------------------------------------------------------------------*/
	RTC_EnableInt(RTC_RIER_AIER);
            
	RTC->TTR &= ~RTC_TTR_TWKE;
	RTC->TTR |= sPt->u8IsEnableWakeUp ? RTC_TTR_TWKE:0;
				
	return E_SUCCESS;

}



/**
 *  @brief Read/Write the spare registers
 *
 *  @param[in]     sparenum   Specify the number of register (0~19)
 *  @param[in]     value      The value we want to write
 *  @param[in]     wrsel      Specify the writing/reading access  [1:Write; 0:Read]                                     
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return                               
 *           E_SUCCESS: Success to write  \n
 *           Or the content value of specified spare register in reading
 *
 *  @par Example
 *  @code   
 *  RTC_SpareRegsAccess(10, 0x5555, 1);
 *  @endcode
 *
 */
int32_t RTC_SpareRegsAccess (int32_t sparenum, int32_t value, int32_t wrsel)
{
	uint32_t ret = E_SUCCESS;

	RTC_WriteEnable();

	if(wrsel==0)
		while((RTC->SPRCTL & RTC_SPRCTL_SPRRDY) == 0);
	
	if(sparenum==0)
		(wrsel==1) ? (RTC->SPR0=value) : (ret=RTC->SPR0);
	if(sparenum==1)
		(wrsel==1) ? (RTC->SPR1=value) : (ret=RTC->SPR1);
	if(sparenum==2)
		(wrsel==1) ? (RTC->SPR2=value) : (ret=RTC->SPR2);
	if(sparenum==3)
		(wrsel==1) ? (RTC->SPR3=value) : (ret=RTC->SPR3);
	if(sparenum==4)
		(wrsel==1) ? (RTC->SPR4=value) : (ret=RTC->SPR4);
	if(sparenum==5)
		(wrsel==1) ? (RTC->SPR5=value) : (ret=RTC->SPR5);
	if(sparenum==6)
		(wrsel==1) ? (RTC->SPR6=value) : (ret=RTC->SPR6);
	if(sparenum==7)
		(wrsel==1) ? (RTC->SPR7=value) : (ret=RTC->SPR7);
	if(sparenum==8)
		(wrsel==1) ? (RTC->SPR8=value) : (ret=RTC->SPR8);
	if(sparenum==9)
		(wrsel==1) ? (RTC->SPR9=value) : (ret=RTC->SPR9);
	if(sparenum==10)
		(wrsel==1) ? (RTC->SPR10=value) : (ret=RTC->SPR10);
	if(sparenum==11)
		(wrsel==1) ? (RTC->SPR11=value) : (ret=RTC->SPR11);
	if(sparenum==12)
		(wrsel==1) ? (RTC->SPR12=value) : (ret=RTC->SPR12);
	if(sparenum==13)
		(wrsel==1) ? (RTC->SPR13=value) : (ret=RTC->SPR13);
	if(sparenum==14)
		(wrsel==1) ? (RTC->SPR14=value) : (ret=RTC->SPR14);
	if(sparenum==15)
		(wrsel==1) ? (RTC->SPR15=value) : (ret=RTC->SPR15);
	if(sparenum==16)
		(wrsel==1) ? (RTC->SPR16=value) : (ret=RTC->SPR16);
	if(sparenum==17)
		(wrsel==1) ? (RTC->SPR17=value) : (ret=RTC->SPR17);
	if(sparenum==18)
		(wrsel==1) ? (RTC->SPR18=value) : (ret=RTC->SPR18);
	if(sparenum==19)
		(wrsel==1) ? (RTC->SPR19=value) : (ret=RTC->SPR19);

	if(wrsel==1)
		while((RTC->SPRCTL & RTC_SPRCTL_SPRRDY) == 0);
	
	return ret;

}

/**
 *  @brief Enable Spare function and set the detection edge
 *
 *  @param[in]     eSpareEdge    Detection edge (DRVRTC_SNOOPER_RISING, RVRTC_SNOOPER_FALLING)                                    
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  None                             
 *
 *  @par Example
 *  @code   
 *  RTC_EnableSpareFunc(DRVRTC_SNOOPER_RISING);
 *  @endcode
 *
 */
void RTC_EnableSpareFunc(uint32_t eSpareEdge)
{
	RTC_WriteEnable();

	/* detection edge select */
	if(eSpareEdge)
	{
		RTC->SPRCTL |= RTC_SPRCTL_SNOOPEDGE;
	}
	else
	{
		RTC->SPRCTL &= ~RTC_SPRCTL_SNOOPEDGE;
	}

	while((RTC->SPRCTL & RTC_SPRCTL_SPRRDY) == 0);
	
	/* enable snooper pin event detection */
	RTC->SPRCTL |= RTC_SPRCTL_SNOOPEN;
	while((RTC->SPRCTL & RTC_SPRCTL_SPRRDY) == 0);
	
}

/**
 *  @brief Disable Spare function
 *
 *  @param   None                                    
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  None                             
 *
 *  @par Example
 *  @code   
 *  RTC_DisableSpareFunc();
 *  @endcode
 *
 */
void RTC_DisableSpareFunc(void)
{
	RTC_WriteEnable();

	/* Disable snooper pin event detection */
	RTC->SPRCTL &= ~RTC_SPRCTL_SNOOPEN;
	while((RTC->SPRCTL & RTC_SPRCTL_SPRRDY) == 0);
}



/**
 *  @brief The function is used to set time tick period for periodic time tick Interrupt.
 *
 *  @param[in]   ucMode 
 *                       The structure of DRVRTC_TICK.
 *                       It is ued to set the RTC time tick period for Periodic Time Tick Interrupt request. 
 *                       It consists of: \n 
 *                       DRVRTC_TICK_1_SEC: Time tick is 1 second        \n
 *                       DRVRTC_TICK_1_2_SEC: Time tick is 1/2 second    \n
 *                       DRVRTC_TICK_1_4_SEC: Time tick is 1/4 second    \n
 *                       DRVRTC_TICK_1_8_SEC: Time tick is 1/8 second    \n
 *                       DRVRTC_TICK_1_16_SEC: Time tick is 1/16 second  \n
 *                       DRVRTC_TICK_1_32_SEC: Time tick is 1/32 second  \n
 *                       DRVRTC_TICK_1_64_SEC: Time tick is 1/64 second  \n
 *                       DRVRTC_TICK_1_128_SEC: Time tick is 1/128 econd 
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  
 *           E_SUCCESS: Success
 *           E_DRVRTC_ERR_EIO: Initial RTC Failed.
 *           E_DRVRTC_ERR_ENOTTY: Parameter is wrong
 *
 *  @par Example
 *  @code   
 *  //Set Tick interrupt is 128 tick/sec
 *  RTC_SetTickMode (DRVRTC_TICK_1_128_SEC) ;
 *  @endcode
 *
 */
int32_t RTC_SetTickMode(uint8_t ucMode)
{

    if (RTC_WriteEnable() != 0)   				/* Write PASSWORD to access enable*/
    {
    	return E_DRVRTC_ERR_EIO ;
    }
    
	if (ucMode > DRVRTC_TICK_1_128_SEC)             /* Tick mode 0 to 7 */
    {
    	return E_DRVRTC_ERR_ENOTTY ;
    }
            
	RTC->TTR = RTC->TTR & ~RTC_TTR_TTR | ucMode;

	return E_SUCCESS;

}

/**
 *  @brief The function is used to enable specified interrupt.
 *
 *  @param[in]   str_IntSrc   The structure of interrupt source. It consists of: \n  
 *                            RTC_RIER_AIER: Alarm interrupt                  \n
 *                            RTC_RIER_TIER: Tick interrupt                    \n
 *                            RTC_RIER_SNOOPIER: Snooper interrupt              \n
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  
 *           E_SUCCESS: Success
 *
 *  @par Example
 *  @code   
 *  //Enable tick interrupt and install callback function "RTC_TickCallBackfn".
 *  RTC_EnableInt(DRVRTC_TICK_INT,RTC_TickCallBackfn)
 *  @endcode
 *
 */
int32_t RTC_EnableInt(uint32_t str_IntSrc)
{
	RTC->RIER |= str_IntSrc; 
	
	NVIC_EnableIRQ(RTC_IRQn); 

	return E_SUCCESS;

}

/**
 *  @brief The function is used to disable specified interrupt.
 *
 *  @param[in]   i32IntSrc   The structure of interrupt source. It consists of: \n  
 *                            RTC_RIER_AIER: Alarm interrupt                  \n
 *                            RTC_RIER_TIER: Tick interrupt                    \n
 *                            RTC_RIER_SNOOPIER: Snooper interrupt              \n
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  
 *           E_SUCCESS: Successg
 *
 *  @par Example
 *  @code   
 *  //Disable tick, snooper and alarm interrupt
 *  RTC_DisableInt(DRVRTC_ALL_INT);
 *  @endcode
 *
 */
int32_t RTC_DisableInt(uint32_t i32IntSrc)
{
	
	if(i32IntSrc & RTC_RIER_TIER)
	{	
		RTC->RIER &= ~RTC_RIER_TIER; 
		RTC->RIIR |= RTC_RIIR_TIS; 
	}
	
	if(i32IntSrc & RTC_RIER_AIER)
	{
		RTC->RIER &= ~RTC_RIER_AIER; 
		RTC->RIIR |= RTC_RIIR_AIS;
	}
	
	if(i32IntSrc & RTC_RIER_SNOOPIER)
	{
		RTC->RIER &= ~RTC_RIER_SNOOPIER; 
		RTC->RIIR |= RTC_RIIR_SNOOPIS; 
	}

	return E_SUCCESS;	

}

/**
 *  @brief Disable NVIC channel of RTC and both tick and alarm interrupt.
 *
 *  @param   None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  E_SUCCESS: Success
 *
 *  @par Example
 *  @code   
 *  RTC_Close();
 *  @endcode
 *
 */
int32_t RTC_Close (void)
{
  	NVIC_DisableIRQ(RTC_IRQn);
  
  	RTC_DisableInt(RTC_RIER_TIER | RTC_RIER_AIER | RTC_RIER_SNOOPIER);
	
    return E_SUCCESS;
}


/*@}*/ /* end of group NANO1xx_RTC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_RTC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



