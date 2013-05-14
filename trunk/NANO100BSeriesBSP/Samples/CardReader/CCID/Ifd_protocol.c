/******************************************************************************
 * @file     Ifd_protocol.c
 * @brief    This file provides all the smartcard firmware functions.
 * @version  V1.0.7
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "nano1xx_smartcard.h"
#include "Ifd_protocol.h"
#include "ccid_class.h"
#include "ccid_if.h"

//#define CCID_SC_DEBUG
#ifdef CCID_SC_DEBUG
#define CCIDSCDEBUG     printf
#else
#define CCIDSCDEBUG(...)
#endif


/* EMV for T=1 */
uint8_t g_ifs_req_flag = 0;


extern uint8_t UsbMessageBuffer[];
extern volatile S_SC_DEV_T sc_device[SMARTCARD_NUM];

static volatile unsigned char IccTransactionType;
static volatile unsigned char IccTransactionLevel;

extern const uint8_t USB_ConfigDescriptor[];

/* global buffer is used for IST to receive */
extern uint8_t g_Port0_RxBUF[];
extern uint8_t g_Port1_RxBUF[];

/* global buffer is used for IST to transmit */
extern uint8_t g_Port0_TxBUF[];
extern uint8_t g_Port1_TxBUF[];

extern int SendRBlock ( S_SC_DEV_T *dev, char nr, char error);


/*
 * 00h: response APDU begins and ends in this command
 * 01h: response APDU begins with this command and is to continue
 * 02h: abData field continues the response APDU and ends the response APDU
 * 03h: abData field continues the response APDU and another block is to follow
 * 10h: empty abDat field, continuation of the command APDU is expected in next PC_to_RDR_XfrBlock command
 */
unsigned char g_ChainParameter = 0x00;

typedef struct 
{
	unsigned char FiDi;
	unsigned char Tcckst;
	unsigned char GuardTime;
	unsigned char WaitingInteger;
	unsigned char ClockStop;
	unsigned char Ifsc;				/* just for protocol T=1 */
	unsigned char Nad;				/* just for protocol T=1 */
}	Param;

static Param IccParameters[SMARTCARD_NUM];


/**
  * @brief  Transfer SC driver status to CCID error code
  * @param  ErrCode		SC driver's error code
  * @retval Slot Status Error Code
  */
unsigned char Ifd_SC2CCIDErrorCode(int32_t ErrCode)
{
	if(ErrCode == SC_ERR_TIME0OUT)
		return SLOTERR_ICC_MUTE;

	else if(ErrCode == SC_ERR_TIME2OUT)
		return SLOTERR_ICC_MUTE;
	
	else if(ErrCode == SC_ERR_AUTOCONVENTION)
		return SLOTERR_BAD_ATR_TS;

	else if(ErrCode == ATR_ERR_INVALID_TCK)
		return SLOTERR_BAD_ATR_TCK;
	
	else if(ErrCode == SC_ERR_READ)
		return SLOTERR_XFR_OVERRUN;
	
	else if(ErrCode == SC_ERR_WRITE)
		return SLOTERR_HW_ERROR;
	
	else if(ErrCode == PROROCOL_T1_P_ERR_NOTICE)
		return SLOTERR_XFR_PARITY_ERROR;
	
	else if(ErrCode == SC_ERR_PARITY_ERROR)
		return SLOTERR_XFR_PARITY_ERROR;
	
	else if(ErrCode == SC_ERR_PARITY_ERROR)
		return SLOTERR_XFR_PARITY_ERROR;
	
	else if(ErrCode == SC_ERR_CARD_REMOVED)
		return SLOTERR_ICC_MUTE;

	else if(ErrCode == SC_ERR_CARD_REMOVED)
		return SLOTERR_ICC_MUTE;

	else if(ErrCode == SC_EBUSY)
		return SLOTERR_CMD_SLOT_BUSY;

	else if(ErrCode == ATR_ERR_INVALID_PARAMETER)
		return SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED;

	else if(ErrCode == PROTOCOL_T0_ERROR)
		return SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED;

	else if(ErrCode == PROTOCOL_T1_ERROR)
		return SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED;

	else if(ErrCode == T1_ERROR_LENGTH)
		return SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED;

	else if(ErrCode == T1_ABORT_RECEIVED)
		return SLOTERR_CMD_ABORTED;

	else
		return SLOT_NO_ERROR;
}


/**
  * @brief  Set the default value applying to CCID protcol data structure
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval Slot Status Error Code
  */
unsigned char Ifd_Init( int32_t portno )
{
	S_SC_DEV_T *dev;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	/* According to dwFeatures of USB_ConfigDescriptor setting where are 40 offset  */
	/* configuration + interface + class descriptor = 9+9+41=59 */
	/* It defines what's exchange level to use */
	switch (USB_ConfigDescriptor[42+18] & 0xF)
	{
		case 0x0:
			SetChar_bmTransactionLevel;
			break;
		case 0x1:
			SetTpdu_bmTransactionLevel;
			break;
		case 0x2:
			SetShortapdu_bmTransactionLevel;
			break;
		case 0x4:
			SetShortExtendapdu_bmTransactionLevel;
			break;
		default:
			return SLOTERR_CMD_NOT_SUPPORTED;
	}
	SetT0_bTransactionType;
	//XfrFlag = INS;		// mask by smfan

	IccParameters[portno].FiDi = DEFAULT_FIDI;
	IccParameters[portno].Tcckst = DEFAULT_T01CONVCHECKSUM;

	if( dev->CardCapabilities.InversConvention == false)
		IccParameters[portno].Tcckst &= ~(0x02);								/* direct convention */
	else 
		IccParameters[portno].Tcckst |= 0x02;									/* inverse convention */
	
	IccParameters[portno].GuardTime = DEFAULT_GUARDTIME;
	IccParameters[portno].WaitingInteger = DEFAULT_WAITINGINTEGER;
	IccParameters[portno].ClockStop = DEFAULT_CLOCKSTOP;
	IccParameters[portno].Ifsc = DEFAULT_IFSC;
	IccParameters[portno].Nad = DEFAULT_NAD;

	return SLOT_NO_ERROR;
}



/**
  * @brief  After got ATR and take ATR information to apply CCID protocol data structure 
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval SLOT_NO_ERROR
  */
