/******************************************************************************
 * @file     Ccid_if.c
 * @brief    This file process CCID commands
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/  
#include <string.h>
#include "nano1xx_usb.h"
#include "nano1xx_smartcard.h"
#include "ccid_class.h"
#include "ccid_if.h"
#include "Ifd_protocol.h"


extern uint8_t g_bBulkOutCompleteFlag, g_bBulkInCompleteFlag;

extern volatile S_SC_DEV_T sc_device[];

extern uint8_t UsbIntMessageBuffer[];
extern uint8_t UsbMessageBuffer[];

extern unsigned char g_ChainParameter;

//======================================================
// Bulk-Out Message Functions
//======================================================
uint8_t PC_to_RDR_IccPowerOn(void)
{
	uint8_t ErrorCode, Voltage;
	uint32_t AtrSize;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_BPOWERSELECT] >= 0x04)
		return SLOTERR_BAD_POWERSELECT;
	if (make16(&UsbMessageBuffer[OFFSET_ABRFU_2B]) != 0)
		return SLOTERR_BAD_ABRFU_2B;

	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	Voltage = UsbMessageBuffer[OFFSET_BPOWERSELECT];
	ErrorCode = Ifd_IccPowerOn(UsbMessageBuffer[OFFSET_BSLOT], Voltage, &UsbMessageBuffer[OFFSET_ABDATA], &AtrSize);


	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	if(AtrSize == 0) {
		ErrorCode = SLOTERR_ICC_MUTE; 	//card removed during activation
	}		

	if(ErrorCode == SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_DWLENGTH] = (uint8_t) AtrSize;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = (uint8_t) (AtrSize>>8);
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = (uint8_t) (AtrSize>>16);
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = (uint8_t) (AtrSize>>24);
		return SLOT_NO_ERROR;
	}


	return ErrorCode;
}

uint8_t PC_to_RDR_IccPowerOff(void)
{
	uint32_t volatile i = 0x200;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		||UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;


	SMARTCARD_DeactivationCmd((S_SC_DEV_T *)&sc_device[1]);

	while(i--);		/* delay */
	
	UsbMessageBuffer[OFFSET_BCLOCKSTATUS] = 0x01;
	return SLOT_NO_ERROR;
}

uint8_t PC_to_RDR_GetSlotStatus(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	return ErrorCode;
}

uint8_t PC_to_RDR_XfrBlock(void)
{
	uint8_t ErrorCode;
	uint32_t BlockSize, ExpectedAnswerSize;
	uint32_t UsbMessageLength;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	BlockSize = make32(&UsbMessageBuffer[OFFSET_DWLENGTH]);
	UsbMessageLength = USB_MESSAGE_HEADER_SIZE + BlockSize;

	if(UsbMessageLength > USB_MESSAGE_BUFFER_MAX_LENGTH)
		return SLOTERR_BAD_LENTGH;

	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	// ExpectedAnswerSize variable just uses by Character Level
	ExpectedAnswerSize = make16(&UsbMessageBuffer[OFFSET_WLEVELPARAMETER]);

	//printf("BlockSize=%d, ExpectedAnswerSize=%d \n", BlockSize, UsbMessageBuffer[OFFSET_WLEVELPARAMETER]);

	ErrorCode = Ifd_XfrBlock(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABDATA], &BlockSize, ExpectedAnswerSize);
	

	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	if(ErrorCode == SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_DWLENGTH] = (uint8_t) BlockSize;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = (uint8_t) (BlockSize>>8);
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = (uint8_t) (BlockSize>>16);
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = (uint8_t) (BlockSize>>24);
	}

	return ErrorCode;
}

uint8_t PC_to_RDR_GetParameters(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);

	return ErrorCode;
}

uint8_t PC_to_RDR_ResetParameters(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;

	UsbMessageBuffer[OFFSET_ABDATA] = DEFAULT_FIDI;
	UsbMessageBuffer[OFFSET_ABDATA+1] = DEFAULT_T01CONVCHECKSUM;
	UsbMessageBuffer[OFFSET_ABDATA+2] = DEFAULT_GUARDTIME;
	UsbMessageBuffer[OFFSET_ABDATA+3] = DEFAULT_WAITINGINTEGER;
	UsbMessageBuffer[OFFSET_ABDATA+4] = DEFAULT_CLOCKSTOP;
	UsbMessageBuffer[OFFSET_ABDATA+5] = 0x00;
	UsbMessageBuffer[OFFSET_ABDATA+6] = 0x00;

	if(UsbMessageBuffer[OFFSET_ABPROTOCOLDATASTRUCTURE+1] & 0x10)
		ErrorCode = Ifd_SetParameters(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABPROTOCOLDATASTRUCTURE], 0x01);	/* T=1 */
	else
		ErrorCode = Ifd_SetParameters(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABPROTOCOLDATASTRUCTURE], 0x00);	/* T=0 */
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	return ErrorCode;
}

