
/******************************************************************************
 * @file     nano1xx_uart.h
 * @brief    NANO1xx UART driver header file
 * @version  1.0.2
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_UART_H
#define __NANO1XX_UART_H

#include "nano1xx.h"

#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_UART_Driver UART Driver
  @{
*/

/** @addtogroup NANO1xx_UART_EXPORTED_CONSTANTS UART Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Version number								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_MAJOR_NUM	1
#define DRVUART_MINOR_NUM	00
#define DRVUART_BUILD_NUM	000

/*---------------------------------------------------------------------------------------------------------*/
/*  Version define with SysInfra				                                                           */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_VERSION_NUM		(((DRVUART_MAJOR_NUM) << 16) | ((DRVUART_MINOR_NUM) << 8) | (DRVUART_BUILD_NUM))					   
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_DRVUART_ERR_TIMEOUT   			1
#define E_DRVUART_ERR_PORT_INVALID		   	2
#define E_DRVUART_ERR_PARITY_INVALID   		3
#define E_DRVUART_ERR_DATA_BITS_INVALID 	4
#define E_DRVUART_ERR_STOP_BITS_INVALID   	4
#define E_DRVUART_ERR_TRIGGERLEVEL_INVALID  5
#define E_DRVUART_ARGUMENT                  7




/*---------------------------------------------------------------------------------------------------------*/
/* Define UART Macro		                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
#define _DRVUART_SENDBYTE(u32Port, byData)					outp32(UART0_BASE + u32Port, (byData))
#define _DRVUART_RECEIVEBYTE(u32Port)						inp32(UART0_BASE + u32Port)
#define _DRVUART_SET_DIVIDER(u32Port, u16Divider)			outp32(&UART0->BAUD + u32Port, (0x1<<31)|(u16Divider))   
#define _DRVUART_RECEIVEAVAILABLE(u32Port)				(inp32(&UART0->FSR + u32Port) & 0x1F00)>>16
#define _DRVUART_WAIT_TX_EMPTY(u32Port)			        	(inp32(&UART0->FSR + u32Port) & 0x800)>>11


/*---------------------------------------------------------------------------------------------------------*/
/* Define IRDA / LIN  Operation direction                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
#define MODE_TX  1
#define MODE_RX  2

/*---------------------------------------------------------------------------------------------------------*/
/* Define IRDA TX/RX selection                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
#define IrDA_TXSelect 1
#define IrDA_RXSelect 0

/*---------------------------------------------------------------------------------------------------------*/
/* Define UART enum      	                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* define UART line status control			                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_LININT          ((uint32_t)0x00000100)		/*!<LIN Interrupt */  
#define DRVUART_ABAUDINT        ((uint32_t)0x00000080)		/*!<Auto-Baud Rate Interrup */ 
#define DRVUART_WAKEUPINT       ((uint32_t)0x00000040)		/*!<Wake up interrupt */ 
#define DRVUART_BUFERRINT       ((uint32_t)0x00000020)		/*!<Buffer Error Interrupt */
#define DRVUART_TOUTINT         ((uint32_t)0x00000010)		/*!<RX time-out Interrupt */
#define DRVUART_MOSINT          ((uint32_t)0x00000008)		/*!<MODEM Interrupt */
#define DRVUART_RLSINT          ((uint32_t)0x00000004)		/*!<Receive Line Interrupt */
#define DRVUART_THREINT         ((uint32_t)0x00000002)		/*!<Transmit Holding Register Empty Interrupt */
#define DRVUART_RDAINT          ((uint32_t)0x00000001)		/*!<Receive Data Available Interrupt */

/*---------------------------------------------------------------------------------------------------------*/
/* DATA BIT                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_DATABITS_5          ((uint32_t)0x00000000)		/*!<Word length select: Character length is 5 bits */
#define DRVUART_DATABITS_6          ((uint32_t)0x00000001)		/*!<Word length select: Character length is 6 bits */
#define DRVUART_DATABITS_7          ((uint32_t)0x00000002)		/*!<Word length select: Character length is 7 bits */
#define DRVUART_DATABITS_8          ((uint32_t)0x00000003)		/*!<Word length select: Character length is 8 bits */

/*---------------------------------------------------------------------------------------------------------*/
/* PARITY Setting                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_PARITY_NONE          ((uint32_t)0x00000000)	/*!<None parity */
#define DRVUART_PARITY_ODD           ((uint32_t)0x00000001)	/*!<Odd parity enable */
#define DRVUART_PARITY_EVEN          ((uint32_t)0x00000003)	/*!<Even parity enable */
#define DRVUART_PARITY_MARK          ((uint32_t)0x00000005)	/*!<Parity mask */
#define DRVUART_PARITY_SPACE         ((uint32_t)0x00000007)	/*!<Parity space */

