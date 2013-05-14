/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVPS2_H__
#define __DRVPS2_H__

#include "NUC1xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPS2_MAJOR_NUM	1
#define DRVPS2_MINOR_NUM	2
#define DRVPS2_BUILD_NUM	2

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define _DRVPS2_VERSION_NUM     _SYSINFRA_VERSION(DRVPS2_MAJOR_NUM, DRVPS2_MINOR_NUM, DRVPS2_BUILD_NUM)
							   
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVPS2_ERR_TIMEOUT  				Time out
// E_DRVPS2_ERR_PORT_INVALID		 	Wrong port
// E_DRVPS2_ERR_PARITY_INVALID			Wrong party setting
// E_DRVPS2_ERR_DATA_BITS_INVALID		Wrong Data bit setting
// E_DRVPS2_ERR_STOP_BITS_INVALID		Wrong Stop bit setting
// E_DRVPS2_ERR_TRIGGERLEVEL_INVALID	Wrong trigger level setting
// E_DRVPS2_ARGUMENT                	Wrong Argument (Wrong UART Port)
#define E_DRVPS2_ERR_TIMEOUT   			    _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 1)
#define E_DRVPS2_ERR_PORT_INVALID		   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 2)
#define E_DRVPS2_ERR_PARITY_INVALID   		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 3)
#define E_DRVPS2_ERR_DATA_BITS_INVALID 	    _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 4)
#define E_DRVPS2_ERR_STOP_BITS_INVALID      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 5)
#define E_DRVPS2_ERR_TRIGGERLEVEL_INVALID   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 6)
#define E_DRVPS2_ARGUMENT                   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVPS2, 7)


typedef void (PFN_DRVPS2_CALLBACK)(uint32_t u32IntStatus);

#define DRVPS2_TXFIFODEPTH	16
/*---------------------------------------------------------------------------------------------------------*/
/* define PS2 interrupt bit			                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVPS2_RXINT		BIT0
#define DRVPS2_TXINT		BIT1

/*---------------------------------------------------------------------------------------------------------*/
/* define macro function                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define _DRVPS2_PS2CLK(state)	(PS2->PS2CON.FPS2CLK = (state))
#define _DRVPS2_PS2DATA(state)	(PS2->PS2CON.FPS2DAT = (state))

/*
* 1: ctrl by SW;0: ctrl by internal state machine
*/
#define _DRVPS2_OVERRIDE(state)	(PS2->PS2CON.OVERRIDE = (state))
#define _DRVPS2_CLRFIFO()		PS2->PS2CON.CLRFIFO = 1; PS2->PS2CON.CLRFIFO = 0
/*
* if parity error or stop bit not received, don't send ACK bit at 12th clock
*/
#define _DRVPS2_ACKNOTALWAYS()	(PS2->PS2CON.ACK = 1)
#define _DRVPS2_ACKALWAYS()		(PS2->PS2CON.ACK = 0)

#define _DRVPS2_RXINTENABLE()	(PS2->PS2CON.RXINTEN = 1)
#define _DRVPS2_RXINTDISABLE()	(PS2->PS2CON.RXINTEN = 0)
#define _DRVPS2_TXINTENABLE()	(PS2->PS2CON.TXINTEN = 1)
#define _DRVPS2_TXINTDISABLE()	(PS2->PS2CON.TXINTEN = 0)

#define _DRVPS2_PS2ENABLE()		(PS2->PS2CON.PS2EN = 1)
#define _DRVPS2_PS2DISABLE()		(PS2->PS2CON.TXINTEN = 0)

#define _DRVPS2_TXFIFO(depth)	(PS2->PS2CON.TXFIFO_DEPTH = (depth-1))

/*
* Software override PS2 CLK/DATA line
*/
#define _DRVPS2_SWOVERRIDE(data, clk)		PS2->PS2CON.FPS2DAT=data;\
								PS2->PS2CON.FPS2CLK=clk;\
								PS2->PS2CON.OVERRIDE = 1

/*
* clear interrupt status function
*/
#define _DRVPS2_INTCLR(intclr)	(PS2->u32INTID = (intclr))

/*
* get Rx data byte function
*/
#define _DRVPS2_RXDATA()	(PS2->RXDATA)
/*
* Send data to tx fifo
*/
#define _DRVPS2_TXDATAWAIT(data, len)	while(_DRVPS2_ISTXEMPTY() == 0);_DRVPS2_TXFIFO(len);PS2->TXDATA[0]=(data)
#define _DRVPS2_TXDATA(data, len)	_DRVPS2_TXFIFO(len);PS2->TXDATA[0]=(data)
#define _DRVPS2_TXDATA0(data)	PS2->TXDATA[0]=(data)
#define _DRVPS2_TXDATA1(data)	PS2->TXDATA[1]=(data)
#define _DRVPS2_TXDATA2(data)	PS2->TXDATA[2]=(data)
#define _DRVPS2_TXDATA3(data)	PS2->TXDATA[3]=(data)

/*
* PS2 status function
*/
#define _DRVPS2_ISTXEMPTY()	(PS2->STATUS.TXEMPTY)
#define _DRVPS2_ISFRAMEERR()	(PS2->STATUS.FRAMERR)
#define _DRVPS2_ISRXBUSY()	(PS2->STATUS.RXBUSY)

/*---------------------------------------------------------------------------------------------------------*/
/* Define PS2 functions prototype                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPS2_Close(void);
void DrvPS2_DisableInt(uint32_t u32InterruptFlag);
void DrvPS2_SetTxFIFODepth(uint16_t	u32TxFIFODepth);


int8_t DrvPS2_GetIntStatus(uint32_t u32InterruptFlag);
int32_t DrvPS2_Read(uint8_t *pu8RxBuf);
int32_t DrvPS2_Open(void);
int32_t DrvPS2_Write(uint32_t *pu32TxBuf,uint32_t u32WriteBytes);
int32_t DrvPS2_EnableInt(uint32_t u32InterruptFlag,PFN_DRVPS2_CALLBACK pfncallback);
int32_t DrvPS2_GetVersion(void);
uint32_t DrvPS2_IsIntEnabled(uint32_t u32InterruptFlag);
uint32_t DrvPS2_ClearInt(uint32_t u32InterruptFlag);



#ifdef  __cplusplus
}
#endif

#endif











