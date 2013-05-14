/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVSPI_H__
#define __DRVSPI_H__

#include "NUC1xx.h"
#include "System/SysInfra.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVSPI_MAJOR_NUM 1
#define DRVSPI_MINOR_NUM 02
#define DRVSPI_BUILD_NUM 003

#define DRVSPI_VERSION_NUM    _SYSINFRA_VERSION(DRVSPI_MAJOR_NUM,DRVSPI_MINOR_NUM,DRVSPI_BUILD_NUM)  

/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*  E_DRVSPI_ERR_BURST_CNT                 Wrong Burst Number           
    E_DRVSPI_ERR_SUSPEND_INTERVAL          Wrong Suspend interval
    E_DRVSPI_ERR_BIT_LENGTH                Wrong Bit Length
    E_DRVSPI_ERR_INIT                      Init Fail
    E_DRVSPI_ERR_BUSY                      SPI is busy
*/
#define E_DRVSPI_ERR_BURST_CNT            _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 0)
#define E_DRVSPI_ERR_SUSPEND_INTERVAL     _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 1)
#define E_DRVSPI_ERR_BIT_LENGTH           _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 2)
#define E_DRVSPI_ERR_INIT                 _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 3)
#define E_DRVSPI_ERR_BUSY                 _SYSINFRA_ERRCODE(TRUE,  MODULE_ID_DRVSPI, 4)

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Setting                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum {
    eDRVSPI_PORT0=0,
    eDRVSPI_PORT1=1,
    eDRVSPI_PORT2=2,
    eDRVSPI_PORT3=3
} E_DRVSPI_PORT;

typedef enum {
    eDRVSPI_MASTER,
    eDRVSPI_SLAVE
} E_DRVSPI_MODE;

typedef enum {
    eDRVSPI_TYPE0,
    eDRVSPI_TYPE1,
    eDRVSPI_TYPE2,
    eDRVSPI_TYPE3,
    eDRVSPI_TYPE4,
    eDRVSPI_TYPE5,
    eDRVSPI_TYPE6,
    eDRVSPI_TYPE7
} E_DRVSPI_TRANS_TYPE;

typedef enum {
    eDRVSPI_LSB_FIRST,
    eDRVSPI_MSB_FIRST
} E_DRVSPI_ENDIAN;

typedef enum {
    eDRVSPI_BYTE_REORDER_SUSPEND_DISABLE=0,
    eDRVSPI_BYTE_REORDER_SUSPEND=1,
    eDRVSPI_BYTE_REORDER=2,
    eDRVSPI_BYTE_SUSPEND=3
} E_DRVSPI_BYTE_REORDER;

typedef enum {
    eDRVSPI_EDGE_TRIGGER,
    eDRVSPI_LEVEL_TRIGGER
} E_DRVSPI_SSLTRIG;

typedef enum {
    eDRVSPI_ACTIVE_LOW_FALLING,
    eDRVSPI_ACTIVE_HIGH_RISING
} E_DRVSPI_SS_ACT_TYPE;

typedef enum {
    eDRVSPI_NONE,
    eDRVSPI_SS0,
    eDRVSPI_SS1,
    eDRVSPI_SS0_SS1
} E_DRVSPI_SLAVE_SEL;

typedef enum {
    eDRVSPI_TX_DMA,
    eDRVSPI_RX_DMA
} E_DRVSPI_DMA_MODE;