/*---------------------------------------------------------------------------------------------------------*/
/* STOP BIT                                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_STOPBITS_1           ((uint32_t)0x00000000)	/*!<Number of stop bit: Stop bit length is 1 bit */
#define DRVUART_STOPBITS_1_5         ((uint32_t)0x00000001)	/*!<Number of stop bit: Stop bit length is 1.5 bit when character length is 5 bits */
#define DRVUART_STOPBITS_2           ((uint32_t)0x00000001)	/*!<Number of stop bit: Stop bit length is 2 bit when character length is 6, 7 or 8 bits */

/*---------------------------------------------------------------------------------------------------------*/
/* FIFO Select                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_FIFO_1BYTES           ((uint32_t)0x00000000)	/*!<RX FIFO interrupt trigger level is 1 byte */
#define DRVUART_FIFO_4BYTES           ((uint32_t)0x00000001)	/*!<RX FIFO interrupt trigger level is 4 bytes */
#define DRVUART_FIFO_8BYTES           ((uint32_t)0x00000002)	/*!<RX FIFO interrupt trigger level is 8 bytes */
#define DRVUART_FIFO_14BYTES          ((uint32_t)0x00000003)	/*!<RX FIFO interrupt trigger level is 14 bytes */

/*---------------------------------------------------------------------------------------------------------*/
/* TRSR UART Transfer Status                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_TRSR_RS485_ADDET_F          ((uint32_t)0x00000001)		/*!<RS-485 Address Byte Detection Status Flag */
#define DRVUART_TRSR_ABAUD_F                ((uint32_t)0x00000002)		/*!<Auto- Bbaud Rrate Iinterrupt */
#define DRVUART_TRSR_ABAUD_TOUT_F           ((uint32_t)0x00000004)		/*!<Auto- Bbaud Rrate Ttime- Oout Iinterrupt */
#define DRVUART_TRSR_LIN_TX_F               ((uint32_t)0x00000008)		/*!<LIN TX Interrupt Flag  */
#define DRVUART_TRSR_LIN_RX_F               ((uint32_t)0x00000010)		/*!<LIN RX Interrupt Flag */
#define DRVUART_TRSR_BIT_ERR_F              ((uint32_t)0x00000020)		/*!<Bit Error Detect Status Flag */
#define DRVUART_TRSR_LIN_RX_SYNC_ERR_F      ((uint32_t)0x00000100)		/*!<LIN RX SYNC Error Flag */

/*---------------------------------------------------------------------------------------------------------*/
/* ISR STATUS                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUART_ISR_MODEM_IS  ((uint32_t)0x00000008)	/*!<MODEM Interrupt Status Flag */
#define DRVUART_ISR_WAKE_IS   ((uint32_t)0x00000040)	/*!<Wake-Uup Interrupt Status Flag */

/*---------------------------------------------------------------------------------------------------------*/
/* Function Selection                                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
#define FUN_UART  ((uint32_t)0x00000000)	/*!<Select UART function */
#define FUN_LIN   ((uint32_t)0x00000001)	/*!<Select LIN function */
#define FUN_IRDA  ((uint32_t)0x00000002)	/*!<Select IrDA function */
#define FUN_RS485 ((uint32_t)0x00000003)	/*!<Select RS485 function */

/*---------------------------------------------------------------------------------------------------------*/
/*  Define RS485 operation mode                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define MODE_RS485_NMM ((uint32_t)0x00000001)	/*!<RS-485 Normal Multidrop Operation Mode */
#define MODE_RS485_AAD ((uint32_t)0x00000002)	/*!<RS-485 Auto Address Detection Operation Mode */
#define MODE_RS485_AUD ((uint32_t)0x00000004)	/*!<RS-485 Auto Direction Mode */

/// @cond						
/*---------------------------------------------------------------------------------------------------------*/
/* Define UART Macro assert_param check                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define CHECK_UART_PORT(port)			((port == UART0) || (port == UART1))
#define CHECK_UART_PARITY(parity)		((parity==DRVUART_PARITY_NONE) || (parity==DRVUART_PARITY_EVEN) || (parity==DRVUART_PARITY_ODD) || (parity==DRVUART_PARITY_MARK) || (parity==DRVUART_PARITY_SPACE))
#define CHECK_UART_DATABIT(databit)	((databit==DRVUART_DATABITS_5) || (databit==DRVUART_DATABITS_6) || (databit==DRVUART_DATABITS_7) || (databit==DRVUART_DATABITS_8))
#define CHECK_UART_STOPBIT(stopbit)	((stopbit==DRVUART_STOPBITS_1) || (stopbit==DRVUART_STOPBITS_2) || (stopbit==DRVUART_STOPBITS_1_5))
#define CHECK_UART_RXTRIGGERLEVEL(trigger)	((trigger==DRVUART_FIFO_1BYTES) || (trigger==DRVUART_FIFO_4BYTES) || (trigger==DRVUART_FIFO_8BYTES) || (trigger==DRVUART_FIFO_14BYTES))

/// @endcond

/*@}*/ /* end of group NANO1xx_UART_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_UART_EXPORTED_STRUCTS UART Exported Structs
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  Define UART data structure                                                                             */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct DRVUART_STRUCT
{
    uint32_t	u32BaudRate;
    uint32_t	u32cDataBits;
    uint32_t	u32cStopBits;
    uint32_t	u32cParity;
    uint32_t	u32cRxTriggerLevel;
    uint8_t		u8TimeOut ;
    uint8_t		u8EnableDiv16;
}STR_UART_T;

