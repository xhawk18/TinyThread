/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "DrvUSB.h"
#include "UVCSys.h"
#include "UVC_API.h"

#define Maximum(a,b)		((a)>(b) ? (a) : (b))
#define Minimum(a,b)		((a)<(b) ? (a) : (b))

S_UVC_DEVICE g_UVC_sDevice;

/* The UVC control and data callback */
UVC_DEVICE_ENABLE   pfnUvcDeviceEnable;
UVC_DEVICE_DISABLE  pfnUvcDeviceDisable;
UVC_ISO_IN          pfnUvcSendData;
UVC_ISO_OUT         pfnUvcGetPlayData;

void UVC_EpNum1InEvent(void * pVoid);
void UVC_EpNum2OutEvent(void * pVoid);
void UVC_CtrlSetupSetInterface(void * pVoid);

#define CFG_ISO_IN      (CFG_ISOCH | ISO_IN_EP_NUM  | CFG_EPT_IN)
#define CFG_ISO_OUT     (CFG_ISOCH | ISO_OUT_EP_NUM | CFG_EPT_OUT)

__align(4) VIDEOSTREAMCMDDATA  VideoStreamCmdCtlData;

/* Endpoint description */
S_DRVUSB_EP_CTRL sEpDescription[] = 
{
	{CTRL_EP_NUM    | EP_INPUT,  UVC_MAX_PACKET_SIZE_EP0, NULL},
	{CTRL_EP_NUM    | EP_OUTPUT, UVC_MAX_PACKET_SIZE_EP0, NULL},
	{ISO_IN_EP_NUM  | EP_INPUT,  ISO_IN_MXPLD,            NULL},
	{ISO_OUT_EP_NUM | EP_OUTPUT, ISO_OUT_MXPLD,     	  NULL},
	{0,0,0}
};

//bus event call back function
S_DRVUSB_EVENT_PROCESS g_sBusOps[6] = 
{
	{NULL, NULL},//attach
	{NULL, NULL},//detach
	{DrvUSB_BusResetCallback, &g_UVC_sDevice},//bus reset
	{NULL, NULL},//bus suspend
	{NULL, NULL},//bus resume
	{DrvUSB_CtrlSetupAck, &g_UVC_sDevice},//setup
};

//USB event call back function
S_DRVUSB_EVENT_PROCESS g_sUsbOps[12] = 
{
	{DrvUSB_CtrlDataInAck, &g_UVC_sDevice},
	{DrvUSB_CtrlDataOutAck, &g_UVC_sDevice},//ctrl pipe0
	{UVC_EpNum1InEvent,  &g_UVC_sDevice},
  	{NULL, NULL},//endpoint address 1
	{NULL, NULL},
	{UVC_EpNum2OutEvent, &g_UVC_sDevice},//endpoint address 2
  	{NULL, NULL},	
	{NULL, NULL},//endpoint address 3
	{UVC_EpNum1InEvent, &g_UVC_sDevice},
	{NULL, NULL},//endpoint address 4
	{NULL, NULL},
	{NULL, NULL},//endpoint address 5
};