unsigned char Ifd_ApplyParametersStructure( int32_t portno )
{
	S_SC_DEV_T *dev;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	IccParameters[portno].FiDi = (dev->CardCapabilities.Fl<<4) | dev->CardCapabilities.Dl;		
	IccParameters[portno].ClockStop = dev->clock_stop_level;
	//IccParameters[portno].Nad;				/* just for protocol T=1 */
	
	if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T0)
    {
		SetT0_bTransactionType;
		/* TCCKST */
		if( dev->CardCapabilities.InversConvention == true)
			IccParameters[portno].Tcckst = 0x02;
		/* GuardTime */
		if(dev->CardCapabilities.GT==11)
			IccParameters[portno].GuardTime = 0x00;
		else
			IccParameters[portno].GuardTime = dev->CardCapabilities.GT - 12;
		/* WaitingInteger */
		IccParameters[portno].WaitingInteger = dev->CardCapabilities.T0.WI;
    }
    else if(dev->CardCapabilities.Protocol.Selected == SCARD_PROTOCOL_T1)
    {
    	SetT1_bTransactionType;
		/* TCCKST */
		IccParameters[portno].Tcckst = 0x10;
		if( dev->CardCapabilities.InversConvention == true)
			IccParameters[portno].Tcckst |= 0x02;
		if( dev->CardCapabilities.T1.EDC == 0x01)		// EDC = CRC scheme
			IccParameters[portno].Tcckst |= 0x01;
		/* GuardTime */
		if(dev->CardCapabilities.GT==11)
			IccParameters[portno].GuardTime = 0xFF;
		else
			IccParameters[portno].GuardTime = dev->CardCapabilities.GT - 12;
		/* WaitingInteger */
		IccParameters[portno].WaitingInteger = (dev->CardCapabilities.T1.BWI<<4) | dev->CardCapabilities.T1.CWI;
		/* Ifsc */
		IccParameters[portno].Ifsc = dev->CardCapabilities.T1.IFSC;	
    }

	//XfrWaitingTime = ( 960 / 64 ) * D * ((unsigned long) IccParameters[portno].WaitingInteger);
	//CRD_SetWaitingTime( XfrWaitingTime );

	return SLOT_NO_ERROR;
}


/**
  * @brief  Check if hardware is busy or in other error condition
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval Slot Status Error Code
  */
unsigned char Ifd_GetHwError( int32_t portno )
{
	unsigned char ErrorCode;
	S_SC_DEV_T *dev;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	ErrorCode = SLOT_NO_ERROR;

	// About HW_ERROR, we have no idea to know the situation of volotage error or current error
	// TODO:  ...

	if(dev->op_state == SC_OP_ATR_READ)
		ErrorCode = SLOTERR_BUSY_WITH_AUTO_SEQUENCE;
	
	//if(dev->op_state)
	//	ErrorCode = SLOTERR_CMD_SLOT_BUSY;

	if(dev->errno == SC_ERR_CARD_REMOVED)
		ErrorCode = SLOTERR_ICC_MUTE;

	if(ErrorCode != SLOT_NO_ERROR)
		SMARTCARD_DeactivationCmd(dev);		/* Switch off power card */

	return ErrorCode;
}


/**
  * @brief  Do cold-reset or warm-reset and return ATR information
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  Voltage			Class Level (AUTO, Class A, B, C)
  * @param  pReceiveBuffer	Pointer uses to fill ATR information
  * @param  pAtrSize		The length of ATR information
  * @retval Slot Status Error Code
  */
unsigned char Ifd_IccPowerOn(	int32_t portno,
										unsigned char Voltage,
										unsigned char *pReceiveBuffer,
										unsigned int *pAtrSize	)
{
	int32_t ErrorCode;
	S_SC_DEV_T *dev;
	SC_TypeDef *SC;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);
	SC = dev->base;

	ErrorCode = Ifd_Init( portno );
	//Ifd_ApplyParametersStructure();
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;


	if((Voltage == SC_ISO_OPERATIONCLASS_AUTO) )/*&& (SMARTCARD_CheckCardPresent(portno) == SCARD_ABSENT))*/
	{
		if(Ifd_GetSlotStatus( portno ) == 0x01)	// Do cold-reset
		{
			dev->voltage = SC_ISO_OPERATIONCLASS_C;
			ErrorCode = SMARTCARD_ColdReset(dev);
			if(ErrorCode != SC_STATUS_SUCCESS)
			{
				SMARTCARD_DeactivationCmd(dev);
				dev->voltage = SC_ISO_OPERATIONCLASS_B;
				ErrorCode = SMARTCARD_ColdReset(dev);
				if(ErrorCode != SC_STATUS_SUCCESS)
				{
					SMARTCARD_DeactivationCmd(dev);
					dev->voltage = SC_ISO_OPERATIONCLASS_A;
					ErrorCode = SMARTCARD_ColdReset(dev);
				}
			}
			if(ErrorCode == ATR_ERR_INVALID_PARAMETER)
				ErrorCode = SMARTCARD_WarmReset(dev);
			
		} 
		else if(Ifd_GetSlotStatus( portno ) == 0x00)	// Do warm-reset
		{
			ErrorCode = SMARTCARD_WarmReset(dev);	
		}
		else if(Ifd_GetSlotStatus( portno ) == 0x02)	// card removed 
		{
			ErrorCode = SC_ERR_CARD_REMOVED;				
		}		
	}
	// assign voltage
	else if( (Voltage == SC_ISO_OPERATIONCLASS_C) || (Voltage == SC_ISO_OPERATIONCLASS_B) || (Voltage == SC_ISO_OPERATIONCLASS_A))
	{
		if(Ifd_GetSlotStatus( portno ) == 0x01)	// Do cold-reset
		{
			dev->voltage = Voltage;
			ErrorCode = SMARTCARD_ColdReset(dev);
		} 
		else if(Ifd_GetSlotStatus( portno ) == 0x00)	// Do warm-reset
		{
			ErrorCode = SMARTCARD_WarmReset(dev);	
		}		
	}	
	else
	{	
		if(SMARTCARD_CheckCardPresent(SC) == SCARD_ABSENT)
		{
			dev->voltage = Voltage;
			ErrorCode = SMARTCARD_ColdReset(dev);	
		} 
		else 
		{
			dev->voltage = Voltage;
			SMARTCARD_VoltageConfig(dev);
			ErrorCode = SMARTCARD_WarmReset(dev);	
		}
	}

	if(ErrorCode != SC_STATUS_SUCCESS)
		return Ifd_SC2CCIDErrorCode(ErrorCode);
	
	// Get the ATR information
	*pAtrSize = SMARTCARD_GetATRBuffer(SC, pReceiveBuffer, 32);

	ErrorCode = Ifd_ApplyParametersStructure( portno );
	
	g_ifs_req_flag = 1;
	
	return SLOT_NO_ERROR;
}

uint8_t Ifd_CheckCmdCase(uint8_t *buf, uint32_t len)
{
	uint8_t lc=0x00;
	
	if (len <= 5) 
	{
		if (len == 4)		
		{
			/* Case1 */ 
			return CMD_CASE1;
		}		
		else	 
		{
			/* Case2 */ 
			return CMD_CASE2;
		}
	}
	else 
	{		
		lc = buf[4];

		// 5 = CLA+INS+P1+P2+Lc
		if (lc != len - 5) // this situation has two results: case3 failed or apply case4
		{
			if((len-5-lc) == 1)
			{
				/* Case4 */
				return CMD_CASE4;
			}
		}

		/* Case3 */
		return CMD_CASE3;
	}
}


