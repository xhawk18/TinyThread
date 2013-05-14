/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef _DRVI2C_H
#define _DRVI2C_H

#include "NUC1xx.h"


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_MAJOR_NUM    1
#define DRVI2C_MINOR_NUM    02
#define DRVI2C_BUILD_NUM    2

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVI2C_VERSION_NUM     _SYSINFRA_VERSION(DRVI2C_MAJOR_NUM, DRVI2C_MINOR_NUM, DRVI2C_BUILD_NUM)
                               
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVI2C_ERR_ARGUMENT        Incorrect Argument
#define E_DRVI2C_ERR_ARGUMENT   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVI2C, 1)

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C port                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
    I2C_PORT0 = 0,
    I2C_PORT1 = 1
} E_I2C_PORT;

typedef void (*I2C_CALLBACK)(uint32_t status);

/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C Call back function type                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    I2CFUNC   = 0,
    ARBITLOSS = 1,
    BUSERROR  = 2,
    TIMEOUT   = 3
} E_I2C_CALLBACK_TYPE;

/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C Call back function Data Struct                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    I2C_CALLBACK    I2CCallBackFn;
    I2C_CALLBACK    ArbitLossCallBackFn;
    I2C_CALLBACK    BusErrCallBackFn;
    I2C_CALLBACK    TimeoutCallBackFn;

} I2C_CALLBACK_T;


/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void     DrvI2C_ClearIntFlag(E_I2C_PORT port);
void     DrvI2C_ClearTimeoutFlag(E_I2C_PORT port);
int32_t  DrvI2C_Close(E_I2C_PORT port);
void     DrvI2C_Ctrl(E_I2C_PORT port, uint8_t start, uint8_t stop, uint8_t intFlag, uint8_t ack);

int32_t  DrvI2C_DisableInt(E_I2C_PORT port);

int32_t  DrvI2C_EnableInt(E_I2C_PORT port);

uint32_t DrvI2C_GetClockFreq(E_I2C_PORT port);
uint8_t  DrvI2C_GetIntFlag(E_I2C_PORT port);
uint32_t DrvI2C_GetStatus(E_I2C_PORT port);
uint32_t DrvI2C_GetVersion(void);

int32_t  DrvI2C_InstallCallback(E_I2C_PORT port, E_I2C_CALLBACK_TYPE Type, I2C_CALLBACK callbackfn);

int32_t  DrvI2C_Open(E_I2C_PORT port, uint32_t u32BusClock);

uint8_t  DrvI2C_ReadData(E_I2C_PORT port);

int32_t  DrvI2C_SetAddress(E_I2C_PORT port, uint8_t slaveNo, uint8_t slave_addr, uint8_t GC_Flag);
int32_t  DrvI2C_SetAddressMask(E_I2C_PORT port, uint8_t slaveNo, uint8_t slaveAddrMask);
int32_t  DrvI2C_SetClockFreq(E_I2C_PORT port, uint32_t u32BusClock);
int32_t  DrvI2C_SetTimeoutCounter(E_I2C_PORT port, int32_t i32enable, uint8_t u8div4);

int32_t  DrvI2C_UninstallCallBack(E_I2C_PORT port, E_I2C_CALLBACK_TYPE Type);

void     DrvI2C_WriteData(E_I2C_PORT port, uint8_t u8data);

#endif

