/******************************************************************************
 * @file     nano1xx_rtc.h
 * @brief    NANO1xx RTC driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_RTC_H
#define __NANO1XX_RTC_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "nano1xx.h"

/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_RTC_Driver RTC Driver
  @{
*/


/** @addtogroup NANO1xx_RTC_EXPORTED_CONSTANTS RTC Exported Constants
  @{
*/

/// @cond
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_MAJOR_NUM 1
#define DRVRTC_MINOR_NUM 00
#define DRVRTC_BUILD_NUM 000

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_VERSION_NUM    (((DRVRTC_MAJOR_NUM) << 16) | ((DRVRTC_MINOR_NUM) << 8) | (DRVRTC_BUILD_NUM))

///@endcond


#define DRVRTC_INIT_KEY			0xA5EB1357	   /*!< RTC Access Key	 */
#define DRVRTC_WRITE_KEY		0xA965		   /*!< RTC Access Key	 */

#define DRVRTC_WAIT_COUNT		0xFFFFFFFF	   /*!< Initial Time Out Value	 */

#define DRVRTC_YEAR2000			2000		   /*!< RTC Reference */
#define DRVRTC_FCR_REFERENCE	32761		   /*!< RTC Reference */

#define DRVRTC_CLOCK_12			0 			   /*!< RTC 12 Hour */
#define DRVRTC_CLOCK_24			1			   /*!< RTC 24 Hour */

#define DRVRTC_AM				1			   /*!< RTC AM */
#define DRVRTC_PM				2			   /*!< RTC PM */

#define DRVRTC_TICK_1_SEC 		((uint32_t) 0x00000000)		/*!< Time tick is 1 second */
#define DRVRTC_TICK_1_2_SEC 	((uint32_t) 0x00000001)		/*!< Time tick is 1/2 second */
#define DRVRTC_TICK_1_4_SEC 	((uint32_t) 0x00000002)
#define DRVRTC_TICK_1_8_SEC 	((uint32_t) 0x00000003)
#define DRVRTC_TICK_1_16_SEC 	((uint32_t) 0x00000004)
#define DRVRTC_TICK_1_32_SEC 	((uint32_t) 0x00000005)
#define DRVRTC_TICK_1_64_SEC 	((uint32_t) 0x00000006)
#define DRVRTC_TICK_1_128_SEC 	((uint32_t) 0x00000007)

#define DRVRTC_SUNDAY		((uint32_t) 0x00000000)
#define DRVRTC_MONDAY		((uint32_t) 0x00000001)
#define DRVRTC_TUESDAY		((uint32_t) 0x00000002)
#define DRVRTC_WEDNESDAY	((uint32_t) 0x00000003)
#define DRVRTC_THURSDAY		((uint32_t) 0x00000004)
#define DRVRTC_FRIDAY		((uint32_t) 0x00000005)
#define DRVRTC_SATURDAY		((uint32_t) 0x00000006)


#define DRVRTC_SNOOPER_RISING	0							/*!< Snooper Active Rising Edge */
#define DRVRTC_SNOOPER_FALLING	1							/*!< Snooper Active Falling Edge */

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code	                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/																  
#define E_DRVRTC_ERR_CALENDAR_VALUE		1
#define E_DRVRTC_ERR_TIMESACLE_VALUE	2
#define E_DRVRTC_ERR_TIME_VALUE			3
#define E_DRVRTC_ERR_DWR_VALUE			4
#define E_DRVRTC_ERR_FCR_VALUE			5
#define E_DRVRTC_ERR_EIO				6
#define E_DRVRTC_ERR_ENOTTY				7
#define E_DRVRTC_ERR_ENODEV				8
#define E_DRVRTC_ERR_FAILED				9

/*@}*/ /* end of group NANO1xx_RTC_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_RTC_EXPORTED_STRUCTS RTC Exported Structs
  @{
*/
/** 
  * @brief  RTC define Time Data Struct
  */