/**
  * @brief  According to hardware and ICC, those conditions decide the transmission protocol.
  *			There are three exchange levels as Character, APDU and TPDU.
  * @param  portno				Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer		Command Data
  * @param  pBlockSize			The size of command data
  * @param  ExpectedAnswerSize	Expected size of received data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrBlock(	int32_t portno,
									unsigned char *pBlockBuffer,
									unsigned int *pBlockSize,
									unsigned int ExpectedAnswerSize	)
{
	unsigned char ErrorCode = SLOT_NO_ERROR;

	//if( bmTransactionLevel != CHARACTER_LEVEL && ExpectedAnswerSize != 0x00 )
	//	return XFR_BADLEVELPARAMETER;

	g_ChainParameter = 0x00;

	if( (bmTransactionLevel == CHARACTER_LEVEL) && (bTransactionType == T0_TYPE) )
		ErrorCode = Ifd_XfrCharT0(portno, pBlockBuffer, pBlockSize, ExpectedAnswerSize);

	if( (bmTransactionLevel == CHARACTER_LEVEL) && (bTransactionType == T1_TYPE) )
		ErrorCode = Ifd_XfrCharT1(portno, pBlockBuffer, pBlockSize, ExpectedAnswerSize);

	if( (bmTransactionLevel == TPDU_LEVEL) && (bTransactionType == T0_TYPE) )
		ErrorCode = Ifd_XfrTpduT0(portno, pBlockBuffer, pBlockSize);

	if( (bmTransactionLevel == TPDU_LEVEL) && (bTransactionType == T1_TYPE) )
		ErrorCode = Ifd_XfrTpduT1(portno, pBlockBuffer, pBlockSize);

	if(UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0)				// Short APDU
	{
		if( ((bmTransactionLevel==SHORTAPDU_LEVEL) || (bmTransactionLevel==SHORTEXTENDAPDU_LEVEL)) && (bTransactionType == T0_TYPE) )
			ErrorCode = Ifd_XfrShortApduT0(portno, pBlockBuffer, pBlockSize);

		if( ((bmTransactionLevel==SHORTAPDU_LEVEL) || (bmTransactionLevel==SHORTEXTENDAPDU_LEVEL)) && (bTransactionType == T1_TYPE) )
			ErrorCode = Ifd_XfrShortApduT1(portno, pBlockBuffer, pBlockSize);
	} 
	else if(UsbMessageBuffer[OFFSET_WLEVELPARAMETER] != 0)		// Extended APDU
	{
		/*
		 * 01h: command APDU begins with this command , and continue in the next PC_to_RDR_XfrBlock
		 * 02h: abData field continues a command AODU and ends the APDU command
		 * 03h: abData field continues a command APDU and another block is to follow
		 * 10h: empty abData field, continuation of response APDU is expected in the next RDR_to_PC_DataBlock
		 */
		if( (bmTransactionLevel == SHORTEXTENDAPDU_LEVEL) && (bTransactionType == T0_TYPE) )
			ErrorCode = Ifd_XfrExtendApduT0(portno, pBlockBuffer, pBlockSize);

		if( (bmTransactionLevel == SHORTEXTENDAPDU_LEVEL) && (bTransactionType == T1_TYPE) )
			ErrorCode = Ifd_XfrExtendApduT1(portno, pBlockBuffer, pBlockSize);
	}

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	return ErrorCode;
}




/**
  * @brief  Transmission by T=0 Character Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @param  AnswerSize		Expected size of received data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrCharT0(	int32_t portno,
									unsigned char *pBlockBuffer,
									unsigned int *pBlockSize,
									unsigned int AnswerSize )
{
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t idx, rlen;
	S_SC_DEV_T *dev;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);
	
	ErrorCode = CBRawTransmit( dev, pBlockBuffer, *pBlockSize, AnswerSize, &rbuf[0], &rlen);
	if(ErrorCode != SC_STATUS_SUCCESS)
		return Ifd_SC2CCIDErrorCode(ErrorCode);
	
	// received data
	for(idx=0; idx<rlen; idx++) 
	{
		*pBlockBuffer = rbuf[idx];
		pBlockBuffer++;
	}
	// length of received data
	*pBlockSize = rlen;

	return SLOT_NO_ERROR;
}


/**
  * @brief  Transmission by T=1 Character Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @param  AnswerSize		Expected size of received data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrCharT1(	int32_t portno,
									unsigned char *pBlockBuffer,
									unsigned int *pBlockSize,
									unsigned int AnswerSize	)
{
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t idx, rlen;
	S_SC_DEV_T *dev;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	ErrorCode = CBRawTransmit( dev, pBlockBuffer, *pBlockSize, AnswerSize, &rbuf[0], &rlen);
	if(ErrorCode != SC_STATUS_SUCCESS)
		return Ifd_SC2CCIDErrorCode(ErrorCode);
	// received data
	for(idx=0; idx<rlen; idx++) 
	{
		*pBlockBuffer = rbuf[idx];
		pBlockBuffer++;
	}
	// length of received data
	*pBlockSize = rlen;

	return SLOT_NO_ERROR;

}


/**
  * @brief  Transmission by T=0 TPDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrTpduT0(int32_t portno,
									unsigned char *pBlockBuffer,
									unsigned int *pBlockSize	)
{
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t idx, AnswerSize, rlen;
	S_SC_DEV_T *dev;	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	if( *pBlockBuffer == 0xFF )		/* PPS Exchange Process */
	{
		AnswerSize = 4;
		ErrorCode = CBRawTransmit( dev, pBlockBuffer, *pBlockSize, AnswerSize, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		
		// received data
		for(idx=0; idx<rlen; idx++) 
		{
			*pBlockBuffer = rbuf[idx];
			pBlockBuffer++;
		}
		// length of received data
		*pBlockSize = rlen;
	}
	else
	{		
		ErrorCode = CBT0Transmit( dev, pBlockBuffer, *pBlockSize, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		
		// received data
		for(idx=0; idx<rlen; idx++) 
		{
			*pBlockBuffer = rbuf[idx];
			pBlockBuffer++;
		}
		// length of received data
		*pBlockSize = rlen;
	}

	return SLOT_NO_ERROR;

}


/**
  * @brief  Transmission by T=1 TPDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrTpduT1(	int32_t portno,
										unsigned char *pBlockBuffer,
										unsigned int *pBlockSize	)
{
	int32_t ErrorCode, origBwt=0;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t idx, XfrWaitingTime = 0, rlen;
	S_SC_DEV_T *dev;	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	// if not equal to zero, change BWT according to BWI
	if(UsbMessageBuffer[OFFSET_BBWI] != 0)	
	{
		origBwt = dev->CardCapabilities.T1.BWT; 
		dev->CardCapabilities.T1.BWT *= UsbMessageBuffer[OFFSET_BBWI];    // how to integrate into BWT??????
	}

	// Sending procedure
	ErrorCode = CBRawTransmit( dev, pBlockBuffer, *pBlockSize, 0, &rbuf[0], &rlen);
	if(ErrorCode != SC_STATUS_SUCCESS)
		return Ifd_SC2CCIDErrorCode(ErrorCode);
	
	// Receiving procedure
	/* [EMV 2000] */
	XfrWaitingTime = (dev->CardCapabilities.T1.BWT / dev->CardCapabilities.etu) + (BitRateAdjustment[dev->CardCapabilities.Dl].DNumerator * 4700) + 1;

	SMARTCARD_TimerCountSet(dev, SC_TIMER0, SC_TMR_MODE_DC_RELOAD_S_S, XfrWaitingTime);
	SMARTCARD_TimerStart(dev, SC_TIMER0);

    dev->errno = PROTOCOL_T1_OK;
	dev->rcv_buf = pBlockBuffer;
	dev->rcv_len = 3;	// Prologue field length
	dev->rcv_cnt = 0;
   	dev->rcv_pos = 0;
	dev->op_state = SC_OP_READ;
	SMARTCARD_WaitFirstReceivedData(dev, SC_TIMER2, SC_TMR_MODE_DC, XfrWaitingTime);
	
	while(dev->op_state == SC_OP_READ && !(dev->errno)) ;
	if(dev->errno < 0)
		return Ifd_SC2CCIDErrorCode(dev->errno);

	dev->rcv_len = dev->rcv_buf[2] + (dev->CardCapabilities.T1.EDC?2:1);
	dev->rcv_cnt = 0;
   	dev->rcv_pos = 3;
	dev->op_state = SC_OP_READ;
	while(dev->op_state == SC_OP_READ && !(dev->errno)) ;
	if(dev->errno < 0)
		return Ifd_SC2CCIDErrorCode(dev->errno);

	SMARTCARD_TimerStop(dev, SC_TIMER0);	// stop timer

	// restore the original BWT value
	if(UsbMessageBuffer[OFFSET_BBWI] != 0)
		dev->CardCapabilities.T1.BWT = origBwt;
	
	// received data
	for(idx=0; idx<(dev->rcv_buf[2]+3+(dev->CardCapabilities.T1.EDC?2:1)); idx++) 
	{
		CCIDSCDEBUG("Received Data: Data[%d]=0x%02x \n", idx, dev->rcv_buf[idx]);
		*pBlockBuffer = dev->rcv_buf[idx];
		pBlockBuffer++;
	}
	// length of received data
	*pBlockSize = dev->rcv_buf[2]+3+(dev->CardCapabilities.T1.EDC?2:1);

	return SLOT_NO_ERROR;

}


