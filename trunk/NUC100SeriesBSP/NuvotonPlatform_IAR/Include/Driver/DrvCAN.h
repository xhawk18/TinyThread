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
#define DRVCAN_MAJOR_NUM	3
#define DRVCAN_MINOR_NUM	01
#define DRVCAN_BUILD_NUM	001

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVCAN_VERSION_NUM     _SYSINFRA_VERSION(DRVCAN_MAJOR_NUM,DRVCAN_MINOR_NUM,DRVCAN_BUILD_NUM) 
								 
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVCAN_ERR_TIMEOUT  				Time out
// E_DRVCAN_ERR_PORT_INVALID		 	Wrong port
// E_DRVCAN_ARGUMENT                	Wrong Argument (Wrong CAN Port)
// E_DRVCAN_ERR_BITRATE                	Wrong bit rate configure 
// E_DRVCAN_NO_PENDING_MSG              No pending error to recevie 
// E_DRVCAN_NO_USEFUL_INTERFACE         No useful interface
// E_DRVCAN_NO_RECEIVED_NEWDATA         No received new data
#define E_DRVCAN_ERR_TIMEOUT   			_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 1)
#define E_DRVCAN_ERR_PORT_INVALID	  	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 2)
#define E_DRVCAN_ERR_ARGUMENT         	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 3)
#define E_DRVCAN_ERR_BITRATE          	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 4)
#define E_DRVCAN_NO_PENDING_MSG        	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 5)
#define E_DRVCAN_NO_USEFUL_INTERFACE   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 6)
#define E_DRVCAN_NO_RECEIVED_NEWDATA   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVCAN, 7)


typedef void (*CAN_CALLBACK)(uint32_t u32IIDRStatus);

/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C Call back function type                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    CALLBACK_RXOK   = 0, 
    CALLBACK_TXOK   = 1,
    CALLBACK_EWARN  = 2,
    CALLBACK_BOFF   = 3,
    CALLBACK_MSG    = 4,
    CALLBACK_WAKEUP = 5
} E_CAN_CALLBACK_TYPE;


/* Message ID types*/
typedef enum 
{
  CAN_STD_ID = 0,
  CAN_EXT_ID = 1
}E_CAN_ID_TYPE;

/* Message Frame types*/
typedef enum 
{
  REMOTE_FRAME = 0,
  DATA_FRAME   = 1
}E_CAN_FRAME_TYPE;
/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C Call back function Data Struct                                                               */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct
{
    CAN_CALLBACK    RxOkCallBackFn;
    CAN_CALLBACK    TxOkCallBackFn;    
    CAN_CALLBACK    ErrorWarningCallBackFn;
    CAN_CALLBACK    BusOffCallBackFn;
    CAN_CALLBACK    MessageCallBackFn;
    CAN_CALLBACK    WakeupCallBackFn;

}STR_CAN_CALLBACK_T;

/* CAN message structure*/
typedef struct
{
  uint32_t 	IdType;
  uint32_t 	FrameType;
  uint32_t 	Id;
  uint8_t 	DLC;
  uint8_t 	Data[8];
} STR_CANMSG_T;

/* CAN mask message structure*/
typedef struct
{
  uint8_t 	u8Xtd;
  uint8_t 	u8Dir;
  uint32_t 	u32Id;
  uint8_t 	u8IdType;
} STR_CANMASK_T;


/* Control register*/
#define CAN_CON_TEST           0x0080
#define CAN_CON_CCE            0x0040
#define CAN_CON_DAR            0x0020
#define CAN_CON_EIE            0x0008
#define CAN_CON_SIE            0x0004
#define CAN_CON_IE             0x0002
#define CAN_CON_INIT           0x0001

/* Status register */
#define CAN_STATUS_BOFF        0x0080
#define CAN_STATUS_EWARN       0x0040
#define CAN_STATUS_EPASS       0x0020
#define CAN_STATUS_RXOK        0x0010
#define CAN_STATUS_TXOK        0x0008
#define CAN_STATUS_LEC         0x0007

/* Test register*/
#define CAN_TESTR_RX           0x0080
#define CAN_TESTR_TX1          0x0040
#define CAN_TESTR_TX0          0x0020
#define CAN_TESTR_LBACK        0x0010
#define CAN_TESTR_SILENT       0x0008
#define CAN_TESTR_BASIC        0x0004

/* IFn / Command Request register*/
#define CAN_CREQ_BUSY           0x8000

