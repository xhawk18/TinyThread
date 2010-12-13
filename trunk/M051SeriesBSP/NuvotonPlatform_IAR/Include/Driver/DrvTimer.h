/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVTIMER_H__
#define __DRVTIMER_H__

#include "M051series.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVTIMER_MAJOR_NUM      1
#define DRVTIMER_MINOR_NUM      00
#define DRVTIMER_BUILD_NUM      001
#define DRVTIMER_VERSION_NUM    _SYSINFRA_VERSION(DRVTIMER_MAJOR_NUM, DRVTIMER_MINOR_NUM, DRVTIMER_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code	                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_DRVTIMER_CHANNEL          _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVTIMER, 1)  /* Invalid channel     */
#define E_DRVTIMER_CLOCK_RATE       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVTIMER, 2)  /* Invalid clock rate  */
#define E_DRVTIMER_EIO              _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVTIMER, 3)  /* Timer IO error      */
#define E_DRVTIMER_EVENT_FULL       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVTIMER, 4)  /* Event Full error    */
#define E_DRVWDT_CMD                _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVTIMER, 5)  /* Invalid CMD         */

/*---------------------------------------------------------------------------------------------------------*/
/* Global interface variables declarations                                                                 */                                                                            
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*TIMER_CALLBACK)(uint32_t data);	/* function pointer */
typedef void (*WDT_CALLBACK)(uint32_t data);	/* function pointer */

/*---------------------------------------------------------------------------------------------------------*/
/* Define TIME EVENT STRUCT                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct timeEvent_t
{
    int32_t              active;
    int32_t              initTick;
    int32_t              curTick;
    TIMER_CALLBACK       funPtr;
    uint32_t             transParam;
} TIMER_EVENT_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Define TIMER Channel                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum {
    E_TMR0  =   0 ,
    E_TMR1  =   1 ,
	E_TMR2  =   2 ,
	E_TMR3  =   3        
} E_TIMER_CHANNEL;

/*---------------------------------------------------------------------------------------------------------*/
/* Define TIMER OPREATION MODE                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum{                   
    E_ONESHOT_MODE      = 0,
    E_PERIODIC_MODE   	= 1,
    E_TOGGLE_MODE     	= 2, 
} E_TIMER_OPMODE ;

/*---------------------------------------------------------------------------------------------------------*/
/* Define WDT Ioctl Command                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum {
    E_WDT_IOC_START_TIMER       = 0,
    E_WDT_IOC_STOP_TIMER        = 1,
    E_WDT_IOC_ENABLE_INT        = 2,
    E_WDT_IOC_DISABLE_INT       = 3,
    E_WDT_IOC_ENABLE_WAKEUP     = 4, 
    E_WDT_IOC_DISABLE_WAKEUP    = 5, 
    E_WDT_IOC_RESET_TIMER       = 6,
    E_WDT_IOC_ENABLE_RESET_FUNC = 7,
    E_WDT_IOC_DISABLE_RESET_FUNC= 8,
	E_WDT_IOC_SET_INTERVAL      = 9
} E_WDT_CMD; 

/*---------------------------------------------------------------------------------------------------------*/
/* Define WDT INTERVAL                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum{
    E_WDT_LEVEL0      = 0,         /* 2^4 WDT_CLK	                                                           */
    E_WDT_LEVEL1      = 1,         /* 2^6 WDT_CLK                                                            */
    E_WDT_LEVEL2      = 2,         /* 2^8 WDT_CLK                                                            */ 
    E_WDT_LEVEL3      = 3,         /* 2^10 WDT_CLK                                                           */
	E_WDT_LEVEL4      = 4,         /* 2^12 WDT_CLK                                                           */
	E_WDT_LEVEL5      = 5,         /* 2^14 WDT_CLK                                                           */
	E_WDT_LEVEL6      = 6,         /* 2^16 WDT_CLK                                                           */
	E_WDT_LEVEL7      = 7          /* 2^18 WDT_CLK                                                           */
} E_WDT_INTERVAL;

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototype                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_Init(void);
int32_t DrvTIMER_Open(E_TIMER_CHANNEL ch, uint32_t uTicksPerSecond, E_TIMER_OPMODE op_mode);
int32_t DrvTIMER_Close(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_SetTimerEvent(E_TIMER_CHANNEL ch, uint32_t uInterruptTicks, TIMER_CALLBACK pTimerCallback, uint32_t parameter);
void DrvTIMER_ClearTimerEvent(E_TIMER_CHANNEL ch, uint32_t uTimerEventNo);
int32_t DrvTIMER_EnableInt(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_DisableInt(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_GetIntFlag(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_ClearIntFlag(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_Start(E_TIMER_CHANNEL ch);
uint32_t DrvTIMER_GetIntTicks(E_TIMER_CHANNEL ch);
int32_t DrvTIMER_ResetIntTicks(E_TIMER_CHANNEL ch);
void DrvTIMER_Delay(E_TIMER_CHANNEL ch, uint32_t uTicks);
void DrvTIMER_SetEXTClockFreq(uint32_t u32ClockFreq);
uint32_t DrvTIMER_GetVersion(void);

void DrvWDT_Open(E_WDT_INTERVAL WDTlevel);
void DrvWDT_Close(void);
void DrvWDT_InstallISR(WDT_CALLBACK pvWDTISR);
int32_t DrvWDT_Ioctl(E_WDT_CMD uWDTCmd, uint32_t uArgument);

#endif	// __DRVTIMER_H__