/**
  * @brief  Transmission by T=0 Short APDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrShortApduT0(	int32_t portno,
											unsigned char *pBlockBuffer,
											unsigned int *pBlockSize	)
{
	//uint8_t case_no=0x00;
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint8_t rbufICC[MIN_BUFFER_SIZE];
	uint8_t *buf = rbuf;
	uint32_t idx, rlen, rlenICC;
	S_SC_DEV_T *dev;	
	unsigned char blockbuf[5];	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	//case_no = Ifd_CheckCmdCase(pBlockBuffer, *pBlockSize);

	CCIDSCDEBUG("Ifd_XfrShortApduT0: header=%02x %02x %02x %02x, len=%d\n",
		pBlockBuffer[0], pBlockBuffer[1], pBlockBuffer[2], pBlockBuffer[3], *pBlockSize);

	if( *pBlockSize == 0x4 )
	{
		blockbuf[0] = pBlockBuffer[0];
		blockbuf[1] = pBlockBuffer[1];
		blockbuf[2] = pBlockBuffer[2];
		blockbuf[3] = pBlockBuffer[3];
		blockbuf[4] = 0x00;
		
		ErrorCode = CBT0Transmit( dev, &blockbuf[0], 0x5, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		
		// received data
		for(idx=0; idx<rlen; idx++) 
		{
			*pBlockBuffer = rbuf[idx];
			pBlockBuffer++;
		}
		// length of received data
		*pBlockSize = rlen;
	}
	else 
	{
		ErrorCode = CBT0Transmit( dev, pBlockBuffer, *pBlockSize, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		
		// check if wrong Le field error
		while((rlen==2) && (rbuf[0]==0x6C))
		{
			pBlockBuffer[4] = rbuf[1];
			ErrorCode = CBT0Transmit( dev, pBlockBuffer, *pBlockSize, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);
		}

		// check if data bytes still available
		if((rlen==2) && (rbuf[0]==0x61))
		{
			rlenICC = 0;
			while(1)
			{
				blockbuf[0] = USB_ConfigDescriptor[48+18];	// CLA = bClassGetResponse field
				blockbuf[1] = 0xC0;						// 0xC0 == Get response command
				blockbuf[2] = 0x00;						// 0x00
				blockbuf[3] = 0x00;						// 0x00
				blockbuf[4] = rbuf[rlen-1];			// Licc = how many data bytes still available
				ErrorCode = CBT0Transmit( dev, blockbuf, 0x5, &rbuf[0], &rlen);
				if(ErrorCode != SC_STATUS_SUCCESS)
					return Ifd_SC2CCIDErrorCode(ErrorCode);
				// received data
				if(rbuf[rlen-2]==0x61)
				{
					for(idx=0; idx<rlen-2; idx++) 
						rbufICC[rlenICC++] = rbuf[idx];
				} 
				else
				{
					for(idx=0; idx<rlen; idx++) 
						rbufICC[rlenICC++] = rbuf[idx];
				}
					
				if(rbuf[rlen-2] != 0x61) break;
			}
			rlen = rlenICC;
			buf = rbufICC;
		}
	

		CCIDSCDEBUG("Ifd_XfrShortApduT0: dwLength = %d, Data = ", rlen);

		/* Check status bytes */
		if((buf[rlen-2]&0xF0)!=0x60 && (buf[rlen-2]&0xF0)!=0x90)
			return SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED;

		// received data
		for(idx=0; idx<rlen; idx++) 
		{
			CCIDSCDEBUG("%02x", buf[idx]);
			*pBlockBuffer = buf[idx];
			pBlockBuffer++;
		}
		CCIDSCDEBUG("\n");
			
		// length of received data
		*pBlockSize = rlen;
	}

	return SLOT_NO_ERROR;

}


