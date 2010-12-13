/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __UAC_HID_SYS_H__
#define __UAC_HID_SYS_H__


#ifdef  __cplusplus
extern "C"
{
#endif

#define UAC_MAJOR_NUM	1
#define UAC_MINOR_NUM	0
#define UAC_BUILD_NUM	1

#define UAC_MICROPHONE  0
#define UAC_SPEAKER		1

// Define module version number.
_SYSINFRA_VERSION_DEF(UAC, UAC_MAJOR_NUM, UAC_MINOR_NUM, UAC_BUILD_NUM)

// E_UAC_SAMPLE_NOT_SUPPORT       Do not support the sample rate
// E_UAC_BUFFER_OVERRUN           Received data is more than buffer size
// E_UAC_CTRL_REG_TAB_FULL        Control register table is full
// E_UAC_INVALID_EP_NUM           EndPoint number error
// E_UAC_NULL_POINTER             Callback function is NULL
// E_UAC_UNDEFINE                 Undefined error
#define E_UAC_UNDEFINE            _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 0)
#define E_UAC_NULL_POINTER        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 1)
#define E_UAC_BUFFER_OVERRUN      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 2)
#define E_UAC_INVALID_EP_NUM      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 3)
#define E_UAC_CTRL_REG_TAB_FULL   _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 4)
#define E_UAC_SAMPLE_NOT_SUPPORT  _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 5)
#define E_HID_NULL_POINTER        _SYSINFRA_ERRCODE(TRUE, MODULE_ID_UAC, 6)


#define PLAY_RATE       16000   /* The audo play sampling rate. The setting is 16KHz */
#define PLAY_CHANNELS   2

#define REC_RATE        16000   /* The record sampling rate. It is 16KHz */
#define REC_CHANNELS    1


/* Define the endpoint number for relative endpoints. 
   The relative endpoint event callback should be implemented if
   the endpoint number setting is changed */
#define CTRL_EP_NUM      0x00
#define ISO_IN_EP_NUM    0x01
#define ISO_OUT_EP_NUM   0x02
#define HID_IN_EP_NUM    0x03
#define HID_OUT_EP_NUM   0x04

#define ISO_OUT_MXPLD   PLAY_RATE*PLAY_CHANNELS*2/1000
#define ISO_IN_MXPLD    REC_RATE*REC_CHANNELS*2/1000
#define HID_OUT_MXPLD   8
#define HID_IN_MXPLD    8


#define REC_FEATURE_UNITID   0x05
#define PLAY_FEATURE_UNITID  0x06

//#define	UAC_MAX_PACKET_SIZE_EP0		64
#define	UAC_MAX_PACKET_SIZE_EP0		8

#define LEN_CONFIG_AND_SUBORDINATE 0xC0


#define UAC_STOP_AUDIO_RECORD		0
#define UAC_START_AUDIO_RECORD		1
#define UAC_PROCESSING_AUDIO_RECORD	2
#define UAC_BUSY_AUDIO_RECORD		3

//***************************************************
//      Audio Class-Specific Request Codes
//***************************************************
#define UAC_REQUEST_CODE_UNDEFINED      0x00
#define UAC_SET_CUR      0x01
#define UAC_GET_CUR      0x81
#define UAC_SET_MIN      0x02
#define UAC_GET_MIN      0x82
#define UAC_SET_MAX      0x03
#define UAC_GET_MAX      0x83
#define UAC_SET_RES      0x04
#define UAC_GET_RES      0x84
#define UAC_SET_MEM      0x05
#define UAC_GET_MEM      0x85
#define UAC_GET_STAT     0xFF

#define MUTE_CONTROL     0x01
#define VOLUME_CONTROL   0x02

    
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

	/* HID */
	const uint8_t *pu8ReportDescriptor;
	uint32_t u32ReportDescriptorSize;

	uint8_t *pu8Report;
	uint32_t u32ReportSize;

	const uint8_t *pu8HIDDescriptor;

    void * *device;

} S_UAC_DEVICE;

extern S_UAC_DEVICE g_UAC_sDevice;

	
typedef void (*UAC_DEVICE_ENABLE)(uint8_t u8Object, uint8_t u8State);
typedef void (*UAC_DEVICE_DISABLE)(uint8_t u8Object);
typedef void (*UAC_ISO_IN)(void);
typedef void (*UAC_ISO_OUT)(int16_t *pi16src, int16_t i16size);


uint32_t UAC_GetVersion(void);
int32_t UAC_Open(void * pVoid);
void UAC_Close(void);
void UAC_Reset(void * *pVoid);
int32_t UAC_IsCfgId(uint8_t u8ConfigureValue);
void UAC_SendOneAudioPacket(uint8_t *pbyAudioData, uint8_t u8AudioSize);
void UAC_Init(
	UAC_DEVICE_ENABLE   pfnDeviceEnable,
	UAC_DEVICE_DISABLE  pfnDeviceDisable,
	UAC_ISO_IN          pfnSendRecData,
    UAC_ISO_OUT         pfnGetPlayData
);

void UAC_CtrlSetupGetInterface(void * pVoid);
void UAC_CtrlSetupSetInterface(void * pVoid);
void UAC_CtrlSetupGetDescriptor(void * pVoid);
void UAC_CtrlDataInGetDescriptor(void * pVoid);
	
void UAC_ClassCtrlSetCurRequest(void * pVoid);
void UAC_ClassCtrlGetCurRequest(void * pVoid);
void UAC_ClassCtrlGetMinRequest(void * pVoid);
void UAC_ClassCtrlGetResRequest(void * pVoid);
void UAC_ClassCtrlGetMaxRequest(void * pVoid);
void UAC_ClassCtrlSetCurOutData(void * pVoid);
uint16_t UAC_GetIsoOutPackeSize(const uint8_t *u8Cfg);
int32_t UAC_GetRecVolume(void);
int32_t UAC_GetPlayVolume(int32_t bIsRight);

void HID_IntInCallback(void * pVoid);
int32_t HID_SetReportDescriptor(const uint8_t* pu8ReportDescriptor, uint32_t u32ReportDescriptorSize);
int32_t HID_SetReportBuf(uint8_t* pu8Report, uint32_t u32ReportSize);



#ifdef  __cplusplus
}
#endif

#endif // __UAC_HID_SYS_H__









