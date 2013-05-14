/******************************************************************************
 * @file     USB_Class.h
 * @brief    Nano1xx USB Driver Sample header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#define AUDIO_RATE		PLAY_RATE
#define AUDIO_CHANNELS	PLAY_CHANNELS
#define AUDIO_BIT_RATE	PLAY_BIT_RATE

#define UAC_MICROPHONE  0
#define UAC_SPEAKER		1

/*!<Define Audio information */
#define PLAY_RATE       16000   /* The audo play sampling rate. The setting is 16KHz */
#define PLAY_CHANNELS   2
#define PLAY_BIT_RATE	0x10	/* 16-bit data rate */

#define REC_RATE        16000   /* The record sampling rate. It is 16KHz */
#define REC_CHANNELS    1
#define REC_BIT_RATE	0x10	/* 16-bit data rate */


#define REC_FEATURE_UNITID		0x05
#define PLAY_FEATURE_UNITID		0x06

#define BUFF_LEN    32*12
#define REC_LEN     REC_RATE / 1000

/********************************************/
/* Audio Class Current State                */
/********************************************/
/*!<Define Audio Class Current State */
#define UAC_STOP_AUDIO_RECORD			0
#define UAC_START_AUDIO_RECORD			1
#define UAC_PROCESSING_AUDIO_RECORD		2
#define UAC_BUSY_AUDIO_RECORD			3

/***************************************************/
/*      Audio Class-Specific Request Codes		   */
/***************************************************/
/*!<Define Audio Class Specific Request */
#define UAC_REQUEST_CODE_UNDEFINED	0x00
#define UAC_SET_CUR					0x01
#define UAC_GET_CUR					0x81
#define UAC_SET_MIN					0x02
#define UAC_GET_MIN					0x82
#define UAC_SET_MAX					0x03
#define UAC_GET_MAX					0x83
#define UAC_SET_RES					0x04
#define UAC_GET_RES					0x84
#define UAC_SET_MEM					0x05
#define UAC_GET_MEM					0x85
#define UAC_GET_STAT				0xFF

#define MUTE_CONTROL				0x01
#define VOLUME_CONTROL				0x02


/***************************************************/
/*      HID Class-Specific Request Codes		   */
/***************************************************/
/*!<Define HID Class Specific Request */
#define GET_REPORT					0x01
#define GET_IDLE					0x02
#define GET_PROTOCOL				0x03
#define SET_REPORT					0x09
#define SET_IDLE					0x0A
#define SET_PROTOCOL				0x0B


/****************************************************/
/*      Audio Class Function APIs					*/
/****************************************************/
void SendRecData(void);
void GetPlayData(int16_t *pi16src, int16_t i16Samples);

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