/**
  * @brief  Transmission by T=1 Short APDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrShortApduT1(	int32_t portno,
											unsigned char *pBlockBuffer,
											unsigned int *pBlockSize	)
{
	int32_t retransmit = 0;
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE], rsp_type;
	uint32_t idx, rlen;
	S_SC_DEV_T *dev;	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);
	
	if(portno == 0) {
    	dev->rcv_buf = (uint8_t *)&g_Port0_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
    	dev->snd_buf = (uint8_t *)&g_Port0_TxBUF;
	} else {
		dev->rcv_buf = (uint8_t *)&g_Port1_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
		dev->snd_buf = (uint8_t *)&g_Port1_TxBUF;
	}

	CCIDSCDEBUG("Ifd_XfrShortApduT1: header=%02x %02x %02x %02x, len=%d\n",  
		pBlockBuffer[0], pBlockBuffer[1], pBlockBuffer[2], pBlockBuffer[3], *pBlockSize);

	/* IFS request only for EMV T=1 */
	/* First block (S-block IFS request) transmits after ATR */
	if(g_ifs_req_flag==1)
	{
		retransmit++;
		g_ifs_req_flag = 0;
		ErrorCode = SendSBlock( dev, T1_BLOCK_S_IFS_REQ, 0xFE);
		rsp_type = T1BlockGetType(dev->rcv_buf);
		if(rsp_type==T1_BLOCK_S)
		{
			/* check IFS RESPONSE */
			if(dev->rcv_buf[1]!=T1_BLOCK_S_IFS_RES) 
				ErrorCode = PROTOCOL_T1_ERROR;
			/* IFSC shall have a value in the range '10' to 'FE' && check NAD && response's inf data must be 0xFE */
			if ( T1BlockGetLength(dev->rcv_buf)!=0x01 || T1BlockGetNAD(dev->rcv_buf)!=0x00 ||
				dev->rcv_buf[3]!=0xFE)
				ErrorCode = PROTOCOL_T1_ERROR;
		}
		while( (ErrorCode!=SC_STATUS_SUCCESS || rsp_type!=T1_BLOCK_S) && retransmit!=3 )
		{
			ErrorCode = SendSBlock( dev, T1_BLOCK_S_IFS_REQ, 0xFE);
			rsp_type = T1BlockGetType(dev->rcv_buf);
			if(rsp_type==T1_BLOCK_S)
			{
				/* check IFS RESPONSE */
				if(dev->rcv_buf[1]!=T1_BLOCK_S_IFS_RES) 
					ErrorCode = PROTOCOL_T1_ERROR;
				/* IFSC shall have a value in the range '10' to 'FE' && check NAD && response's inf data must be 0xFE */
				if ( T1BlockGetLength(dev->rcv_buf)!=0x01 || T1BlockGetNAD(dev->rcv_buf)!=0x00 ||
					dev->rcv_buf[3]!=0xFE)
					ErrorCode = PROTOCOL_T1_ERROR;
			}
			retransmit++;
		}
		
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
	}

	//for(idx=0; idx<*pBlockSize; idx++) 	// skip checksum byte
	//	CCIDSCDEBUG("Sending Data: Data[%d]=0x%02x \n", idx, pBlockBuffer[idx]);

	// Sending procedure
	ErrorCode = CBT1Transmit( dev, pBlockBuffer, *pBlockSize, &rbuf[0], &rlen);
	if(ErrorCode != SC_STATUS_SUCCESS)
		return Ifd_SC2CCIDErrorCode(ErrorCode);

	CCIDSCDEBUG("Ifd_XfrShortApduT1: dwLength = %d, Data = ", rlen);
	
	// received data
	for(idx=0; idx<rlen; idx++)
	{
		//CCIDSCDEBUG("Received Data: Data[%d]=0x%02x \n", idx, rbuf[idx]);
		CCIDSCDEBUG("%02x", rbuf[idx]);
		*pBlockBuffer = rbuf[idx];
		pBlockBuffer++;
	}
	CCIDSCDEBUG("\n");
			
	// length of received data
	*pBlockSize = rlen;	

	return SLOT_NO_ERROR;
	
}