typedef void (*PFN_DRVSPI_CALLBACK)(uint32_t userData);

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvSPI_Open(E_DRVSPI_PORT eSpiPort, E_DRVSPI_MODE eMode, E_DRVSPI_TRANS_TYPE eType, int32_t i32BitLength);
void DrvSPI_Close(E_DRVSPI_PORT eSpiPort);
void DrvSPI_Set2BitTransferMode(E_DRVSPI_PORT eSpiPort, uint8_t bEnable);
void DrvSPI_SetEndian(E_DRVSPI_PORT eSpiPort, E_DRVSPI_ENDIAN eEndian);
int32_t DrvSPI_SetBitLength(E_DRVSPI_PORT eSpiPort, int32_t i32BitLength);
int32_t DrvSPI_SetByteReorder(E_DRVSPI_PORT eSpiPort, E_DRVSPI_BYTE_REORDER eOption);
int32_t DrvSPI_SetSuspendCycle(E_DRVSPI_PORT eSpiPort, int32_t i32Interval);
void DrvSPI_SetTriggerMode(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SSLTRIG eSSTriggerMode);
void DrvSPI_SetSlaveSelectActiveLevel(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SS_ACT_TYPE eSSActType);
uint8_t DrvSPI_GetLevelTriggerStatus(E_DRVSPI_PORT eSpiPort);
void DrvSPI_EnableAutoSS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);
void DrvSPI_DisableAutoSS(E_DRVSPI_PORT eSpiPort);
void DrvSPI_SetSS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);
void DrvSPI_ClrSS(E_DRVSPI_PORT eSpiPort, E_DRVSPI_SLAVE_SEL eSlaveSel);
uint8_t DrvSPI_IsBusy(E_DRVSPI_PORT eSpiPort);
int32_t DrvSPI_BurstTransfer(E_DRVSPI_PORT eSpiPort, int32_t i32BurstCnt, int32_t i32Interval);
uint32_t DrvSPI_SetClockFreq(E_DRVSPI_PORT eSpiPort, uint32_t u32Clock1, uint32_t u32Clock2);
uint32_t DrvSPI_GetClock1Freq(E_DRVSPI_PORT eSpiPort);
uint32_t DrvSPI_GetClock2Freq(E_DRVSPI_PORT eSpiPort);
void DrvSPI_SetVariableClockFunction(E_DRVSPI_PORT eSpiPort, uint8_t bEnable, uint32_t u32Pattern);
void DrvSPI_EnableInt(E_DRVSPI_PORT eSpiPort, PFN_DRVSPI_CALLBACK pfnCallback, uint32_t u32UserData);
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort);
uint32_t DrvSPI_GetIntFlag(E_DRVSPI_PORT eSpiPort);
void DrvSPI_ClrIntFlag(E_DRVSPI_PORT eSpiPort);
uint8_t DrvSPI_SingleRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data);
uint8_t DrvSPI_SingleWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Data);
uint8_t DrvSPI_BurstRead(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf);
uint8_t DrvSPI_BurstWrite(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf);
uint32_t DrvSPI_DumpRxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount);
uint32_t DrvSPI_SetTxRegister(E_DRVSPI_PORT eSpiPort, uint32_t *pu32Buf, uint32_t u32DataCount);
void DrvSPI_SetGo(E_DRVSPI_PORT eSpiPort);
void DrvSPI_ClrGo(E_DRVSPI_PORT eSpiPort);
void DrvSPI_SetPDMA(E_DRVSPI_PORT eSpiPort, E_DRVSPI_DMA_MODE eDmaMode, uint8_t bEnable);
void DrvSPI_EnableDivOne(E_DRVSPI_PORT eSpiPort);
void DrvSPI_DisableDivOne(E_DRVSPI_PORT eSpiPort);
void DrvSPI_Enable3Wire(E_DRVSPI_PORT eSpiPort);
void DrvSPI_Disable3Wire(E_DRVSPI_PORT eSpiPort);
void DrvSPI_3WireAbort(E_DRVSPI_PORT eSpiPort);
void DrvSPI_Enable3WireStartInt(E_DRVSPI_PORT eSpiPort, PFN_DRVSPI_CALLBACK pfnCallback, uint32_t u32UserData);
void DrvSPI_DisableInt(E_DRVSPI_PORT eSpiPort);
uint32_t DrvSPI_Get3WireStartIntFlag(E_DRVSPI_PORT eSpiPort);
void DrvSPI_Clr3WireStartIntFlag(E_DRVSPI_PORT eSpiPort);
uint32_t DrvSPI_GetVersion(void);

#endif
