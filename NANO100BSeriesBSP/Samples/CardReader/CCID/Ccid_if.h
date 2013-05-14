/******************************************************************************
 * @file     Ccid_if.h
 * @brief    The include header file for CCID interface functions.
 * @version  V1.0.1
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __CCID_IF_H__
#define __CCID_IF_H__

#ifdef  __cplusplus
extern "C"
{
#endif



/************************************************************************************/
/*	ERROR CODES for USB Bulk In Messages : bError									*/
/************************************************************************************/

#define		SLOT_NO_ERROR						0x81

#define		SLOTERR_UNKNOWN						0x82

#define		SLOTERR_CMD_NOT_SUPPORTED			0x00
#define		SLOTERR_BAD_LENTGH					0x01
#define		SLOTERR_BAD_SLOT					0x05
#define		SLOTERR_BAD_POWERSELECT				0x07
#define		SLOTERR_BAD_PROTOCOLNUM				0x07
#define		SLOTERR_BAD_CLOCKCOMMAND			0x07
#define		SLOTERR_BAD_ABRFU_3B				0x07
#define		SLOTERR_BAD_ABRFU_2B				0x08
#define		SLOTERR_BAD_LEVELPARAMETER			0x08
#define		SLOTERR_BAD_FIDI					0x0A
#define		SLOTERR_BAD_T01CONVCHECKSUM			0x0B
#define		SLOTERR_BAD_GUARDTIME				0x0C
#define		SLOTERR_BAD_WAITINGINTEGER			0x0D
#define		SLOTERR_BAD_CLOCKSTOP				0x0E
#define		SLOTERR_BAD_IFSC					0x0F
#define		SLOTERR_BAD_NAD						0x10

#define		SLOTERR_CMD_ABORTED					0xFF
#define		SLOTERR_ICC_MUTE					0xFE
#define		SLOTERR_XFR_PARITY_ERROR			0xFD
#define		SLOTERR_XFR_OVERRUN					0xFC
#define		SLOTERR_HW_ERROR					0xFB
#define		SLOTERR_BAD_ATR_TS					0xF8
#define		SLOTERR_BAD_ATR_TCK					0xF7
#define		SLOTERR_ICC_PROTOCOL_NOT_SUPPORTED	0xF6
#define		SLOTERR_ICC_CLASS_NOT_SUPPORTED		0xF5
#define		SLOTERR_PROCEDURE_BYTE_CONFLICT		0xF4
#define		SLOTERR_DEACTIVATED_PROTOCOL		0xF3
#define		SLOTERR_BUSY_WITH_AUTO_SEQUENCE		0xF2
#define		SLOTERR_PIN_TIMEOUT					0xF0
#define		SLOTERR_PIN_CANCELLED				0xEF
#define		SLOTERR_CMD_SLOT_BUSY				0xE0


/************************************************************************************/
/*	ERROR CODES for RDR_TO_PC_HARDWAREERROR Message : bHardwareErrorCode			*/
/************************************************************************************/

#define		HW_ERR_OVERCURRENT			0x01
#define		HW_ERR_VOLTAGEERROR			0x02
#define		HW_ERR_OVERCURRENT_IT		0x04
#define		HW_ERR_VOLTAGEERROR_IT		0x08

//------------------------------------------------------------------------------------
uint8_t PC_to_RDR_IccPowerOn(void);
uint8_t PC_to_RDR_IccPowerOff(void);
uint8_t PC_to_RDR_GetSlotStatus(void);
uint8_t PC_to_RDR_XfrBlock(void);
uint8_t PC_to_RDR_GetParameters(void);
uint8_t PC_to_RDR_ResetParameters(void);
uint8_t PC_to_RDR_SetParameters(void);
uint8_t PC_to_RDR_Escape(void);
uint8_t PC_to_RDR_IccClock(void);
uint8_t PC_to_RDR_Abort(void);
void RDR_to_PC_DataBlock(uint8_t ErrorCode);
void RDR_to_PC_SlotStatus(uint8_t ErrorCode);
void RDR_to_PC_Parameters(uint8_t ErrorCode);
void RDR_to_PC_Escape(uint8_t ErrorCode);
void RDR_to_PC_NotifySlotChange(void);
void RDR_to_PC_HardwareError(void);
void CmdNotSupported(void);
void ccid_T0TimeExtension(void);


#ifdef  __cplusplus
}
#endif

#endif // __CCID_IF_H__

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