/**
  * @brief  Transmission by T=0 Extend APDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrExtendApduT0(	int32_t portno,
												unsigned char *pBlockBuffer,
												unsigned int *pBlockSize	)
{
	int32_t ErrorCode, idx;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t datalength, rlen;
	S_SC_DEV_T *dev;
	unsigned char getresponsebuf[5];
	unsigned char envelopebuf[300];	// 255 bytes + 5 command data
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);


	if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x01)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;

		while(datalength)
		{
			envelopebuf[0] = USB_ConfigDescriptor[49+18];		// CLA = bClassEnvelope field
			envelopebuf[1] = 0xC2;							// 0xC2 = Envelope command
			envelopebuf[2] = 0x00;							// 0x00
			envelopebuf[3] = 0x00;							// 0x00
			if(datalength >= 0xFF)
				envelopebuf[4] = 0xFF;						// APDU length > buffer length
			else
				envelopebuf[4] = datalength;					// APDU length > buffer length

			if(datalength >= 0xFF) {
				for(idx=0; idx < 0xFF; idx++)
					envelopebuf[idx + 5] = pBlockBuffer[idx];
			} else {
				for(idx=0; idx < datalength; idx++)
					envelopebuf[idx + 5] = pBlockBuffer[idx];
			}

			if(datalength >= 0xFF)
				ErrorCode = CBT0Transmit( dev, &envelopebuf[0], 0x5+0xFF, &rbuf[0], &rlen);
			else
				ErrorCode = CBT0Transmit( dev, &envelopebuf[0], 0x5+datalength, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);

			if(datalength >= 0xFF) {
				pBlockBuffer+=0xFF;
				datalength-=0xFF;
			} else
				datalength-=datalength;

		}
		
		// length of received data
		*pBlockSize = 0;

		// Chain Parameter Setting
		g_ChainParameter = 0x10;
	}
	
	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x02)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;
		
		envelopebuf[0] = USB_ConfigDescriptor[49+18];		// CLA = bClassEnvelope field
		envelopebuf[1] = 0xC2;							// 0xC2 = Envelope command
		envelopebuf[2] = 0x00;							// 0x00
		envelopebuf[3] = 0x00;							// 0x00
		envelopebuf[4] = datalength;						// last part of APDU command
		for(idx=0; idx < datalength; idx++)
			envelopebuf[idx + 5] = pBlockBuffer[idx];
		
		ErrorCode = CBT0Transmit( dev, &envelopebuf[0], 0x5+envelopebuf[4], &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);

		// get response command
		getresponsebuf[0] = USB_ConfigDescriptor[48+18];	// CLA = bClassGetResponse field
		getresponsebuf[1] = 0xC0;							// 0xC0 = Get response command
		getresponsebuf[2] = 0x00;							// 0x00
		getresponsebuf[3] = 0x00;							// 0x00
		getresponsebuf[4] = 0x00;							// 0x00
		
		ErrorCode = CBT0Transmit( dev, &getresponsebuf[0], 0x5, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		// check if data bytes still available, max. size of buffer is 500 bytes
		//while(1)
		//{
			if((rlen==2) && (rbuf[0]==0x61))
			{
				getresponsebuf[0] = USB_ConfigDescriptor[48+18];	// CLA = bClassGetResponse field
				getresponsebuf[1] = 0xC0;							// 0xC0 == Get response command
				getresponsebuf[2] = 0x00;							// 0x00
				getresponsebuf[3] = 0x00;							// 0x00
				getresponsebuf[4] = rbuf[1];			// Licc = how many data bytes still available
				if(getresponsebuf[4]==0xFF) // TODO: care this point. NOT sure how to handle this
					ErrorCode = CBRawTransmit( dev, &getresponsebuf[0], 0x5, 0xFF, &rbuf[0], &rlen);
				else {
					ErrorCode = CBT0Transmit( dev, &getresponsebuf[0], 0x5, &rbuf[0], &rlen);
					printf("why available data is less than 0xff \n");
				}
				if(ErrorCode != SC_STATUS_SUCCESS)
					return Ifd_SC2CCIDErrorCode(ErrorCode);
			}
			
		//}

		// store received data
		pBlockBuffer = pBlockBuffer - 7;
		for(idx=0; idx<rlen; idx++)
			pBlockBuffer[idx] = rbuf[idx];
		
		// length of received data
		*pBlockSize = getresponsebuf[4];

		// Chain Parameter Setting
		g_ChainParameter = 0x01;
	}
	
	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x03)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;

		while(datalength)
		{
			envelopebuf[0] = USB_ConfigDescriptor[49+18];		// CLA = bClassEnvelope field
			envelopebuf[1] = 0xC2;							// 0xC2 = Envelope command
			envelopebuf[2] = 0x00;							// 0x00
			envelopebuf[3] = 0x00;							// 0x00
			if(datalength >= 0xFF)
				envelopebuf[4] = 0xFF;						// APDU length > buffer length
			else
				envelopebuf[4] = datalength;					// APDU length > buffer length

			if(datalength >= 0xFF) {
				for(idx=0; idx < 0xFF; idx++)
					envelopebuf[idx + 5] = pBlockBuffer[idx];
			} else {
				for(idx=0; idx < datalength; idx++)
					envelopebuf[idx + 5] = pBlockBuffer[idx];
			}

			if(datalength >= 0xFF)
				ErrorCode = CBT0Transmit( dev, &envelopebuf[0], 0x5+0xFF, &rbuf[0], &rlen);
			else
				ErrorCode = CBT0Transmit( dev, &envelopebuf[0], 0x5+datalength, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);

			if(datalength >= 0xFF) {
				pBlockBuffer+=0xFF;
				datalength-=0xFF;
			} else
				datalength-=datalength;

		}
		
		// length of received data
		*pBlockSize = 0;

		// Chain Parameter Setting
		g_ChainParameter = 0x10;
	}

	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x10)
	{
		// get response command
		getresponsebuf[0] = USB_ConfigDescriptor[48+18];	// CLA = bClassGetResponse field
		getresponsebuf[1] = 0xC0;							// 0xC0 = Get response command
		getresponsebuf[2] = 0x00;							// 0x00
		getresponsebuf[3] = 0x00;							// 0x00
		getresponsebuf[4] = 0x00;							// 0x00
		
		ErrorCode = CBT0Transmit( dev, &getresponsebuf[0], 0x5, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		// check if data bytes still available, max. size of buffer is 500 bytes
		if((rlen==2) && (rbuf[0]==0x61))
		{
			getresponsebuf[0] = USB_ConfigDescriptor[48+18];	// CLA = bClassGetResponse field
			getresponsebuf[1] = 0xC0;							// 0xC0 == Get response command
			getresponsebuf[2] = 0x00;							// 0x00
			getresponsebuf[3] = 0x00;							// 0x00
			getresponsebuf[4] = rbuf[1];			// Licc = how many data bytes still available
			if(getresponsebuf[4]==0xFF)
				ErrorCode = CBRawTransmit( dev, &getresponsebuf[0], 0x5, 0xFF, &rbuf[0], &rlen);
			else
				ErrorCode = CBT0Transmit( dev, &getresponsebuf[0], 0x5, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);
		}		

		// store received data
		for(idx=0; idx<rlen; idx++)
			pBlockBuffer[idx] = rbuf[idx];
		
		// length of received data
		if(getresponsebuf[4]==0xFF)
			*pBlockSize = getresponsebuf[4];
		else
			*pBlockSize = rlen;

		// Chain Parameter Setting
		if(getresponsebuf[4]==0xFF)
			g_ChainParameter = 0x03;
		else
			g_ChainParameter = 0x02;
	}

	return SLOT_NO_ERROR;

}


/**
  * @brief  Transmission by T=1 Extend APDU Mode
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	Command Data
  * @param  pBlockSize		The size of command data
  * @retval Slot Status Error Code
  */
unsigned char Ifd_XfrExtendApduT1(	int32_t portno,
												unsigned char *pBlockBuffer,
												unsigned int *pBlockSize	)
{
	int32_t ErrorCode;
	uint8_t rbuf[MIN_BUFFER_SIZE];
	uint32_t idx, rlen;
	uint32_t datalength;
	S_SC_DEV_T *dev;	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);

	if(portno == 0) {
    	dev->rcv_buf = (uint8_t *)&g_Port0_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
    	dev->snd_buf = (uint8_t *)&g_Port0_TxBUF;
	} else {
		dev->rcv_buf = (uint8_t *)&g_Port1_RxBUF;//(uint8_t *)malloc(MAX_BUF_LEN * sizeof(uint8_t));
		dev->snd_buf = (uint8_t *)&g_Port1_TxBUF;
	}

	if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x01)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;

		while(datalength)
		{

			if(datalength >= 0xFE)
				ErrorCode = CBT1Transmit( dev, pBlockBuffer, 0xFE, &rbuf[0], &rlen);
			else
				ErrorCode = CBT1Transmit( dev, pBlockBuffer, datalength, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);

			if(datalength >= 0xFE) {
				pBlockBuffer+=0xFE;
				datalength-=0xFE;
			} else
				datalength-=datalength;

		}
		
		// length of received data
		*pBlockSize = 0;

		// Chain Parameter Setting
		g_ChainParameter = 0x10;
	}
	
	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x02)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;
		
		ErrorCode = CBT1Transmit( dev, pBlockBuffer, datalength, &rbuf[0], &rlen);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);
		
		// check if data bytes still available
		//datalength = rlen - 3 - (dev->CardCapabilities.T1.EDC?2:1);
		pBlockBuffer = pBlockBuffer -7;
		for(idx=0; idx<rlen; idx++)
			pBlockBuffer[idx] = rbuf[idx];
		
		// length of received data
		*pBlockSize = rlen;

		// Chain Parameter Setting
		g_ChainParameter = 0x01;
	}
	
	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x03)
	{
		datalength = (*pBlockSize) - 7;
		pBlockBuffer = pBlockBuffer + 7;

		while(datalength)
		{

			if(datalength >= 0xFE)
				ErrorCode = CBT1Transmit( dev, pBlockBuffer, 0xFE, &rbuf[0], &rlen);
			else
				ErrorCode = CBT1Transmit( dev, pBlockBuffer, datalength, &rbuf[0], &rlen);
			if(ErrorCode != SC_STATUS_SUCCESS)
				return Ifd_SC2CCIDErrorCode(ErrorCode);

			if(datalength >= 0xFE) {
				pBlockBuffer+=0xFE;
				datalength-=0xFE;
			} else
				datalength-=datalength;

		}
		
		// length of received data
		*pBlockSize = 0;

		// Chain Parameter Setting
		g_ChainParameter = 0x10;
	}

	else if( UsbMessageBuffer[OFFSET_WLEVELPARAMETER] == 0x10)
	{
		ErrorCode = SendRBlock( dev, 1, 0);
		if(ErrorCode != SC_STATUS_SUCCESS)
			return Ifd_SC2CCIDErrorCode(ErrorCode);

		datalength = dev->rcv_pos - 3 - (dev->CardCapabilities.T1.EDC?2:1);
		pBlockBuffer = pBlockBuffer -7;
		for(idx=0; idx<datalength; idx++)
			pBlockBuffer[idx] = dev->rcv_buf[3+idx];
		
		// length of received data
		if(dev->rcv_pos==0xFE+3)
			*pBlockSize = 0xFE;
		else
			*pBlockSize = dev->rcv_pos - 3;

		// Chain Parameter Setting
		if(dev->rcv_pos==0xFE)
			g_ChainParameter = 0x03;
		else
			g_ChainParameter = 0x02;
	}

	return SLOT_NO_ERROR;

}


