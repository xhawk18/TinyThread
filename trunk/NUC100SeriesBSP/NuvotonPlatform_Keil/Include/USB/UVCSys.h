/****************************************************************
 *                                                              *
 * Copyright (c) Nuvoton Technology Corp. All rights reserved.  *
 *                                                              *
 ****************************************************************/
#ifndef __UVC_SYS_H__
#define __UVC_SYS_H__

#ifdef  __cplusplus
extern "C"
{
#endif

#define UVC_MAJOR_NUM	1
#define UVC_MINOR_NUM	0
#define UVC_BUILD_NUM	1


/* Define module version number. */
_SYSINFRA_VERSION_DEF(UVC, UVC_MAJOR_NUM, UVC_MINOR_NUM, UVC_BUILD_NUM)

#define E_UVC_UNDEFINE            _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 0)
#define E_UVC_NULL_POINTER        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 1)
#define E_UVC_BUFFER_OVERRUN      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 2)
#define E_UVC_INVALID_EP_NUM      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 3)
#define E_UVC_CTRL_REG_TAB_FULL   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 4)
#define E_UVC_SAMPLE_NOT_SUPPORT  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UVC, 5)


#define PLAY_RATE       48000   /* The audo play sampling rate. The setting should be: 15000Hz, 30000Hz or 45000Hz*/
#define PLAY_CHANNELS   2


/* Define the endpoint number for relative endpoints. 
   The relative endpoint event callback should be implemented if
   the endpoint number setting is changed */
#define CTRL_EP_NUM      0x00
#define ISO_IN_EP_NUM    0x04
#define ISO_OUT_EP_NUM   0x02

#define ISO_OUT_MXPLD   192
#define ISO_IN_MXPLD    64


#define REC_FEATURE_UNITID  0x05
#define PLAY_FEATURE_UNITID  0x06

//Control Selector Codes
//VideoControl Interface Control Selectors
#define VC_CONTROL_UNDEFINED				0x0000
#define VC_VIDEO_POWER_MODE_CONTROL			0x0100
#define VC_REQUEST_ERROR_CODE_CONTROL			0X0200
#define Reserved					0x0300

//VideoStreaming Interface Control Selectors
#define	VS_CONTROL_UNDEFINED				0x00
#define	VS_PROBE_CONTROL				0x01
#define	VS_COMMIT_CONTROL				0x02
#define	VS_STILL_PROBE_CONTROL				0x03
#define	VS_STILL_COMMIT_CONTROL				0x04
#define	VS_STILL_IMAGE_TRIGGER_CONTROL			0x05
#define	VS_STREAM_ERROR_CODE_CONTROL			0x06
#define	VS_GENERATE_KEY_FRAME_CONTROL			0x07
#define	VS_UPDATE_FRAME_SEGMENT_CONTROL			0x08
#define	VS_SYNCH_DELAY_CONTROL				0x09


#define UVC_CS_INTERFACE  	0x0001
#define UVC_CT_CONTROL		0x0100
#define UVC_PU_CONTROL	 	0x0500
#define UVC_INTERFACE_CTRL	0x0000


#define PLAY_FEATURE_UNITID  0x06


#define	UVC_MAX_PACKET_SIZE_EP0		64

#define LEN_CONFIG_AND_SUBORDINATE 0xC0


//***************************************************
//      Vedio Class-Specific Request Codes
//***************************************************
#define UVC_REQUEST_CODE_UNDEFINED      0x00
#define UVC_SET_CUR      0x01
#define UVC_GET_CUR      0x81
#define UVC_SET_MIN      0x02
#define UVC_GET_MIN      0x82
#define UVC_SET_MAX      0x03
#define UVC_GET_MAX      0x83
#define UVC_SET_RES      0x04
#define UVC_GET_RES      0x84
#define UVC_SET_MEM      0x05
#define UVC_GET_MEM      0x85
#define UVC_GET_INFO     0x86
#define UVC_GET_DEF      0x87
#define UVC_GET_STAT     0xFF

#define MUTE_CONTROL             0x01
#define VOLUME_CONTROL           0x02

__packed typedef struct _VIDEOSTREAMCMDDATA {
    uint16_t  bmHint;
    uint8_t   bFormatIndex;
    uint8_t   bFrameIndex;    
    uint32_t  dwFrameInterval;        
    uint16_t  dwKeyFrameRate;    
    uint16_t  wPFrameRate;    
    uint16_t  wCompQuality;     
    uint16_t  wCompWindowSize;    
    uint16_t  wDelay;    
    uint32_t  dwMaxVideoFrameSize; 
    uint32_t  dwMaxPayloadTransferSize;           
} VIDEOSTREAMCMDDATA;
    
typedef struct
{	
	uint8_t u8AlternativeSetting;
	uint8_t u8AudioState;
	uint8_t u8IsoOutPacketSize;
	uint8_t u8IsoOutToggle;
	
	uint8_t  u8RecMute;
	int16_t  i16RecVolume;
	int16_t  i16RecMaxVolume;
	int16_t  i16RecMinVolume;
	int16_t  i16RecResVolume;	


	uint8_t  u8PlayMute;
	int16_t  i16PlayVolumeL;
	int16_t  i16PlayVolumeR;
	int16_t  i16PlayMaxVolume;
	int16_t  i16PlayMinVolume;
	int16_t  i16PlayResVolume;	

    void * *device;

} S_UVC_DEVICE;

extern S_UVC_DEVICE g_UVC_sDevice;

	
typedef void (*UVC_DEVICE_ENABLE)(uint8_t u8Object, uint8_t u8State);
typedef void (*UVC_DEVICE_DISABLE)(uint8_t u8Object);
typedef void (*UVC_ISO_IN)(void);
typedef void (*UVC_ISO_OUT)(int16_t *pi16src, int16_t i16size);


uint32_t UVC_GetVersion(void);
int32_t UVC_Open(void * pVoid);
void UVC_Close(void);
void UVC_Reset(void * *pVoid);
int32_t UVC_IsCfgId(uint8_t u8ConfigureValue);
void UVC_SendOnePacket(uint8_t *pbyData, uint8_t u8DataSize);
void UVC_Init(
	UVC_DEVICE_ENABLE   pfnDeviceEnable,
	UVC_DEVICE_DISABLE  pfnDeviceDisable,
	UVC_ISO_IN          pfnSendData,
    UVC_ISO_OUT         pfnGetPlayData
);

void UVC_CtrlSetupGetInterface(void * pVoid);
void UVC_CtrlSetupSetInterface(void * pVoid);
void UVC_CtrlSetupGetDescriptor(void * pVoid);
void UVC_CtrlDataInGetDescriptor(void * pVoid);
	
void UVC_ClassCtrlSetCurRequest(void * pVoid);
void UVC_ClassCtrlGetCurRequest(void * pVoid);
void UVC_ClassCtrlGetMinRequest(void * pVoid);
void UVC_ClassCtrlGetResRequest(void * pVoid);
void UVC_ClassCtrlGetMaxRequest(void * pVoid);
void UVC_ClassCtrlSetCurOutData(void * pVoid);
uint16_t UVC_GetIsoOutPackeSize(const uint8_t *u8Cfg);
int32_t UVC_GetRecVolume(void);
int32_t UVC_GetPlayVolume(int32_t bIsRight);

#ifdef  __cplusplus
}
#endif

#endif // __UVC_SYS_H__




