/*---------------------------------------------------------------------------------------------------------*/
/*  Define RS485 Setting structure                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

typedef struct RS485_STRUCT
{
    uint8_t		u8cModeSelect;
    uint8_t		u8cAddrEnable;
    uint8_t	       u8cAddrValue;
    uint8_t	       u8cDelayTime;
    uint8_t	       u8cRxDisable;
}STR_RS485_T;

/*@}*/ /* end of group NANO1xx_UART_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_UART_EXPORTED_FUNCTIONS UART Exported Functions
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Define UART functions prototype                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
//void UART_DeInit(E_UART_PORT u32Port);
void UART_DeInit(UART_TypeDef *u32Port);
void UART_EnableInt(UART_TypeDef  *tUART,uint32_t u32InterruptFlag);
void UART_DisableInt(UART_TypeDef  *tUART,uint32_t u32InterruptFlag);
void UART_GetCTSInfo(UART_TypeDef  *tUART,uint8_t *pu8CTSValue,uint8_t *pu8CTSChangeState);
void UART_SetRTS(UART_TypeDef  *tUART,uint8_t u8Value,uint16_t u16TriggerLevel);

int32_t UART_Write(UART_TypeDef  *tUART,uint8_t *pu8TxBuf,uint32_t u32WriteBytes);
int32_t UART_Read(UART_TypeDef  *tUART,uint8_t *pu8RxBuf,uint32_t	u32ReadBytes);
int32_t UART_Init(UART_TypeDef  *tUART, STR_UART_T *sParam);
int32_t UART_GetIntStatus(UART_TypeDef  *tUART,uint32_t u32InterruptFlag);

uint32_t UART_ClearIntFlag(UART_TypeDef  *tUART,uint32_t u32InterruptFlag);
static void UART_EnablePDMA(UART_TypeDef  *tUART);
static void UART_DisablePDMA(UART_TypeDef  *tUART);

void UART_SetFnIRDA(UART_TypeDef  *tUART, uint8_t TxXSelect );
void UART_SetFnLIN(UART_TypeDef  *tUART,uint16_t u16Mode,uint16_t u16BreakLength);
void UART_SetFnRS485(UART_TypeDef  *tUART,STR_RS485_T *str_RS485);

uint32_t UART_GetUartClk(void);


/**
 *  @brief Return the current version number of driver.
 *
 *  @param None
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *
 *  @return 
 *          Version number: \n
 *             [31:24]: 00000000  \n
 *             [23:16]: MAJOR_NUM \n
 *             [15: 8]: MINOR_NUM \n
 *             [ 7: 0]: BUILD_NUM \n
 *
 */
static __INLINE int32_t UART_GetVersion(void)  {return DRVUART_VERSION_NUM;}


/**
 *  @brief The function is used to control disable PDMA transmit/receive channel
 *
 *  @param [in] tUART Specify UART0/UART1
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h  
 *
 *  @return None
 *
 *  @par Example:
 *  @code
 *  //Disable Tx and Rx PDMA in UART 1
 *  UART_DisablePDMA(UART_PORT1);
 *  @endcode
 *
 */
static __INLINE void UART_DisablePDMA(UART_TypeDef  *tUART)
{
	tUART->CTL 	&= ~(UART_CTL_DMA_RX_EN | UART_CTL_DMA_TX_EN);
}

/**
 *  @brief The function is used to control enable PDMA transmit/receive channel
 *
 *  @param [in] tUART Specify UART0/UART1
 *
 *  @par Include:
 *       nano1xx_drivers/nano1xx_uart.h
 *  
 *  @return None
 *
 *  @par Example:
 *  @code
 *  //Enable TX and RX PDMA in UART 1
 *  UART_EnablePDMA(UART_PORT1);
 *  @endcode
 *
 */
static __INLINE void UART_EnablePDMA(UART_TypeDef  *tUART)
{
	tUART->CTL 	|= UART_CTL_DMA_RX_EN | UART_CTL_DMA_TX_EN;
}


/*@}*/ /* end of group NANO1xx_UART_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_UART_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#ifdef  __cplusplus
}
#endif

#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/