/**
  * @brief  Give slot's protocol data structure
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pParamBuffer	Fill the protocol data structure continuously
  * @retval 0x00 for T=0, 0x01 for T=1
  */
unsigned char Ifd_GetParameters(int32_t portno, unsigned char *pParamBuffer)
{

	CCIDSCDEBUG("\n**************Ifd_GetParameters***************\n");
	CCIDSCDEBUG("FiDi = 0x%x \n", IccParameters[portno].FiDi);
	CCIDSCDEBUG("Tcckst = 0x%x \n", IccParameters[portno].Tcckst);
	CCIDSCDEBUG("GuardTime = 0x%x \n", IccParameters[portno].GuardTime);
	CCIDSCDEBUG("WaitingInteger = 0x%x \n", IccParameters[portno].WaitingInteger);
	CCIDSCDEBUG("ClockStop = 0x%x \n", IccParameters[portno].ClockStop);
	CCIDSCDEBUG("Ifsc = 0x%x \n", IccParameters[portno].Ifsc);
	CCIDSCDEBUG("Nad = 0x%x \n", IccParameters[portno].Nad);

	*pParamBuffer = IccParameters[portno].FiDi;
	*(pParamBuffer+1) = IccParameters[portno].Tcckst;
	*(pParamBuffer+2) = IccParameters[portno].GuardTime;
	*(pParamBuffer+3) = IccParameters[portno].WaitingInteger;
	*(pParamBuffer+4) = IccParameters[portno].ClockStop;
	*(pParamBuffer+5) = IccParameters[portno].Ifsc;
	*(pParamBuffer+6) = IccParameters[portno].Nad;

	if(IccParameters[portno].Tcckst & 0x10)
		return 0x01;

	return 0x00;
}


/**
  * @brief  Set slot's protocol data structure and apply to hardware setting
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pParamBuffer	Protocol data structure
  * @param  T01				Specify if data structure is T=0 or T=1 type
  * @retval Slot Status Error Code
  */
unsigned char Ifd_SetParameters(	int32_t portno,
											unsigned char *pParamBuffer,
											unsigned char T01)
{
	Param NewIccParameters;
	S_SC_DEV_T *dev;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);


	CCIDSCDEBUG("\n**************Ifd_SetParameters***************\n");
	CCIDSCDEBUG("FiDi = 0x%x \n", *pParamBuffer);
	CCIDSCDEBUG("Tcckst = 0x%x \n", *(pParamBuffer+1));
	CCIDSCDEBUG("GuardTime = 0x%x \n", *(pParamBuffer+2));
	CCIDSCDEBUG("WaitingInteger = 0x%x \n", *(pParamBuffer+3));
	CCIDSCDEBUG("ClockStop = 0x%x \n", *(pParamBuffer+4));
	CCIDSCDEBUG("Ifsc = 0x%x \n", *(pParamBuffer+5));
	CCIDSCDEBUG("Nad = 0x%x \n", *(pParamBuffer+6));


	NewIccParameters.FiDi = *pParamBuffer;
	NewIccParameters.Tcckst = *(pParamBuffer+1);
	NewIccParameters.GuardTime = *(pParamBuffer+2);
	NewIccParameters.WaitingInteger = *(pParamBuffer+3);
	NewIccParameters.ClockStop = *(pParamBuffer+4);
	if(T01 == 0x01)
	{
		NewIccParameters.Ifsc = *(pParamBuffer+5);
		NewIccParameters.Nad = *(pParamBuffer+6);
	}
	else
	{
		NewIccParameters.Ifsc = 0x00;
		NewIccParameters.Nad = 0x00;
	}

	if( (ClockRateConversion[(NewIccParameters.FiDi && 0x0F)].F == 0) 
			|| (BitRateAdjustment[(NewIccParameters.FiDi>>4)].DNumerator== 0) )
		return SLOTERR_BAD_FIDI;

	if( (T01 == 0x00)
			&& (NewIccParameters.Tcckst != 0x00)
			&& (NewIccParameters.Tcckst != 0x02) )
		return SLOTERR_BAD_T01CONVCHECKSUM;

	if( (T01 == 0x01)
			&& (NewIccParameters.Tcckst != 0x10)
			&& (NewIccParameters.Tcckst != 0x11)
			&& (NewIccParameters.Tcckst != 0x12)
			&& (NewIccParameters.Tcckst != 0x13) )
		return SLOTERR_BAD_T01CONVCHECKSUM;

	if( (NewIccParameters.WaitingInteger >= 0xA0)		// condition: BWI > 0xA0 is reserved for future use
			&& ((NewIccParameters.Tcckst & 0x10) == 0x10) )
		return SLOTERR_BAD_WAITINGINTEGER;

	if( (NewIccParameters.ClockStop != 0x00)
			&& (NewIccParameters.ClockStop != 0x03) )
		return SLOTERR_BAD_CLOCKSTOP;

	if( NewIccParameters.Nad != 0x00 )
		return SLOTERR_BAD_NAD;

	IccParameters[portno].FiDi = NewIccParameters.FiDi;
	IccParameters[portno].Tcckst = NewIccParameters.Tcckst;
	
	if( dev->CardCapabilities.InversConvention == false )
		IccParameters[portno].Tcckst &= ~(0x02);
	else
		IccParameters[portno].Tcckst |= 0x02;
	
	IccParameters[portno].GuardTime = NewIccParameters.GuardTime;
	IccParameters[portno].WaitingInteger = NewIccParameters.WaitingInteger;
	IccParameters[portno].ClockStop = NewIccParameters.ClockStop;
	IccParameters[portno].Ifsc = NewIccParameters.Ifsc;
	IccParameters[portno].Nad = NewIccParameters.Nad;

	/* Set the S_SC_DEV_T structure */
	dev->CardCapabilities.Fl = (IccParameters[portno].FiDi & 0xF0)>>4;
	dev->CardCapabilities.Dl = IccParameters[portno].FiDi & 0xF;
	if(IccParameters[portno].Tcckst & 0x02)
	{
		if(dev->CardCapabilities.InversConvention != true)
			printf("Be careful, convention setting is different from before. \n");
		dev->CardCapabilities.InversConvention = true;
	}
	else
	{
		if(dev->CardCapabilities.InversConvention != false)
			printf("Be careful, convention setting is different from before. \n");
		dev->CardCapabilities.InversConvention = false;
	}
	if(IccParameters[portno].Tcckst & 0x10) // T=1
	{
		if(IccParameters[portno].Tcckst & 0x01)	// CRC
			dev->CardCapabilities.T1.EDC = 1;
		else
			dev->CardCapabilities.T1.EDC = 0;

		if(IccParameters[portno].GuardTime == 0xFF)
			dev->CardCapabilities.GT = 11;
		else
			dev->CardCapabilities.GT = IccParameters[portno].GuardTime + 12;

		dev->CardCapabilities.T1.CWI = IccParameters[portno].WaitingInteger & 0xF;
		dev->CardCapabilities.T1.BWI = (IccParameters[portno].WaitingInteger & 0xF0)>>4;
		dev->CardCapabilities.T1.IFSC = IccParameters[portno].Ifsc;
		//CCIDSCDEBUG("T=1, set parameter \n");
	}
	else		// T=0
	{
		if(IccParameters[portno].GuardTime == 0xFF)
			dev->CardCapabilities.GT = 12;
		else
			dev->CardCapabilities.GT = IccParameters[portno].GuardTime + 12;

		dev->CardCapabilities.T0.WI = IccParameters[portno].WaitingInteger;
		//CCIDSCDEBUG("T=0, set parameter \n");
	}
	
	dev->clock_stop_level = (E_CLOCK_STOP)IccParameters[portno].ClockStop;
	
	//ErrorCode = Ifd_ApplyParametersStructure( portno );

	//CCIDSCDEBUG("GT = %d \n", dev->CardCapabilities.GT);

	SMARTCARD_SetReaderParameter( dev );

	return SLOT_NO_ERROR;
}