/* IFn / Command Mask register*/
#define CAN_CMASK_WRRD           0x0080
#define CAN_CMASK_MASK           0x0040
#define CAN_CMASK_ARB            0x0020
#define CAN_CMASK_CONTROL        0x0010
#define CAN_CMASK_CLRINTPND      0x0008
#define CAN_CMASK_TXRQSTNEWDAT   0x0004
#define CAN_CMASK_DATAA          0x0002
#define CAN_CMASK_DATAB          0x0001

/* IFn / Mask 2 register*/
#define CAN_MASK2_MXTD           0x8000
#define CAN_MASK2_MDIR           0x4000

/* IFn / Arbitration 2 register*/
#define CAN_ARB2_MSGVAL         0x8000
#define CAN_ARB2_XTD            0x4000
#define CAN_ARB2_DIR            0x2000

/* IFn / Message Control register*/
#define CAN_MCON_NEWDAT         0x8000
#define CAN_MCON_MSGLST         0x4000
#define CAN_MCON_INTPND         0x2000
#define CAN_MCON_UMASK          0x1000
#define CAN_MCON_TXIE           0x0800
#define CAN_MCON_RXIE           0x0400
#define CAN_MCON_RMTEN          0x0200
#define CAN_MCON_TXRQST         0x0100
#define CAN_MCON_EOB            0x0080


/* ERROR status */
#define ERR_NOERROR    0x0
#define ERR_STUFF      0x1
#define ERR_FORM       0x2
#define ERR_ACK        0x3
#define ERR_BIT1       0x4
#define ERR_BIT0       0x5
#define ERR_CRC        0x6
#define ERR_UNUSED     0x7


#define ALL_MSG  32
#define MSG(id)	 id



/* Exported functions ------------------------------------------------------- */
void DrvCAN_Init(void);
void DrvCAN_Close(void);
void DrvCAN_WaitMsg(void);
void DrvCAN_EnterInitMode(void);
void DrvCAN_LeaveInitMode(void);
void DrvCAN_EnterTestMode(uint8_t TestMask);
void DrvCAN_LeaveTestMode(void);
void DrvCAN_ResetIF(uint8_t number);
void DrvCAN_ResetAllMsgObj(void);
int32_t DrvCAN_UninstallCallback(E_CAN_CALLBACK_TYPE Type);
void DrvCAN_SetTiming(uint8_t u8Tseg2, uint8_t u8Tseg1, uint8_t u8Sjw, uint32_t u32Brp);
void DrvCAN_EnableWakeUp(void);
void DrvCAN_DisableWakeUp(void);

int32_t DrvCAN_GetCANBitRate(void);
int32_t DrvCAN_ResetMsgObj(uint8_t msgobj);
int32_t DrvCAN_Open(uint32_t u32bitrate);
int32_t DrvCAN_WaitEndOfTx(void);
int32_t DrvCAN_ClrIntPnd(uint8_t msgobj);
int32_t DrvCAN_BasicSendMsg(STR_CANMSG_T* pCanMsg);
int32_t DrvCAN_BasicReceiveMsg(STR_CANMSG_T* pCanMsg);
int32_t DrvCAN_EnableInt(uint16_t u16IntEnable);
int32_t DrvCAN_DisableInt(uint16_t u16IntEnable);
int32_t DrvCAN_SetTxMsgObj(uint8_t u8msgobj, STR_CANMSG_T* pCanMsg);
int32_t DrvCAN_SetMsgObjMask(uint8_t u8msgobj,STR_CANMASK_T* MaskMsg);
int32_t DrvCAN_ReadMsgObj(uint8_t msgobj, uint8_t release, STR_CANMSG_T* pCanMsg);
int32_t DrvCAN_SetRxMsgObj(uint8_t u8msgobj, uint8_t u8idType, uint32_t u32id, uint8_t u8singleOrFifoLast);
int32_t DrvCAN_InstallCallback(E_CAN_CALLBACK_TYPE Type, CAN_CALLBACK callbackfn);
int32_t DrvCAN_GetVersion(void);

uint32_t DrvCAN_IsNewDataReceived(uint8_t msgobj);
uint32_t DrvCAN_SetTxRqst(uint8_t u8msgobj);
uint32_t DrvCAN_IsObjectValid(uint8_t msgobj);
uint32_t DrvCAN_IsTxRqstPending(uint8_t msgobj);
uint32_t DrvCAN_GetRxErrCount(void);
uint32_t DrvCAN_GetTxErrCount(void);
uint32_t DrvCAN_IsIntPending(uint8_t u8MsgObj);
                         
 
/*---------------------------------------------------------------------------------------------------------*/
/* Define CAN functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

#endif													 








