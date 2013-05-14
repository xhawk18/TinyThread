/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __DRVUART_H__
#define __DRVUART_H__

#include "NUC1xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_MAJOR_NUM	1
#define DRVUART_MINOR_NUM	02
#define DRVUART_BUILD_NUM	004

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_VERSION_NUM     _SYSINFRA_VERSION(DRVUART_MAJOR_NUM, DRVUART_MINOR_NUM, DRVUART_BUILD_NUM)
							   
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
// E_DRVUART_ERR_TIMEOUT  				Time out
// E_DRVUART_ERR_PORT_INVALID		 	Wrong port
// E_DRVUART_ERR_PARITY_INVALID			Wrong party setting
// E_DRVUART_ERR_DATA_BITS_INVALID		Wrong Data bit setting
// E_DRVUART_ERR_STOP_BITS_INVALID		Wrong Stop bit setting
// E_DRVUART_ERR_TRIGGERLEVEL_INVALID	Wrong trigger level setting
// E_DRVUART_ARGUMENT                	Wrong Argument (Wrong UART Port)
#define E_DRVUART_ERR_TIMEOUT   			_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 1)
#define E_DRVUART_ERR_PORT_INVALID		   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 2)
#define E_DRVUART_ERR_PARITY_INVALID   		_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 3)
#define E_DRVUART_ERR_DATA_BITS_INVALID 	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 4)
#define E_DRVUART_ERR_STOP_BITS_INVALID   	_SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 5)
#define E_DRVUART_ERR_TRIGGERLEVEL_INVALID  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 6)
#define E_DRVUART_ARGUMENT                  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 7)
#define E_DRVUART_ERR_OPEN                  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUART, 8)

      
typedef void (PFN_DRVUART_CALLBACK)(uint32_t userData);

/*---------------------------------------------------------------------------------------------------------*/
/* Define UART Macro		                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
#define _DRVUART_SENDBYTE(u32Port,byData)					outp32(UART0_BASE + u32Port, (byData))
#define _DRVUART_RECEIVEBYTE(u32Port)						inp32(UART0_BASE + u32Port)
#define _DRVUART_SET_DIVIDER(u32Port,u16Divider)			outp32(&UART0->BAUD + u32Port,(0x3<<28)|(u16Divider))    /* Using Mode3 */
#define _DRVUART_RECEIVEAVAILABLE(u32Port)					(inp32(&UART0->FSR + u32Port) & 0x3F00)>>8
#define _DRVUART_WAIT_TX_EMPTY(u32Port)			        	(inp32(&UART0->FSR + u32Port) & (1<<28))>>28


/*---------------------------------------------------------------------------------------------------------*/
/* Define IRDA / LIN  Operation direction                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
#define MODE_TX  1
#define MODE_RX  2

/*---------------------------------------------------------------------------------------------------------*/
/* Define UART enum      	                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/*  Define UART Channel Sturcture                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	UART_PORT0 = 0x000, 
	UART_PORT1 = 0x100000,
	UART_PORT2 = 0x104000
} E_UART_PORT;

/*---------------------------------------------------------------------------------------------------------*/
/* define UART Interrupt Source                                                                     	   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	 DRVUART_LININT   	=    BIT8,	    /* LIN RX Break Field Detected Interrupt */
     DRVUART_WAKEUPINT  =    BIT7,      /* Wakeup Enable */
     DRVUART_BUFERRINT  =	 BIT5,		/* Buffer Error Interrupt */
     DRVUART_TOUTINT   	=	 BIT4,	    /* RX Time out Interrupt */
	 DRVUART_MOSINT   	=	 BIT3,	    /* MODEM Status Interrupt */
	 DRVUART_RLSINT   	=	 BIT2,	    /* Receive Line Status Interrupt*/
	 DRVUART_THREINT   	=	 BIT1,	    /* Transmit Holding Register Empty Interrupt */
	 DRVUART_RDAINT   	=	 BIT0		/* Receive Data Available Interrupt */
} E_INT_SOURCE;


/*---------------------------------------------------------------------------------------------------------*/
/* define UART Interrupt Flag    			                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	 DRVUART_LININT_FLAG   	 =   BIT15,	    /* LIN RX Break Field Detected Interrupt flag*/
     DRVUART_BUFERRINT_FLAG  =	 BIT13,		/* Buffer Error Interrupt flag*/
     DRVUART_TOUTINT_FLAG    =	 BIT12,	    /* RX Time out Interrupt flag*/
	 DRVUART_MOSINT_FLAG   	 =	 BIT11,	    /* MODEM Status Interrupt flag*/
	 DRVUART_RLSINT_FLAG   	 =	 BIT10,	    /* Receive Line Status Interrupt flag*/
	 DRVUART_THREINT_FLAG    =	 BIT9,	    /* Transmit Holding Register Empty Interrupt flag*/
	 DRVUART_RDAINT_FLAG   	 =	 BIT8		/* Receive Data Available Interrupt flag*/
} E_INT_FLAG;

