/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVRTC_H__
#define __DRVRTC_H__

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_MAJOR_NUM    1
#define DRVRTC_MINOR_NUM    04
#define DRVRTC_BUILD_NUM    005

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_VERSION_NUM    _SYSINFRA_VERSION(DRVRTC_MAJOR_NUM, DRVRTC_MINOR_NUM,DRVRTC_BUILD_NUM)  

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code	                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVRTC_ERR_CALENDAR_VALUE		Wrong Calendar Value                  								   
// E_DRVRTC_ERR_TIMESACLE_VALUE		Wrong Time Scale Value                       						   
// E_DRVRTC_ERR_TIME_VALUE			Wrong Time Value                   									   
// E_DRVRTC_ERR_DWR_VALUE			Wrong Day Value                   									   
// E_DRVRTC_ERR_FCR_VALUE			Wrong Compenation value                   							   
// E_DRVRTC_ERR_EIO 				Initial RTC Failed.                									   
// E_DRVRTC_ERR_ENOTTY        		Command not support, or parameter incorrect.             	  		   
// E_DRVRTC_ERR_ENODEV   	    	Interface number incorrect.                     					   
// E_DRVRTC_ERR_FAILED				Failed.																  
#define E_DRVRTC_ERR_CALENDAR_VALUE		_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 0)
#define E_DRVRTC_ERR_TIMESACLE_VALUE	_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 1)
#define E_DRVRTC_ERR_TIME_VALUE			_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 2)
#define E_DRVRTC_ERR_DWR_VALUE			_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 3)
#define E_DRVRTC_ERR_FCR_VALUE			_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 4)
#define E_DRVRTC_ERR_EIO				_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 5)
#define E_DRVRTC_ERR_ENOTTY				_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 6)
#define E_DRVRTC_ERR_ENODEV				_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 7)
#define E_DRVRTC_ERR_FAILED				_SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVRTC, 8)

/*---------------------------------------------------------------------------------------------------------*/
/*  RTC Access Key 		                                                          						   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_INIT_KEY			0xa5eb1357
#define DRVRTC_WRITE_KEY		0xA965

/*---------------------------------------------------------------------------------------------------------*/
/*  RTC Initial Time Out Value				                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_WAIT_COUNT		0xFFFFFFFF

/*---------------------------------------------------------------------------------------------------------*/
/*  RTC Reference 								                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_YEAR2000			2000
#define DRVRTC_FCR_REFERENCE	32761

/*---------------------------------------------------------------------------------------------------------*/
/*  12-Hour / 24-Hour				                                                       				   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_CLOCK_12			0 
#define DRVRTC_CLOCK_24			1

/*---------------------------------------------------------------------------------------------------------*/
/*  AM / PM				                                                         						   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVRTC_AM				1
#define DRVRTC_PM				2

/*---------------------------------------------------------------------------------------------------------*/
/* INTERRUPT SOURCE                                                                		                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
	DRVRTC_ALARM_INT    = 0x01,
	DRVRTC_TICK_INT     = 0x02,
	DRVRTC_ALL_INT      = 0x03
}E_DRVRTC_INT_SOURCE;

/*---------------------------------------------------------------------------------------------------------*/
/* Define for RTC Tick mode                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
 	DRVRTC_TICK_1_SEC       =   0,          /* 1     sec                            */
 	DRVRTC_TICK_1_2_SEC     =   1,          /* 1/2   sec                            */
 	DRVRTC_TICK_1_4_SEC     =   2,          /* 1/4   sec                            */ 
 	DRVRTC_TICK_1_8_SEC     =   3,          /* 1/8   sec                            */
 	DRVRTC_TICK_1_16_SEC    =   4,          /* 1/16  sec                            */
 	DRVRTC_TICK_1_32_SEC    =   5,          /* 1/32  sec                            */ 
 	DRVRTC_TICK_1_64_SEC    =   6,          /* 1/64  sec                            */
 	DRVRTC_TICK_1_128_SEC   =   7           /* 1/128 sec                            */
}E_DRVRTC_TICK;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Time data struct & some parameters                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (PFN_DRVRTC_CALLBACK)(void);

typedef enum
{
	DRVRTC_CURRENT_TIME     = 0,
	DRVRTC_ALARM_TIME       = 1 
}E_DRVRTC_TIME_SELECT;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Day of week parameter                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    DRVRTC_SUNDAY           = 0,
    DRVRTC_MONDAY           = 1,
    DRVRTC_TUESDAY          = 2,
    DRVRTC_WEDNESDAY        = 3,
    DRVRTC_THURSDAY         = 4,
    DRVRTC_FRIDAY           = 5,
    DRVRTC_SATURDAY         = 6
}E_DRVRTC_DWR_PARAMETER;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Time Data Struct                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint8_t u8cClockDisplay;            /* 12-Hour, 24-Hour */
    uint8_t u8cAmPm;                    /* Only 12-hr used */
    uint32_t u32cSecond;
    uint32_t u32cMinute;
    uint32_t u32cHour;
    uint32_t u32cDayOfWeek;
    uint32_t u32cDay;
    uint32_t u32cMonth;
    uint32_t u32Year;
	uint8_t  u8IsEnableWakeUp;          /* Wakeup Enable */
    
}S_DRVRTC_TIME_DATA_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototype                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrcRTC_IsLeapYear(void);
int32_t DrvRTC_GetIntTick(void);
void DrvRTC_ResetIntTick(void);
int32_t DrvRTC_Init(void);   
int32_t DrvRTC_Open(S_DRVRTC_TIME_DATA_T *sPt);
int32_t DrvRTC_Read(E_DRVRTC_TIME_SELECT eTime, S_DRVRTC_TIME_DATA_T *sPt);
int32_t DrvRTC_Write(E_DRVRTC_TIME_SELECT eTime, S_DRVRTC_TIME_DATA_T *sPt);
int32_t DrvRTC_SetFrequencyCompensation(int32_t i32FrequencyX100);
int32_t DrvRTC_WriteEnable (void);
int32_t DrvRTC_Close(void);
int32_t DrvRTC_GetVersion (void);
int32_t DrvRTC_DisableInt(E_DRVRTC_INT_SOURCE i32IntSrc);
int32_t DrvRTC_EnableInt(E_DRVRTC_INT_SOURCE str_IntSrc, PFN_DRVRTC_CALLBACK pfncallback);
int32_t DrvRTC_SetTickMode(uint8_t ucMode);

#endif /* __DRVRTC_H__ */



