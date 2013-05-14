/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef _DRVPWM_H
#define _DRVPWM_H

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

#include "NUC1xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_MAJOR_NUM    1
#define DRVPWM_MINOR_NUM    03
#define DRVPWM_BUILD_NUM    2

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_VERSION_NUM     _SYSINFRA_VERSION(DRVPWM_MAJOR_NUM, DRVPWM_MINOR_NUM, DRVPWM_BUILD_NUM)
                               
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVPWM_ERR_ARGUMENT        Incorrect Argument
// E_DRVPWM_ERR_NO_SUPPORT      Does not support this function
#define E_DRVPWM_ERR_ARGUMENT   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPWM, 1)
#define E_DRVPWM_ERR_NO_SUPPORT _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPWM, 2)

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Timer                                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_TIMER0   0x00
#define DRVPWM_TIMER1   0x01
#define DRVPWM_TIMER2   0x02
#define DRVPWM_TIMER3   0x03
#define DRVPWM_TIMER4   0x04
#define DRVPWM_TIMER5   0x05
#define DRVPWM_TIMER6   0x06
#define DRVPWM_TIMER7   0x07

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Capture                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_CAP0     0x10
#define DRVPWM_CAP1     0x11
#define DRVPWM_CAP2     0x12
#define DRVPWM_CAP3     0x13
#define DRVPWM_CAP4     0x14
#define DRVPWM_CAP5     0x15
#define DRVPWM_CAP6     0x16
#define DRVPWM_CAP7     0x17

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Capture Interrupt Enable/Disable Flag                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_CAP_RISING_INT   1
#define DRVPWM_CAP_FALLING_INT  2
#define DRVPWM_CAP_ALL_INT      3

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Capture Latched Indicator                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_CAP_RISING_FLAG  6
#define DRVPWM_CAP_FALLING_FLAG 7

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Clcok Selector                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_CLOCK_DIV_1    4
#define DRVPWM_CLOCK_DIV_2    0
#define DRVPWM_CLOCK_DIV_4    1
#define DRVPWM_CLOCK_DIV_8    2
#define DRVPWM_CLOCK_DIV_16   3

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Clcok Source Selector                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_EXT_12M        0
#define DRVPWM_EXT_32K        1
#define DRVPWM_HCLK           2
#define DRVPWM_INTERNAL_22M   3

/*---------------------------------------------------------------------------------------------------------*/
/*  PWM Mode Selector                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPWM_AUTO_RELOAD_MODE 1
#define DRVPWM_ONE_SHOT_MODE    0

/*---------------------------------------------------------------------------------------------------------*/
/* Define PWM Time Data Struct                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint8_t   u8Mode;
    uint8_t   u8HighPulseRatio;
    uint8_t   u8ClockSelector;
    uint8_t   u8PreScale;
    uint32_t  u32Frequency; 
    uint32_t  u32Duty;
    int32_t   i32Inverter;
}S_DRVPWM_TIME_DATA_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Define PWM Call back funvtion Data Struct                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVPWM_CALLBACK)(void);

/*---------------------------------------------------------------------------------------------------------*/
/* Define PWM Call back function Data Struct                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    PFN_DRVPWM_CALLBACK pfnPWM0CallBack;    
    PFN_DRVPWM_CALLBACK pfnCAP0CallBack;
   
    PFN_DRVPWM_CALLBACK pfnPWM1CallBack;    
    PFN_DRVPWM_CALLBACK pfnCAP1CallBack;
    
    PFN_DRVPWM_CALLBACK pfnPWM2CallBack;    
    PFN_DRVPWM_CALLBACK pfnCAP2CallBack;
    
    PFN_DRVPWM_CALLBACK pfnPWM3CallBack;    
    PFN_DRVPWM_CALLBACK pfnCAP3CallBack;        
   
}S_DRVPWM_CALLBACK_T;

/*---------------------------------------------------------------------------------------------------------*/
/* Define PWM functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void     DrvPWM_ClearCaptureIntStatus(uint8_t u8Capture, uint8_t u8IntType);
void     DrvPWM_ClearInt(uint8_t u8Timer);
void     DrvPWM_Close(void);

void     DrvPWM_DisableInt(uint8_t u8Timer);

void     DrvPWM_Enable(uint8_t u8Timer, int32_t i32Enable);
void     DrvPWM_EnableDeadZone(uint8_t u8Timer, uint8_t u8Length, int32_t i32EnableDeadZone);
void     DrvPWM_EnableInt(uint8_t u8Timer, uint8_t u8Int, PFN_DRVPWM_CALLBACK pfncallback);

int32_t  DrvPWM_GetCaptureIntStatus(uint8_t u8Capture, uint8_t u8IntType);

uint16_t DrvPWM_GetFallingCounter(uint8_t u8Capture);
int32_t  DrvPWM_GetIntFlag(uint8_t u8Timer);
uint16_t DrvPWM_GetRisingCounter(uint8_t u8Capture);
uint32_t DrvPWM_GetTimerCounter(uint8_t u8Timer);
uint32_t DrvPWM_GetVersion (void);

int32_t  DrvPWM_IsTimerEnabled(uint8_t u8Timer);

void     DrvPWM_Open(void);

int32_t  DrvPWM_SelectClearLatchFlagOption(int32_t i32option);
void     DrvPWM_SelectClockSource(uint8_t u8Timer, uint8_t u8ClockSourceSelector);
uint32_t DrvPWM_SetTimerClk(uint8_t u8Timer, S_DRVPWM_TIME_DATA_T *sPt);
void     DrvPWM_SetTimerCounter(uint8_t u8Timer, uint16_t u16Counter);
void     DrvPWM_SetTimerIO(uint8_t u8Timer, int32_t i32Enable);

#ifdef  __cplusplus
}
#endif

#endif



