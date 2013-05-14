/******************************************************************************
 * @file     CDC_Class.h
 * @brief    Nano1xx USB Sample header file
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/***************************************************/
/*      CDC Class-Specific Request Codes		   */
/***************************************************/
/*!<Define CDC Class Specific Request */
#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODE               0x20
#define GET_LINE_CODE               0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23


/***************************************************/
/*!<USB VCOM Class Information Structure */
typedef struct DRVUSBD_VCOM_STRUCT
{
	uint16_t	CtrlSignal;     /* BIT0: DTR(Data Terminal Ready) , BIT1: RTS(Request To Send) */

}STR_USBD_VCOM_T;


/********************************************************/
#define RXBUFSIZE			256 /* RX buffer size */  
#define TXBUFSIZE			256 /* RX buffer size */  
#define TX_FIFO_SIZE		64	/* TX Hardware FIFO size */         


/********************* Global Variables for UART **********************/
extern uint8_t volatile comRbuf[RXBUFSIZE];
extern uint16_t volatile comRbytes;
extern uint16_t volatile comRhead;
extern uint16_t volatile comRtail;

extern uint8_t volatile comTbuf[TXBUFSIZE];
extern uint16_t volatile comTbytes;
extern uint16_t volatile comThead;
extern uint16_t volatile comTtail;



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/