uint8_t PC_to_RDR_SetParameters(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if ((make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0x05) &&
		(make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0x07))
		return SLOTERR_BAD_LENTGH;
	if( ((UsbMessageBuffer[OFFSET_DWLENGTH] == 0x05)
			&& (UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] != 0x00))
		|| ((UsbMessageBuffer[OFFSET_DWLENGTH] == 0x07)
			&& (UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] != 0x01)) )
		return SLOTERR_BAD_PROTOCOLNUM;
	if(UsbMessageBuffer[OFFSET_ABRFU_2B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_2B+1] != 0)
		return SLOTERR_BAD_ABRFU_2B;

	ErrorCode = Ifd_SetParameters(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABDATA], UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT]);


	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	return ErrorCode;
}

uint8_t PC_to_RDR_Escape(void)
{
	uint8_t ErrorCode;
	uint32_t BlockSize, UsbMessageLength;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	BlockSize = make32(&UsbMessageBuffer[OFFSET_DWLENGTH]);
	UsbMessageLength = USB_MESSAGE_HEADER_SIZE + BlockSize;
	if(UsbMessageLength > USB_MESSAGE_BUFFER_MAX_LENGTH)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;

	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	ErrorCode = Ifd_Escape(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABDATA], &BlockSize);

	if( gCcidInfo.bAbortRequestFlag )
		return SLOTERR_CMD_ABORTED;

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	UsbMessageBuffer[OFFSET_DWLENGTH] = (uint8_t) BlockSize;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = (uint8_t) (BlockSize>>8);
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = (uint8_t) (BlockSize>>16);
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = (uint8_t) (BlockSize>>24);

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	return ErrorCode;
}

uint8_t PC_to_RDR_IccClock(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_BCLOCKCOMMAND] > 0x01)
		return SLOTERR_BAD_CLOCKCOMMAND;
	if(UsbMessageBuffer[OFFSET_ABRFU_2B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_2B+1] != 0)
		return SLOTERR_BAD_ABRFU_2B;

	ErrorCode = Ifd_SetClock(UsbMessageBuffer[OFFSET_BSLOT], UsbMessageBuffer[OFFSET_BCLOCKCOMMAND]);
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

	ErrorCode = Ifd_GetHwError(UsbMessageBuffer[OFFSET_BSLOT]);
	return ErrorCode;
}

uint8_t PC_to_RDR_Abort(void)
{
	uint8_t ErrorCode;

	//if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
	//	return SLOTERR_BAD_SLOT;
	if (make32(&UsbMessageBuffer[OFFSET_DWLENGTH]) != 0)
		return SLOTERR_BAD_LENTGH;
	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERR_BAD_ABRFU_3B;

	if( (!gCcidInfo.bAbortRequestFlag) /*|| (g_bSeq != UsbMessageBuffer[OFFSET_BSEQ]) 
		|| (g_bSlot != UsbMessageBuffer[OFFSET_BSLOT])*/)
		return SLOTERR_CMD_ABORTED;

	ErrorCode = Ifd_AbortTxRx(UsbMessageBuffer[OFFSET_BSLOT]);
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;
	
	gCcidInfo.bAbortRequestFlag = 0;

	return SLOT_NO_ERROR;
}

//======================================================
// Bulk-In Message Functions
//======================================================
void RDR_to_PC_DataBlock(uint8_t ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_DATABLOCK;
	UsbMessageBuffer[OFFSET_BSTATUS] = Ifd_GetSlotStatus(UsbMessageBuffer[OFFSET_BSLOT]);

	UsbMessageBuffer[OFFSET_BERROR] = 0x80;
	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] = ErrorCode;
	}
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] = g_ChainParameter;
	gCcidInfo.isBulkInReady = 1;
}