/*---------------------------------------------------------------------------------------------------------*/
/* DATA BIT                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	 DRVUART_DATABITS_5   =    0x0,
	 DRVUART_DATABITS_6	  =	   0x1,
     DRVUART_DATABITS_7   =	   0x2,
     DRVUART_DATABITS_8   =	   0x3

} E_DATABITS_SETTINS;

/*---------------------------------------------------------------------------------------------------------*/
/* PARITY Setting                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	 DRVUART_PARITY_NONE   =    0x0,
	 DRVUART_PARITY_ODD	   =	0x1,
     DRVUART_PARITY_EVEN   =	0x3,
     DRVUART_PARITY_MARK   =	0x5,
     DRVUART_PARITY_SPACE  =	0x7
} E_PARITY_SETTINS;

/*---------------------------------------------------------------------------------------------------------*/
/* STOP BIT                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	 DRVUART_STOPBITS_1  	=   0x0,
	 DRVUART_STOPBITS_1_5	=   0x1,
     DRVUART_STOPBITS_2   	=   0x1
} E_STOPBITS_SETTINS;


/*---------------------------------------------------------------------------------------------------------*/
/* FIFO Select                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum 
{
	DRVUART_FIFO_1BYTES 	= 0x0, 
	DRVUART_FIFO_4BYTES 	= 0x1,
	DRVUART_FIFO_8BYTES 	= 0x2,
	DRVUART_FIFO_14BYTES 	= 0x3,
	DRVUART_FIFO_30BYTES 	= 0x4,
	DRVUART_FIFO_46BYTES 	= 0x5,
	DRVUART_FIFO_62BYTES 	= 0x6
} E_FIFO_SETTINGS;




/*---------------------------------------------------------------------------------------------------------*/
/* Function Selection                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	FUN_UART 	= 0, 
	FUN_LIN 	= 1,
	FUN_IRCR 	= 2,
	FUN_RS485 	= 3
} E_UART_FUNC;

/*---------------------------------------------------------------------------------------------------------*/
/*  Define RS485 operation mode                                                                            */
/*---------------------------------------------------------------------------------------------------------*/

typedef enum 
{
	MODE_RS485_NMM = 1, 
	MODE_RS485_AAD = 2,
	MODE_RS485_AUD = 4
} E_MODE_RS485;
									


/*---------------------------------------------------------------------------------------------------------*/
/*  Define UART data structure                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct DRVUART_STRUCT
{
    uint32_t            u32BaudRate;
    E_DATABITS_SETTINS  u8cDataBits;
    E_STOPBITS_SETTINS  u8cStopBits;
    E_PARITY_SETTINS 	u8cParity;
    E_FIFO_SETTINGS     u8cRxTriggerLevel;
    uint8_t             u8TimeOut ;
}STR_UART_T;

/*---------------------------------------------------------------------------------------------------------*/
/*  Define IRDA data structure                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct IRCR_STRUCT
{
    uint8_t   	    u8cTXSelect;
    uint8_t			u8cInvTX;
    uint8_t	        u8cInvRX;
}STR_IRCR_T;

/*---------------------------------------------------------------------------------------------------------*/
/*  Define RS485 Setting structure                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct RS485_STRUCT
{
    uint8_t   	    u8cModeSelect;
    uint8_t			u8cAddrEnable;
    uint8_t	        u8cAddrValue;
	uint8_t	        u8cDelayTime;
	uint8_t	        u8cRxDisable;
}STR_RS485_T;


 
/*---------------------------------------------------------------------------------------------------------*/
/* Define UART functions prototype                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvUART_Close(E_UART_PORT u32Port);
void DrvUART_EnableInt(E_UART_PORT u32Port,uint32_t u32InterruptFlag,PFN_DRVUART_CALLBACK pfncallback);
void DrvUART_DisableInt(E_UART_PORT u32Port,uint32_t u32InterruptFlag);
void DrvUART_GetClcok(uint8_t *pu8ClcokSrc, uint8_t *pu8ClcokDiv);
void DrvUART_GetCTSInfo(E_UART_PORT u32Port,uint8_t *pu8CTSValue,uint8_t *pu8CTSChangeState);
void DrvUART_SetClcok(uint8_t u8ClcokSrc, uint8_t u8ClcokDiv);
void DrvUART_SetRTSTrigLevel(E_UART_PORT u32Port,uint16_t u16TriggerLevel);
void DrvUART_SetRTS(E_UART_PORT u32Port,uint8_t u8Value,uint16_t u16TriggerLevel);

int32_t DrvUART_Write(E_UART_PORT u32Port,uint8_t *pu8TxBuf,uint32_t u32WriteBytes);
int32_t DrvUART_Read(E_UART_PORT u32Port,uint8_t *pu8RxBuf,uint32_t	u32ReadBytes);
int32_t DrvUART_Open(E_UART_PORT u32Port, STR_UART_T *sParam);
int32_t DrvUART_GetIntStatus(E_UART_PORT u32Port,uint32_t u32InterruptFlag);
int32_t DrvUART_GetVersion(void);

uint32_t DrvUART_ClearIntFlag(E_UART_PORT u32Port,uint32_t u32InterruptFlag);
void DrvUART_EnablePDMA(E_UART_PORT u32Port);
void DrvUART_DisalbePDMA(E_UART_PORT u32Port);

void DrvUART_SetFnIRDA(E_UART_PORT u32Port,STR_IRCR_T *str_IRCR );
void DrvUART_SetFnLIN(E_UART_PORT u32Port,uint16_t u16Mode,uint16_t u16BreakLength);
void DrvUART_SetFnRS485(E_UART_PORT u32Port,STR_RS485_T *str_RS485);
#ifdef  __cplusplus
}
#endif

#endif