/* ctrl pipe call back.                                                                   */
/* it will be call by DrvUSB_CtrlSetupAck, DrvUSB_CtrlDataInAck and DrvUSB_CtrlDataOutAck */
/* if in ack handler and out ack handler is 0, default handler will be called             */
S_DRVUSB_CTRL_CALLBACK_ENTRY g_asCtrlCallbackEntry[64] =
{  /* request type,command     ,setup ack handler         , in ack handler      ,out ack handler,  parameter */
	{REQ_STANDARD, SET_ADDRESS, DrvUSB_CtrlSetupSetAddress, DrvUSB_CtrlDataInSetAddress, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, CLEAR_FEATURE, DrvUSB_CtrlSetupClearSetFeature, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, SET_FEATURE, DrvUSB_CtrlSetupClearSetFeature, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, GET_CONFIGURATION, DrvUSB_CtrlSetupGetConfiguration, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, GET_STATUS, DrvUSB_CtrlSetupGetStatus, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, GET_INTERFACE, UVC_CtrlSetupGetInterface, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, SET_INTERFACE, UVC_CtrlSetupSetInterface, NULL, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, GET_DESCRIPTOR, UVC_CtrlSetupGetDescriptor, UVC_CtrlDataInGetDescriptor, NULL, &g_UVC_sDevice},
	{REQ_STANDARD, SET_CONFIGURATION, DrvUSB_CtrlSetupSetConfiguration, NULL, NULL, &g_UVC_sDevice},
	{REQ_CLASS, UVC_SET_CUR, UVC_ClassCtrlSetCurRequest, NULL, UVC_ClassCtrlSetCurOutData, &g_UVC_sDevice},
	{REQ_CLASS, UVC_GET_CUR, UVC_ClassCtrlGetCurRequest, NULL, NULL, &g_UVC_sDevice},
	{REQ_CLASS, UVC_GET_MIN, UVC_ClassCtrlGetMinRequest, NULL, NULL, &g_UVC_sDevice},
	{REQ_CLASS, UVC_GET_MAX, UVC_ClassCtrlGetMaxRequest, NULL, NULL, &g_UVC_sDevice},
	{REQ_CLASS, UVC_GET_RES, UVC_ClassCtrlGetResRequest, NULL, NULL, &g_UVC_sDevice},
};

/* -128 ~ 24 dB in 2 dB step in Q12. Formula: x = 10.^([-128:2:24]/20) */
uint32_t gVolumeTbl[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,3,3,4,5,6,8,10,13,16,21,26,33,41,52,65,82,103,130,163,205,258,325,410,516,649,817,1029,1295,1631,2053,2584,3254,4096,5157,6492,8173,10289,12953,16306,20529,25844,32536,40960,51566,64917
};

/* The structure registered to USB driver */
S_DRVUSB_CLASS sUvcUsbClass = 
{
	(void *)&g_UVC_sDevice, 
	(PFN_DRVUSB_CALLBACK)UVC_Reset,
	UVC_IsCfgId
};

static  volatile uint16_t s_u16Len;
static  volatile uint16_t s_u16SendLength, s_u16CurPtr, s_u16Toggle;
static  volatile uint8_t  s_u8ConfigLenOver64;