void RDR_to_PC_SlotStatus(uint8_t ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_SLOTSTATUS;
	UsbMessageBuffer[OFFSET_BSTATUS] = Ifd_GetSlotStatus(UsbMessageBuffer[OFFSET_BSLOT]);

	UsbMessageBuffer[OFFSET_BERROR] = 0x80;
	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] = ErrorCode;
	}
	UsbMessageBuffer[OFFSET_BCLOCKSTATUS] = Ifd_GetClockStatus(UsbMessageBuffer[OFFSET_BSLOT]);
	gCcidInfo.isBulkInReady = 1;
}

void RDR_to_PC_Parameters(uint8_t ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_PARAMETERS;
	UsbMessageBuffer[OFFSET_BSTATUS] = Ifd_GetSlotStatus(UsbMessageBuffer[OFFSET_BSLOT]);
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	UsbMessageBuffer[OFFSET_BERROR] = 0x80;
	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] = ErrorCode;
		gCcidInfo.isBulkInReady = 1;
		return;
	}

	UsbMessageBuffer[OFFSET_BPROTOCOLNUM_IN] = Ifd_GetParameters(UsbMessageBuffer[OFFSET_BSLOT], &UsbMessageBuffer[OFFSET_ABPROTOCOLDATASTRUCTURE]);

	if(UsbMessageBuffer[OFFSET_BPROTOCOLNUM_IN])
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x07;
	else
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x05;
	gCcidInfo.isBulkInReady = 1;
}

void RDR_to_PC_Escape(uint8_t ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_ESCAPE;
	UsbMessageBuffer[OFFSET_BSTATUS] = Ifd_GetSlotStatus(UsbMessageBuffer[OFFSET_BSLOT]);
	UsbMessageBuffer[OFFSET_BERROR] = 0x80;
	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] = ErrorCode;
	}
	UsbMessageBuffer[OFFSET_BRFU] = 0x00;
	gCcidInfo.isBulkInReady = 1;
}

//======================================================
// Interrupt-In Message Functions
//======================================================
void RDR_to_PC_NotifySlotChange(void)
{
	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_NOTIFYSLOTCHANGE;

	if( SMARTCARD_CheckCardPresent( SC1 ) == SCARD_PRESENT )
	{
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x03;
	}
	else
	{
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x02;
	}
}

void RDR_to_PC_HardwareError(void)
{
	uint8_t ErrorCode;

	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_HARDWAREERROR;
	UsbIntMessageBuffer[OFFSET_INT_BSLOT] = 0x00;
	UsbIntMessageBuffer[OFFSET_INT_BSEQ] = UsbMessageBuffer[OFFSET_BSEQ];
	ErrorCode = Ifd_GetHwError(UsbIntMessageBuffer[OFFSET_INT_BSLOT]);
	UsbIntMessageBuffer[OFFSET_INT_BHARDWAREERRORCODE] = ErrorCode;

	//??  How to specify the slot number between 0 and 1???????
	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_HARDWAREERROR;
	UsbIntMessageBuffer[OFFSET_INT_BSLOT] = 0x01;
	UsbIntMessageBuffer[OFFSET_INT_BSEQ] = UsbMessageBuffer[OFFSET_BSEQ];
	ErrorCode = Ifd_GetHwError(UsbIntMessageBuffer[OFFSET_INT_BSLOT]);
	UsbIntMessageBuffer[OFFSET_INT_BHARDWAREERRORCODE] = ErrorCode;
}


//======================================================
// Not Support Message
//======================================================
void CmdNotSupported(void)
{
	UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	UsbMessageBuffer[OFFSET_BSTATUS] = 0x40 + Ifd_GetSlotStatus(UsbMessageBuffer[OFFSET_BSLOT]);
	UsbMessageBuffer[OFFSET_BERROR] = 0x00;
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] = 0x00;
}

//======================================================
// Bulk-In Time Extension Message Function
//======================================================
void ccid_T0TimeExtension(void)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_DATABLOCK;
	UsbMessageBuffer[OFFSET_BSTATUS] = 0x80;
	UsbMessageBuffer[OFFSET_BERROR] = 0x01;
	UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] = 0x00;
	my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), UsbMessageBuffer, 10);
	USBD->MXPLD2 = 10;	/* BULK IN */
}

//======================================================
// Bulk-In Time Extension Message Function
//======================================================
void ccid_T1TimeExtension(void)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_DATABLOCK;
	UsbMessageBuffer[OFFSET_BSTATUS] = 0x80;
	UsbMessageBuffer[OFFSET_BERROR] = 0x03;
	UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] = 0x00;
	my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), UsbMessageBuffer, 10);
	USBD->MXPLD2 = 10;	/* BULK IN */
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


