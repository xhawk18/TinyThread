/******************************************************************************
 * @file     Ifd_protocol.h
 * @brief    Header file for Ifd_protocol.c
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
#ifndef __IF_PROTOCOL_H__
#define __IF_PROTOCOL_H__

#ifdef  __cplusplus
extern "C"
{
#endif


#define XFR_NOERROR					0x00
#define XFR_NOANSWER					SC_ERR_TIMEOUT
#define XFR_BADLEVELPARAMETER		SC_EPARAMETER
#define XFR_BUFFEROVERFLOW			SC_ERR_OVER_RUN
#define XFR_PARITYERROR				SC_ERR_PARITY_ERROR

// case number of command
#define CMD_CASE1			0x1
#define CMD_CASE2			0x2
#define CMD_CASE3			0x3
#define CMD_CASE4			0x4

//	static volatile unsigned char	IccTransactionLevelType;
#define TYPE_MASK		0x01
#define LEVEL_MASK		0x0F

#define T0_TYPE			0x00
#define T1_TYPE			0x01

#define CHARACTER_LEVEL				0x00
#define TPDU_LEVEL						0x02
#define SHORTAPDU_LEVEL				0x04
#define SHORTEXTENDAPDU_LEVEL		0x08

#define bTransactionType								(IccTransactionType & TYPE_MASK)
#define SetT0_bTransactionType					(IccTransactionType &= (~TYPE_MASK))
#define SetT1_bTransactionType					(IccTransactionType |= TYPE_MASK)


#define bmTransactionLevel									(IccTransactionLevel & LEVEL_MASK)
#define SetChar_bmTransactionLevel						(IccTransactionLevel &= (~LEVEL_MASK))
#define SetTpdu_bmTransactionLevel						(IccTransactionLevel = (IccTransactionLevel & (~LEVEL_MASK)) | TPDU_LEVEL )
#define SetShortapdu_bmTransactionLevel				(IccTransactionLevel = (IccTransactionLevel & (~LEVEL_MASK)) | SHORTAPDU_LEVEL )
#define SetShortExtendapdu_bmTransactionLevel		(IccTransactionLevel = (IccTransactionLevel & (~LEVEL_MASK)) | SHORTEXTENDAPDU_LEVEL )


// static unsigned char XfrFlag;
#define INS		0x00
#define DATA		0x01


#define DEFAULT_FIDI						0x11
#define DEFAULT_T01CONVCHECKSUM		0x00
#define DEFAULT_GUARDTIME				0x00
#define DEFAULT_WAITINGINTEGER		0x0A
#define DEFAULT_CLOCKSTOP				0x03
#define DEFAULT_IFSC						0x20
#define DEFAULT_NAD						0x00

#define PARAM_CLOCK_NOTSTOPPED		0x00
#define PARAM_CLOCK_STOPLOW			0x01
#define PARAM_CLOCK_STOPHIGH			0x02
#define PARAM_CLOCK_STOPHIGHORLOW	0x03



unsigned char Ifd_Init( int32_t );
unsigned char Ifd_ApplyParametersStructure( int32_t );
unsigned char Ifd_GetHwError( int32_t );
unsigned char Ifd_IccPowerOn(	int32_t,
											unsigned char,
											unsigned char *,
											unsigned int *	);
unsigned char Ifd_XfrBlock(	int32_t portno,
										unsigned char *,
										unsigned int *,
										unsigned int);
unsigned char Ifd_XfrCharT0(	int32_t, 
										unsigned char *,
										unsigned int *,
										unsigned int);
unsigned char Ifd_XfrCharT1(	int32_t,
										unsigned char *,
										unsigned int *,
										unsigned int);
unsigned char Ifd_XfrTpduT0(int32_t,
										unsigned char *,
										unsigned int *);
unsigned char Ifd_XfrTpduT1(int32_t,
										unsigned char *,
										unsigned int *);
unsigned char Ifd_XfrShortApduT0(int32_t,
												unsigned char *,
												unsigned int *);
unsigned char Ifd_XfrShortApduT1(int32_t,
												unsigned char *,
												unsigned int *);
unsigned char Ifd_XfrExtendApduT0(	int32_t,
													unsigned char *,
													unsigned int *);
unsigned char Ifd_XfrExtendApduT1(	int32_t,
													unsigned char *,
													unsigned int *);
unsigned char Ifd_GetParameters(int32_t, unsigned char *);
unsigned char Ifd_SetParameters(	int32_t,
											unsigned char *,
											unsigned char);
unsigned char Ifd_Escape(	int32_t,
								unsigned char *,
								unsigned int *	);
unsigned char Ifd_SetClock(	int32_t,
									unsigned char);
unsigned char Ifd_GetSlotStatus( int32_t );
unsigned char Ifd_GetClockStatus( int32_t );
unsigned char Ifd_AbortTxRx( int32_t portno );

uint8_t Ifd_CheckCmdCase(uint8_t *buf, uint32_t len);

/* function from DrvSMARTCARD_Protocol.c */
extern int SendSBlock ( S_SC_DEV_T *dev, uint8_t control, uint8_t data);
extern uint8_t T1BlockGetType (uint8_t* block);
extern uint8_t T1BlockGetLength (uint8_t* block);
extern uint8_t T1BlockGetNAD (uint8_t* block);

#ifdef  __cplusplus
}
#endif

#endif // __IF_PROTOCOL_H__

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