/**
  * @brief  None
  * @param  portno			Indicate which port to open (0:Port0; 1:Port1)
  * @param  pBlockBuffer	
  * @param  pBlockSize		
  * @retval SLOT_NO_ERROR
  */
unsigned char Ifd_Escape(	int32_t portno,
								unsigned char *pBlockBuffer,
								unsigned int *pBlockSize	)
{
	//S_SC_DEV_T *dev;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	//dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);


	*pBlockSize = 128;

	return SLOT_NO_ERROR;
}



/**
  * @brief  Enable/Disable Clock
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @param  ClockCmd	Specify to enable/disable clock
  * @retval Slot Status Error Code
  */
unsigned char Ifd_SetClock(	int32_t portno,
									unsigned char ClockCmd)
{
	unsigned char ErrorCode;
	S_SC_DEV_T *dev;
	SC_TypeDef *SC;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);
	SC = dev->base;

	ErrorCode = SLOT_NO_ERROR;

	if(SMARTCARD_CheckCardPresent(SC) != SCARD_PRESENT)
		return SLOTERR_ICC_MUTE;

	if((ClockCmd == 0x01) && (dev->clock_stop_level == CLOCKSTOP_NOT_SUPPORT))
		return SLOTERR_CMD_NOT_SUPPORTED;

	if(ClockCmd == 0x00)		// restart clock
	{	
		SMARTCARD_SetClockState(dev, 1);
	}
	else						// stop clock
	{
		SMARTCARD_SetClockState(dev, 0);
	}

	return ErrorCode;
}



/**
  * @brief  Get card slot status
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval Slot Status Error Code
  */
unsigned char Ifd_GetSlotStatus( int32_t portno )
{
	unsigned char Ret = 0x00;
	SC_TypeDef *SC;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;

	if(portno==0)
		SC = SC0;
	else if(portno==1)
		SC = SC1;
	else if(portno==2)
		SC = SC2;
		
	if((SMARTCARD_CheckCardPresent(SC)==SCARD_PRESENT) && (SC->PINCSR & SC_PINCSR_CLK_KEEP))
	{
		Ret = 0x00;
		CCIDSCDEBUG("Ifd_GetSlotStatus:: Running ... \n");
	}

	if((SMARTCARD_CheckCardPresent(SC)==SCARD_PRESENT) && !(SC->PINCSR & SC_PINCSR_CLK_KEEP))
	{
		Ret = 0x01;
		CCIDSCDEBUG("Ifd_GetSlotStatus:: Stop ... \n");
	}

	if(SMARTCARD_CheckCardPresent(SC)==SCARD_ABSENT)
	{
		Ret = 0x02;
		CCIDSCDEBUG("Ifd_GetSlotStatus:: Card absent ... \n");
	}
	
	return Ret;
}


/**
  * @brief  Get clock status of card slot
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval Slot Status Error Code
  */
unsigned char Ifd_GetClockStatus( int32_t portno )
{
	SC_TypeDef *SC;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	SC = (portno)?SC1:SC0;
	
	if( SC->PINCSR & SC_PINCSR_CLK_KEEP)		// clock running
		return 0x00;
	else if( !(SC->PINCSR & SC_PINCSR_CLK_KEEP) && !(SC->PINCSR & SC_PINCSR_CLK_STOP_LEV))		// clock stopped LOW
		return 0x01;
	else if( !(SC->PINCSR & SC_PINCSR_CLK_KEEP) && (SC->PINCSR & SC_PINCSR_CLK_STOP_LEV))		// clock stopped HIGH
		return 0x02;
	else
		return 0x03;
}

/**
  * @brief  Abort all transmiting or receiving process
  * @param  portno		Indicate which port to open (0:Port0; 1:Port1)
  * @retval Slot Status Error Code
  */
unsigned char Ifd_AbortTxRx( int32_t portno )
{
	S_SC_DEV_T *dev;
	SC_TypeDef *SC;

	if(portno != 0 && portno != 1)
		return SLOTERR_BAD_SLOT;
	
	dev =  (S_SC_DEV_T *)((uint32_t)&sc_device[portno]);
	SC = (portno)?SC1:SC0;

	// disable Tx interrupt
	SC->IER &= ~SC_IER_TBE_IE;
	
	// reset parameters
	dev->rcv_cnt = 0;
	dev->rcv_len = 0;
	dev->rcv_pos = 0;
	dev->snd_len = 0;
	dev->snd_pos = 0;
	//dev->rcv_buf = NULL;
	//dev->snd_buf = NULL;

	// Tx/Rx software reset
	SMARTCARD_ClearFIFO(dev);

	return SLOT_NO_ERROR;
	
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