typedef struct
{
    uint8_t u8cClockDisplay;       			/*!<  12-Hour, 24-Hour */
    uint8_t u8cAmPm;               				/*!<  Only 12-hr used */
	//uint32_t u8cClockDisplay;       			/*!<  12-Hour, 24-Hour */
    //uint32_t u8cAmPm;               				/*!<  Only 12-hr used */
    uint32_t u32cSecond;
    uint32_t u32cMinute;
    uint32_t u32cHour;
    uint32_t u32cDayOfWeek;
    uint32_t u32cDay;
    uint32_t u32cMonth;
    uint32_t u32Year;
	uint8_t  u8IsEnableWakeUp;      			/*!<  Wakeup Enable */
    //uint32_t  u8IsEnableWakeUp;      			/*!<  Wakeup Enable */
} S_DRVRTC_TIME_DATA_T;

/*@}*/ /* end of group NANO1xx_RTC_EXPORTED_STRUCTS */


  


/** @addtogroup NANO1xx_RTC_EXPORTED_FUNCTIONS RTC Exported Functions
  @{
*/
int32_t RTC_Init(void);   
int32_t RTC_Open(S_DRVRTC_TIME_DATA_T *sPt);
int32_t RTC_Read(S_DRVRTC_TIME_DATA_T *sPt);
int32_t RTC_Write(S_DRVRTC_TIME_DATA_T *sPt);
int32_t RTC_WriteAlarm(S_DRVRTC_TIME_DATA_T *sPt);
int32_t RTC_ReadAlarm (S_DRVRTC_TIME_DATA_T *sPt);
int32_t RTC_SetFrequencyCompensation(int32_t i32FrequencyX100);
int32_t RTC_WriteEnable (void);
int32_t RTC_Close(void);
int32_t RTC_DisableInt(uint32_t i32IntSrc);
int32_t RTC_EnableInt(uint32_t str_IntSrc);
int32_t RTC_SetTickMode(uint8_t ucMode);
void RTC_EnableSpareFunc(uint32_t eSpareEdge);
void RTC_DisableSpareFunc(void);
int32_t RTC_SpareRegsAccess (int32_t sparenum, int32_t value, int32_t wrsel);

/**
 *  @brief According to current time , return this year is leap year or not.
 *
 *  @param   None                                    
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  
 *           1: This year is a leap year.
 *           0: This year is not a leap year.
 *
 *  @par Example
 *  @code   
 *  If(RTC_IsLeapYear(void))
 *      printf("This is Leap year!");
 *  else
 *      printf("This is not Leap year!");
 *  @endcode
 *
 */
__INLINE int32_t RTC_IsLeapYear(void)
{
	return (RTC->LIR & RTC_LIR_LIR)?1:0;
}


/**
 *  @brief   Disable RTC clock.
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
 *  RTC_DeInit();
 *  @endcode
 *
 */
 static __INLINE void RTC_DeInit(void)
 {
	/* Disable RTC Clock */
	CLK->APBCLK &= ~CLK_APBCLK_RTC_EN;
 }

/**
 *  @brief   Return the current version number of driver.
 *
 *  @param   None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_rtc.h
 *
 *  @return  DRVRTC_VERSION_NUM    Version number: \n
 *                                [31:24]: 00000000   \n
 *                                [23:16]: MAJOR_NUM  \n
 *                                [15: 8]: MINOR_NUM  \n
 *                                [ 7: 0]: BUILD_NUM 
 *
 *  @par Example
 *  @code   
 *  int32_t RTC_Version
 *  RTC_Version = RTC_GetVersion();
 *  @endcode
 *
 */
static __INLINE int32_t RTC_GetVersion (void)
{
	return DRVRTC_VERSION_NUM;
}


/*@}*/ /* end of group NANO1xx_RTC_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_RTC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


#ifdef  __cplusplus
}
#endif

#endif /* __NANO1XX_RTC_H */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/




