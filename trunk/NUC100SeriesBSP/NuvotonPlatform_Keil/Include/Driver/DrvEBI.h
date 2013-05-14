/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef DRVEBI_H
#define DRVEBI_H

#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVEBI_MAJOR_NUM	    1
#define DRVEBI_MINOR_NUM        03
#define DRVEBI_BUILD_NUM	    001

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVEBI_VERSION_NUM          _SYSINFRA_VERSION(DRVEBI_MAJOR_NUM, DRVEBI_MINOR_NUM, DRVEBI_BUILD_NUM)

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_DRVEBI_ERR_ARGUMENT		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVEBI, 1)    /* Invalid areument    */
#define E_DRVEBI_ERR_BUS_WIDTH		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVEBI, 2)    /* Invalid bus width   */

#define DRVEBI_BASE_ADDR		0x60000000	/* 0x60000000 ~ 0x6001FFFF */
#define DRVEBI_MAX_SIZE			0x20000
#define DRVEBI_TIMEOUT_COUNT	0x10000

typedef enum
{
	E_DRVEBI_DATA_8BIT	= 0x0,
	E_DRVEBI_DATA_16BIT	= 0x1,	
} E_DRVEBI_DATA_WIDTH;

typedef enum
{
	E_DRVEBI_ADDR_8BIT	= 0x0,
	E_DRVEBI_ADDR_16BIT	= 0x1,	
} E_DRVEBI_ADDR_WIDTH;

typedef enum
{
	E_DRVEBI_MCLKDIV_1	= 0x0,
	E_DRVEBI_MCLKDIV_2,
	E_DRVEBI_MCLKDIV_4,
	E_DRVEBI_MCLKDIV_8,
	E_DRVEBI_MCLKDIV_16,
	E_DRVEBI_MCLKDIV_32,
	E_DRVEBI_MCLKDIV_DEFAULT,
} E_DRVEBI_MCLKDIV;

typedef struct 
{
    E_DRVEBI_DATA_WIDTH	eDataWidth;
    E_DRVEBI_ADDR_WIDTH eAddrWidth;
    uint32_t			u32BaseAddress;
    uint32_t			u32Size;
} DRVEBI_CONFIG_T;

typedef struct 
{
    E_DRVEBI_MCLKDIV	eMCLKDIV;
    uint8_t				u8ExttALE;
    uint8_t				u8ExtIR2R;
    uint8_t				u8ExtIW2X;
    uint8_t				u8ExttAHD;
    uint8_t				u8ExttACC;
} DRVEBI_TIMING_T;

int32_t DrvEBI_Open(DRVEBI_CONFIG_T	sEBIConfig);
void DrvEBI_Close(void);
void DrvEBI_SetBusTiming(DRVEBI_TIMING_T sEBITiming);
void DrvEBI_GetBusTiming(DRVEBI_TIMING_T *psEBITiming);
uint32_t DrvEBI_GetVersion(void);

#endif