void UVC_CtrlSetupGetInterface(void * pVoid)
{
	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  
  	DrvUSB_DataIn(0, &psDevice->u8AlternativeSetting, 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_CtrlSetupSetInterface()	          	                                                           */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      Setup ACK handler for SET_INTERFACE request                                                        */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   The function point of UVC device                                                           */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                                                                                               */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_CtrlSetupSetInterface(void * pVoid)
{
    S_DRVUSB_DEVICE *psUSBDevice = &gsUsbDevice;
    S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *)pVoid;

	psDevice->u8AlternativeSetting = psUSBDevice->au8Setup[2];
	
    if (psUSBDevice->au8Setup[4] == 1)
    {
        /* Iso IN interface */
    	if (psDevice->u8AlternativeSetting == 1)
	    {
            DrvUSB_DataIn(ISO_IN_EP_NUM, NULL, 0);
     	}
     	else if (psDevice->u8AlternativeSetting == 0)
    	{
            DrvUSB_DataIn(ISO_IN_EP_NUM, NULL, 0);
	    }
	}
	else if (psUSBDevice->au8Setup[4] == 2)
	{
	    /* Iso OUT interface */
    	    	
	}
	
    DrvUSB_DataIn(0, NULL, 0);
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_EpNum1InEvent()                                                                                */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      Call back function for ISO pipe IN ack of endpoint number 1.                                       */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   The function point of UVC device                                                           */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                                                                                               */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_EpNum1InEvent(void * pVoid)
{
    uint32_t u32PacketType;
	
	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
	S_DRVUSB_DEVICE *psUsbDevice;
	int32_t u32EpId;	
	
	psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
	
	u32EpId = DrvUSB_GetEpId(ISO_IN_EP_NUM | 0x80);
    u32PacketType = ((psUsbDevice->u32EPSTS >> (4 + u32EpId * 3))  & EPSTS_STS);
	
	/* Confirm it is ISO IN event */
	if (u32PacketType == EPSTS_ISO)
	{
		pfnUvcSendData();
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_EpNum2OutEvent()                                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      Call back function for ISO pipe OUT ack of endpoint number 2.                                      */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   The function point of UVC device                                                           */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                                                                                               */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_EpNum2OutEvent(void * pVoid)
{
    uint32_t tmp;
    uint8_t *pcmBuf;
    uint32_t u32EpId;
    S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;

    pcmBuf = DrvUSB_GetOutData(ISO_OUT_EP_NUM, &tmp);
    tmp = (PLAY_RATE / 1000) * PLAY_CHANNELS;
    pfnUvcGetPlayData((int16_t *)pcmBuf, (int16_t)tmp);
    u32EpId = DrvUSB_GetEpIdentity(ISO_OUT_EP_NUM, EP_OUTPUT);
    
    _DRVUSB_SET_EP_BUF(u32EpId, (uint32_t)(pcmBuf));
    _DRVUSB_TRIG_EP(u32EpId, ISO_OUT_MXPLD);
    if (psDevice->u8IsoOutToggle == 0)
    {
        psDevice->u8IsoOutToggle = 1;
    }
    else 
    {
        psDevice->u8IsoOutToggle = 0;
    }
    
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_GetVersion()	             	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      To get the UVC driver version.                                                                     */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      The UVC driver version.                                                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t UVC_GetVersion(void)
{
	return UVC_VERSION_NUM;
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_GetIsoOutPackeSize()	      	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      To get the ISO OUT packet size according configuration descriptor.                                 */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      u8Cfg   The pointer of configuration descriptor.                                                   */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      The packet size of ISO OUT.                                                                        */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint16_t UVC_GetIsoOutPackeSize(const uint8_t *u8Cfg)
{
    uint16_t packetSize;
    int32_t offset;
    
    offset = 0;
    while(1)
    {
        offset += u8Cfg[offset];
        
        /* Limit the max search size */
        if (offset > LEN_CONFIG_AND_SUBORDINATE)
            break;
    
        if ((u8Cfg[offset + 1] == 0x05) && (u8Cfg[offset + 3] == 0x0D))
        {
            packetSize = u8Cfg[offset + 4] | ((uint16_t)u8Cfg[offset + 5] << 8);
            return packetSize;
        }       
    }
    
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_Open()	                       	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      To initial all descriptors and control handlers of USB Video Class.                                */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   The function point of UVC device                                                           */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t UVC_Open(void * pVoid)
{		
	g_UVC_sDevice.device = (void *)DrvUSB_InstallClassDevice(&sUvcUsbClass);
	g_UVC_sDevice.u8IsoOutPacketSize = UVC_GetIsoOutPackeSize(gau8ConfigDescriptor);
	
	g_UVC_sDevice.i16RecMaxVolume = 6144;   /* about 24dB */
	g_UVC_sDevice.i16RecMinVolume = -32767; /* about -128dB */
    g_UVC_sDevice.i16RecResVolume = 256;    /* 1dB step */

	g_UVC_sDevice.i16PlayMaxVolume = 768;   /* about 3dB */
	g_UVC_sDevice.i16PlayMinVolume = -8191; /* about -32dB */
    g_UVC_sDevice.i16PlayResVolume = 256;   /* 1dB step */
  
	DrvUSB_InstallCtrlHandler(g_UVC_sDevice.device, g_asCtrlCallbackEntry, 
					sizeof(g_asCtrlCallbackEntry) / sizeof(g_asCtrlCallbackEntry[0]));

    DrvUSB_EnableSelfPower();
    DrvUSB_DisableRemoteWakeup();

	return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_Close()	                       	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      None.                                                                                              */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_Close(void)
{

}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_Reset()	                       	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      The function to reset endpoint settings. It would be called at set configuration or set interface. */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      psDevice   The function point of UVC device                                                        */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_Reset(void * *pVoid)
{
    uint32_t u32EpId;
    S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *)pVoid;
    S_DRVUSB_DEVICE *psUsbDevice;

    psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
    
	/* Reset ISO IN endpoint setting */    
    u32EpId = DrvUSB_GetEpIdentity(ISO_IN_EP_NUM, EP_INPUT);
  	_DRVUSB_SET_CFG(u32EpId, CFG_ISO_IN);
	_DRVUSB_SET_CFGP(u32EpId, CFGP_CLRRDY);
    _DRVUSB_SET_EP_BUF(u32EpId, (uint32_t)psUsbDevice->sEpCrl[u32EpId].u8SramBuffer);
    
    /* Reset ISO OUT endpoint setting */    
    u32EpId = DrvUSB_GetEpIdentity(ISO_OUT_EP_NUM, EP_OUTPUT);
	_DRVUSB_SET_CFG(u32EpId, CFG_ISO_OUT);
	_DRVUSB_SET_CFGP(u32EpId, CFGP_CLRRDY);  
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_IsCfgId()	                  	                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      To check if it is the specified bConfigurationValue in the confiuration descriptor.                */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      u8ConfigureValue   The bConfigurationValue to check.                                               */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t UVC_IsCfgId(uint8_t u8ConfiguationValue)
{
	return (u8ConfiguationValue == gau8ConfigDescriptor[5]);
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_SendOnePacket()	       	                                                                       */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      Send one packet to host.                                                                           */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pbyData        The data buffer pointer.                                                            */
/*      u8DataSize     The data size in byte.                                                              */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_SendOnePacket(uint8_t *pbyData, uint8_t u8DataSize)
{
    if (u8DataSize > 0)
    {
        DrvUSB_DataIn(ISO_IN_EP_NUM, (uint8_t *)pbyData, u8DataSize);
    }
    else
    {
       _DRVUSB_TRIG_EP(DrvUSB_GetEpIdentity(ISO_IN_EP_NUM, EP_INPUT), 0);  
    }      
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_Init()	       	                                                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      Install the callback functions for device enable, disable, ISO IN and ISO OUT.                     */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pfnDeviceEnable   The callback function to enable video device.                                    */
/*      pfnDeviceDisable  The callback function to disable video device.                                   */
/*      pfnSendData       The callback function to prepare the data for ISO IN.                            */
/*      pfnGetPlayData    The callback function to get ISO OUT data.                                       */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_Init(
	UVC_DEVICE_ENABLE   pfnDeviceEnable,
	UVC_DEVICE_DISABLE  pfnDeviceDisable,
	UVC_ISO_IN          pfnSendData,
    UVC_ISO_OUT         pfnGetPlayData
)
{
    pfnUvcDeviceEnable  = pfnDeviceEnable;
    pfnUvcDeviceDisable = pfnDeviceDisable;
    pfnUvcSendData   = pfnSendData;
    pfnUvcGetPlayData   = pfnGetPlayData;
}


/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_CtrlSetupGetDescriptor()	       	                                                           */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      The callback funciton for get descriptor request ack event.                                        */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   A pointer of UVC device.                                                                   */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_CtrlSetupGetDescriptor(void * pVoid)
{
	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
	S_DRVUSB_DEVICE *pInfraDevice = (S_DRVUSB_DEVICE *)psDevice->device;
	uint16_t u16Len;
	uint16_t u16ConfigLen;
	s_u8ConfigLenOver64 = 0;

	u16Len = 0;
	u16Len = pInfraDevice->au8Setup[7];
	u16Len <<= 8;
	u16Len += pInfraDevice->au8Setup[6];
	
	switch (pInfraDevice->au8Setup[3])
	{
		/* Get Device Descriptor */
		case DESC_DEVICE:
		{
			u16Len = Minimum(u16Len, LEN_DEVICE);
		
			DrvUSB_DataIn(0, gau8DeviceDescriptor, u16Len);
			break;
		}
		/* Get Configuration Descriptor */
		case DESC_CONFIG:
		{
			u16ConfigLen = gau8ConfigDescriptor[3];
			u16ConfigLen <<= 8;
			u16ConfigLen |= gau8ConfigDescriptor[2];

			s_u16Len = Minimum(u16Len, u16ConfigLen);
			s_u16SendLength = 64;
			s_u16CurPtr = 0;
			s_u16Toggle = 0;		

			if (s_u16Len > s_u16SendLength)
			{
				DrvUSB_DataIn(0, gau8ConfigDescriptor + s_u16CurPtr, s_u16SendLength);

				s_u16Len -= s_u16SendLength;
				s_u16CurPtr += s_u16SendLength;
				s_u8ConfigLenOver64 = 1;
			}
			else
			{
				/* Trigger next Control In DATA1 Transaction. */
				DrvUSB_DataIn(0, gau8ConfigDescriptor + s_u16CurPtr, s_u16Len);
			}
			break;
		}
		/* Get String Descriptor */
		case DESC_STRING:
		{
			/* Get Language */
			if (pInfraDevice->au8Setup[2] == 0)
			{
				u16Len = Minimum(u16Len, 4);
				/* Copy to USB SRAM. */
      			DrvUSB_DataIn(0, gau8StringLang, u16Len);
			}
			else
			{
				/* Get String Descriptor */
				switch (pInfraDevice->au8Setup[2])
				{
					case 1:
					{
						u16Len = Minimum(u16Len, gau8VendorStringDescriptor[0]);
						/* Copy to USB SRAM. */				 
						DrvUSB_DataIn(0, gau8VendorStringDescriptor, u16Len);
						break;
					}
					case 2:
					{
						u16Len = Minimum(u16Len, gau8ProductStringDescriptor[0]);
						/* Copy to USB SRAM. */
						DrvUSB_DataIn(0, gau8ProductStringDescriptor, u16Len);
						break;
					}
					case 3:
					{
						u16Len = Minimum(u16Len, gau8StringSerial[0]);
						/* Copy to USB SRAM. */
						DrvUSB_DataIn(0, gau8StringSerial, u16Len);
						break;
					}
					case 4:
					case 5:
					case 6:
					case 7:
					{
						u16Len = Minimum(u16Len, gau8StringAudio[0]);
						/* Copy to USB SRAM. */
						DrvUSB_DataIn(0,  gau8StringAudio, u16Len);
						break;
					}
					default:
						/* Not support. Reply STALL. */
						DrvUSB_ClrCtrlReadyAndTrigStall();
						break;
				}
			}
			break;
		}
		default:
			/* Not support. Reply STALL. */
			DrvUSB_ClrCtrlReadyAndTrigStall();
			break;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* FUNCTION                                                                                                */
/*      UVC_CtrlDataInGetDescriptor()	       	                                                           */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*      DATA IN ACK call back function for Get descriptor command                                          */
/*                                                                                                         */
/* INPUTS                                                                                                  */
/*      pVoid   A pointer of UVC device.                                                                   */
/*                                                                                                         */
/* OUTPUTS                                                                                                 */
/*      None                            				                                                   */
/*                                                                                                         */
/* RETURN                                                                                                  */
/*      None                                                                                               */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void UVC_CtrlDataInGetDescriptor(void * pVoid)
{
	S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;
	
	if (psDevice->au8Setup[0] & 0x80)
	{
		if (s_u8ConfigLenOver64  ==  1)  // Configure descriptor over 64
		{
			if (s_u16Len >= s_u16SendLength)
			{				
				DrvUSB_DataIn(0, gau8ConfigDescriptor + s_u16CurPtr, s_u16SendLength);
				s_u16Len -= s_u16SendLength;
				s_u16CurPtr += s_u16SendLength;
				
				if (s_u16Len == 0)
					_DRVUSB_TRIG_EP(1,0x00); //Ctl data out
			}
			else
			{			
				DrvUSB_DataIn(0, gau8ConfigDescriptor + s_u16CurPtr, s_u16Len);
				s_u8ConfigLenOver64 = 0;		
			}
		}
		else
		{
			//_UVC_TRIG_CTRL_OUT(0x00);
			_DRVUSB_TRIG_EP(1,0x00);
		}
	}
}


void UVC_ClassCtrlSetCurRequest(void * pVoid)
{
  	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
  	uint8_t *CmdDataPtr;
  	int i;
  	int value = 0; 
  	
  	if (UVC_CS_INTERFACE == psUsbDevice->au8Setup[4])
  	{
        switch ( psUsbDevice->au8Setup[3])
        {
            case VS_PROBE_CONTROL:
			case VS_COMMIT_CONTROL:
            	CmdDataPtr = (uint8_t *)&VideoStreamCmdCtlData;	
				    for (i = 0 ; i < psUsbDevice->au8Setup[6] ; i=i+1)     
				    {
				     	  value = psUsbDevice->sEpCrl[0].u8SramBuffer[i];
				        *CmdDataPtr++ = value;	                   	          
					}             
            
               _DRVUSB_TRIG_EP(1,psUsbDevice->au8Setup[6]); 
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }
    else
    {
        /* STALL control pipe */
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
    } 
}

void UVC_ClassCtrlGetCurRequest(void * pVoid)
{
  	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;
	uint8_t *ptr;
 	if(UVC_CS_INTERFACE == psUsbDevice->au8Setup[4])
  	{
        switch ( psUsbDevice->au8Setup[3])
        {

            case VS_PROBE_CONTROL:
			case VS_COMMIT_CONTROL:
			{
				switch(VideoStreamCmdCtlData.bFrameIndex)
				{
					case 2://160*120
			 			VideoStreamCmdCtlData.dwMaxVideoFrameSize = 0x00009600;
			    		break;			    		
					case 1://320*240
				    	VideoStreamCmdCtlData.dwMaxVideoFrameSize = 0x00025800;
				    	break;
					case 3://640*480
				    	VideoStreamCmdCtlData.dwMaxVideoFrameSize = 0x00096000;
				    	break;		    
				}
				//************Depend on the specificed frame*****************//  
				VideoStreamCmdCtlData.dwMaxPayloadTransferSize = 0x000040;//VideoStreamCmdCtlData.dwMaxVideoFrameSize+2; 			    							    
							     
				ptr = (uint8_t *)&VideoStreamCmdCtlData;					
				{
					int i;			
					for(i =0 ; i<26;i++)
      		    		psUsbDevice->sEpCrl[0].u8SramBuffer[i] = *ptr++;
	      		 }   
      		   
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 26);
				break;
            }
			default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else
    {
        /* STALL control pipe */
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
    } 
}


void UVC_ClassCtrlGetMinRequest(void * pVoid)
{
 	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;

 	if (REC_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16RecMinVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16RecMinVolume >> 8);  
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else if (PLAY_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16PlayMinVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16PlayMinVolume >> 8);  
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else
    {
        /* STALL control pipe */
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
    } 
}


void UVC_ClassCtrlGetMaxRequest(void * pVoid)
{
 	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;

 	if (REC_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16RecMaxVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16RecMaxVolume >> 8); 
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else if (PLAY_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16PlayMaxVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16PlayMaxVolume >> 8); 
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else
    {
        /* STALL control pipe */
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
    } 
}

void UVC_ClassCtrlGetResRequest(void * pVoid)
{
 	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;

 	if (REC_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16RecResVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16RecResVolume >> 8);  
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else if (PLAY_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        switch (psUsbDevice->au8Setup[3])
        {
            case VOLUME_CONTROL:
                psUsbDevice->sEpCrl[0].u8SramBuffer[0] = (uint8_t)(psDevice->i16PlayResVolume);    
                psUsbDevice->sEpCrl[0].u8SramBuffer[1] = (uint8_t)(psDevice->i16PlayResVolume >> 8);  
        		_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
        		_DRVUSB_TRIG_EP(0, 2);
               break;
            default:   
                /* STALL control pipe */
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
        		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
       	        break;
        }       	      
    }   	 
    else
    {
        /* STALL control pipe */
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(0);
		_DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(1);
    } 
}			

void UVC_ClassCtrlSetCurOutData(void * pVoid)
{
  	S_UVC_DEVICE *psDevice = (S_UVC_DEVICE *) pVoid;
  	S_DRVUSB_DEVICE *psUsbDevice = (S_DRVUSB_DEVICE *)psDevice->device;

 	if (REC_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        if (psUsbDevice->au8Setup[3] == MUTE_CONTROL)
        {
            psDevice->u8RecMute = psUsbDevice->sEpCrl[0].u8SramBuffer[0];
        }   
        else if(psUsbDevice->au8Setup[3] == VOLUME_CONTROL)
        {
            psDevice->i16RecVolume =(int16_t) (psUsbDevice->sEpCrl[0].u8SramBuffer[0] | (psUsbDevice->sEpCrl[0].u8SramBuffer[1]<<8));
        }
    }   	 
    else if(PLAY_FEATURE_UNITID == psUsbDevice->au8Setup[5])
  	{
        if (psUsbDevice->au8Setup[3] == MUTE_CONTROL)
        {
            psDevice->u8PlayMute = psUsbDevice->sEpCrl[0].u8SramBuffer[0];
        }   
        else if(psUsbDevice->au8Setup[3] == VOLUME_CONTROL)
        {
            if(psUsbDevice->au8Setup[2] == 1)
            {
                psDevice->i16PlayVolumeL =(int16_t) (psUsbDevice->sEpCrl[0].u8SramBuffer[0] | (psUsbDevice->sEpCrl[0].u8SramBuffer[1]<<8));
            }
            else
            {
                psDevice->i16PlayVolumeR =(int16_t) (psUsbDevice->sEpCrl[0].u8SramBuffer[0] | (psUsbDevice->sEpCrl[0].u8SramBuffer[1]<<8));
            }
        }
    }
	_DRVUSB_SET_EP_TOG_BIT(0, FALSE);
	_DRVUSB_TRIG_EP(0, 0);
}


/* Translate the volume gain form dB value to Q12 integer. Output data is in Q12 */
int32_t UVC_GetRecVolume(void)
{
    
    return 0;
}

/* Translate the volume gain form dB value to Q12 integer. Output data is in Q12 */
int32_t UVC_GetPlayVolume(int32_t bIsRight)
{
    int32_t volume;
    int32_t q, r;
    
    /* Just return 0 when MUTE */
    if (g_UVC_sDevice.u8PlayMute)
        return 0;
    
    if (bIsRight)
        volume = g_UVC_sDevice.i16PlayVolumeR;
    else        
        volume = g_UVC_sDevice.i16PlayVolumeL;
    
    
    if (volume == -32768)
    {   
        /* MUTE control */
        volume = 0;
    }
    else
    {   
        volume -= g_UVC_sDevice.i16RecMinVolume;
        q = volume >> (8 + 1);
        r = volume & 0x1FF;
        
        volume = gVolumeTbl[q] + ((gVolumeTbl[q+1] - gVolumeTbl[q]) * r >> 9);
    }
    
    return volume;
}

