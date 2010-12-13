/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVCAN_H__
#define __DRVCAN_H__


#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVCAN_MAJOR_NUM	1
#define DRVCAN_MINOR_NUM	02
#define DRVCAN_BUILD_NUM	002

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVCAN_VERSION_NUM     _SYSINFRA_VERSION(DRVCAN_MAJOR_NUM,DRVCAN_MINOR_NUM,DRVCAN_BUILD_NUM) 
								 
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVCAN_ERR_TIMEOUT  				Time out
// E_DRVCAN_ERR_PORT_INVALID		 	Wrong port
// E_DRVCAN_ARGUMENT                	Wrong Argument (Wrong UART Port)
#define E_DRVCAN_ERR_TIMEOUT   				_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 1)
#define E_DRVCAN_ERR_PORT_INVALID	  		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 2)
#define E_DRVCAN_ERR_ARGUMENT               _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 3)
#define E_DRVCAN_ERR_BITRATE                _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 4)


typedef void (PFN_DRVCAN_CALLBACK)(uint32_t userData);


/*---------------------------------------------------------------------------------------------------------*/
/*  Define FORMAT	// 0 - STANDARD, 	1- EXTENDED IDENTIFIER						                       */
/*---------------------------------------------------------------------------------------------------------*/
#define STANDARD_FORMAT     0
#define EXTENDED_FORMAT		1


/*---------------------------------------------------------------------------------------------------------*/
/*  Define TYPE		// 0 - DATA FRAME, 	1- REMOTE FRAME								                       */
/*---------------------------------------------------------------------------------------------------------*/
#define DATA_TYPE     	0
#define REMOTE_TYPE		1


/*---------------------------------------------------------------------------------------------------------*/
/*  Define Mask 																	                       */
/*---------------------------------------------------------------------------------------------------------*/
#define AMR_ALLPASS  0x0
#define AMR_ALLMASK  0xFFFFFFFF


/*---------------------------------------------------------------------------------------------------------*/
/* Interrupt Flag        								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	DRVCAN_INT_RI  =  BIT0,	     /* Recevie Interrupt */
	DRVCAN_INT_TI  =  BIT1,		 /* Transmit Interrupt */
	DRVCAN_INT_WUI =  BIT4,
	DRVCAN_INT_ALI =  BIT6,
	DRVCAN_INT_BEI =  BIT7

} E_DRVCAN_INT_SOURCE;

/*---------------------------------------------------------------------------------------------------------*/
/* Error Capture        								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	DRVCAN_ERRSTUFF =0,    /* Stuff Error 			*/
	DRVCAN_ERRFORM  =1,    /* Form Error 			*/
	DRVCAN_ERRCRC   =2,    /* CRC Error 			*/
	DRVCAN_ERRACK   =3,    /* Acknowledge Error  	*/
	DRVCAN_ERRBIT   =4     /* Bit Error 			*/
} E_DRVCAN_ERRFLAG;

/*---------------------------------------------------------------------------------------------------------*/
/*  Define CAN Callback funciton type structure                                                            */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
	TYPE_RI  	=  0,	     
	TYPE_TI  	=  1,		 
	TYPE_WUI	=  2,
	TYPE_ALI	=  3,
	TYPE_BEI 	=  4
} E_DRVCAN_CALLBACK_TYPE;



/*---------------------------------------------------------------------------------------------------------*/
/*  Define CAN initialization data structure                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct DRVCAN_STRUCT		  
{
    uint32_t        id;				/* ID identifier */
    uint32_t    	u32cData[2];	/* Data field */
    uint32_t   	    u8cLen;			/* Length of data field in bytes */
    uint8_t			u8cFormat;	  	/* 0 - STANDARD, 	1- EXTENDED IDENTIFIER */
    uint8_t	        u8cType;		/* 0 - DATA FRAME, 	1- REMOTE FRAME */
}STR_CAN_T;




/*---------------------------------------------------------------------------------------------------------*/
/* Define CAN Call back function Data Struct                                                               */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    
	PFN_DRVCAN_CALLBACK	*BusErrorCallBackFn;	
	PFN_DRVCAN_CALLBACK	*ArbitLostCallBackFn;
    PFN_DRVCAN_CALLBACK	*WakeUpCallBackFn;    
    PFN_DRVCAN_CALLBACK	*TransmitCallBackFn;
    PFN_DRVCAN_CALLBACK	*ReceiveCallBackFn;

} CAN_CALLBACK_T;

 
/*---------------------------------------------------------------------------------------------------------*/
/* Define CAN functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
 void    DrvCAN_Init(void);
 int32_t DrvCAN_Open(int32_t i32CANKiloBitRate,int16_t i16SamplePointPos);
 int32_t DrvCAN_Close(void);
 int32_t DrvCAN_DisableInt(uint32_t u32InterruptFlag);
 int32_t DrvCAN_EnableInt(uint32_t u32InterruptFlag);
 int32_t DrvCAN_GetErrorStatus(E_DRVCAN_ERRFLAG u32ErrorFlag);
 int32_t DrvCAN_WaitReady(void);
 int32_t DrvCAN_Write(STR_CAN_T *Msg);
 int32_t DrvCAN_SetAcceptanceFilter(int32_t id_Filter );
 int32_t DrvCAN_SetMaskFilter(int32_t id_Filter );
 int32_t DrvCAN_GetVersion(void);
 int32_t DrvCAN_GetClockFreq(void);
 STR_CAN_T DrvCAN_Read(void);
 int32_t DrvCAN_SetBusTiming(int8_t i8SynJumpWidth,int16_t i16TimeSeg1,int8_t i8TimeSeg2,int8_t SampPtNo);
 void DrvCAN_ReTransmission(int32_t bIsEnable);
 int32_t DrvCAN_GetTxErrorCount(void);
 int32_t DrvCAN_GetRxErrorCount(void);
 void DrvCAN_InstallCallback(E_DRVCAN_CALLBACK_TYPE Type, PFN_DRVCAN_CALLBACK callbackfn);
 void DrvCAN_UninstallCallback(E_DRVCAN_CALLBACK_TYPE Type);
#endif													 








