/******************************************************************************
 * @file     UAC_Class.c
 * @brief    Nano1xx USB Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "nano1xx_i2c.h"
#include "nano1xx_i2s.h"
#include "nano1xx_usb.h"
#include "nano1xx_gpio.h"
#include "UAC_Class.h"

/* Define the vendor id and product id */
#define USB_VID		0x0416  /* Vendor ID */
#define USB_PID		0x1000  /* Product ID */

/* Define EP maximum packet size */
#define	UAC_MAX_PACKET_SIZE_EP0		64
#define MAX_PACKET_SIZE_ISO_OUT		PLAY_RATE*PLAY_CHANNELS*2/1000
#define MAX_PACKET_SIZE_ISO_IN		REC_RATE*REC_CHANNELS*2/1000

/* Define the interrupt In EP number */
#define ISO_IN_EP_NUM    0x01
#define ISO_OUT_EP_NUM   0x02

/* Define Descriptor information */
#if(PLAY_CHANNELS == 1)
#define PLAY_CH_CFG     1
#endif
#if(PLAY_CHANNELS == 2)
#define PLAY_CH_CFG     3
#endif

#if(REC_CHANNELS == 1)
#define REC_CH_CFG     1
#endif
#if(REC_CHANNELS == 2)
#define REC_CH_CFG     3
#endif

#define PLAY_RATE_LO    (PLAY_RATE & 0xFF)
#define PLAY_RATE_MD    ((PLAY_RATE >> 8) & 0xFF)
#define PLAY_RATE_HI    ((PLAY_RATE >> 16) & 0xFF)

#define REC_RATE_LO     (REC_RATE & 0xFF)
#define REC_RATE_MD     ((REC_RATE >> 8) & 0xFF)
#define REC_RATE_HI     ((REC_RATE >> 16) & 0xFF)


/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[] =
{
	LEN_DEVICE,		/* bLength */
	DESC_DEVICE,	/* bDescriptorType */
	0x10, 0x01,		/* bcdUSB */
	0x00,			/* bDeviceClass */
	0x00,			/* bDeviceSubClass */
	0x00,			/* bDeviceProtocol */
	UAC_MAX_PACKET_SIZE_EP0,	/* bMaxPacketSize0 */
	/* idVendor */
	USB_VID & 0x00FF,
	(USB_VID & 0xFF00) >> 8,
	/* idProduct */
	USB_PID & 0x00FF,
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x01,		/* bcdDevice */
	0x01, 			/* iManufacture */
	0x02, 			/* iProduct */
	0x00, 			/* iSerialNumber */
	0x01			/* bNumConfigurations */
};

/*!<USB Configure Descriptor */
const uint8_t USB_ConfigDescriptor[] =
{
	LEN_CONFIG,     /* bLength */
	DESC_CONFIG,    /* bDescriptorType */
	0xC0,0x00,      /* wTotalLength */
	0x03,           /* bNumInterfaces */
	0x01,           /* bConfigurationValue */
	0x00,           /* iConfiguration */
	0x80,           /* bmAttributes */
	0x20,           /* Max power */

	/* Standard AC inteface */
	0x09,           /* bLength */
	0x04,           /* bDescriptorType */
	0x00,           /* bInterfaceNumber */
	0x00,           /* bAlternateSetting */
	0x00,           /* bNumEndpoints */
	0x01,           /* bInterfaceClass:AUDIO */
	0x01,           /* bInterfaceSubClass:AUDIOCONTROL */
	0x00,           /* bInterfaceProtocol */
	0x00,           /* iInterface */

	/* Class-spec AC interface descriptor */
	0x0A,           /* bLength */
	0x24,           /* bDescriptorType:CS_INTERFACE */
	0x01,           /* bDescriptorSubType:HEADER */
	0x00,0x01,      /* bcdADC:1.0 */
	0x46,0x00,      /* wTotalLength */
	0x02,           /* bInCollection */
	0x01,           /* baInterfaceNr(1) */
	0x02,           /* baInterfaceNr(n) */
	
	/* TID 1: Input for usb streaming */
    0x0C,               /* bLength */
    0x24,               /* bDescriptorType:CS_INTERFACE */
    0x02,               /* bDescriptorSubType:INPUT_TERMINAL */
    0x01,               /* bTerminalID */
    0x01,0x01,          /* wTerminalType: 0x0101 usb streaming */
    0x00,               /* bAssocTerminal */
    PLAY_CHANNELS,      /* bNrChannels */
    PLAY_CH_CFG,0x00,   /* wChannelConfig */
    0x00,               /* iChannelNames */
    0x00,               /* iTerminal */

    /* UNIT ID 5: Feature Unit */
    0x08,               /* bLength */
    0x24,               /* bDescriptorType */
    0x06,               /* bDescriptorSubType */
    REC_FEATURE_UNITID, /* bUnitID */
    0x04,               /* bSourceID */
    0x01,               /* bControlSize */
    0x03,               /* bmaControls(0) */
    0x00,               /* iFeature */

	/* TID 2: Output Terminal for usb streaming */
	0x09,               /* bLength */
	0x24,               /* bDescriptorType:CS_INTERFACE */
	0x03,               /* bDescriptorSubType:OUTPUT_TERMINAL */
	0x02,               /* bTerminalID */
	0x01,0x01,          /* wTerminalType: 0x0101 usb streaming */
	0x00,               /* bAssocTerminal */
	REC_FEATURE_UNITID, /* bSourceID */
	0x00,               /* iTerminal */

    /* UNIT ID 6: Feature Unit */
    0x0A,               /* bLength */
    0x24,               /* bDescriptorType */
    0x06,               /* bDescriptorSubType */
    PLAY_FEATURE_UNITID, /* bUnitID */
    0x01,               /* bSourceID */
    0x01,               /* bControlSize */
    0x01,               /* bmaControls(0) */
    0x02,               /* bmaControls(0) */
    0x02,               /* bmaControls(0) */
    0x00,               /* iFeature */
	
	/* TID 3: Output for speaker */
    0x09,           /* bLength*/
    0x24,           /* bDescriptorType:CS_INTERFACE*/
    0x03,           /* bDescriptorSubType:OUTPUT_TERMINAL*/
    0x03,           /* bTerminalID*/
    0x01,0x03,      /* wTerminalType: 0x0301 speaker*/
    0x00,           /* bAssocTerminal*/
    0x06, /* bSourceID*/
    0x00,           /* iTerminal*/

	/* TID 4: Input Terminal for microphone */
	0x0C,               /* bLength */
	0x24,               /* bDescriptorType:CS_INTERFACE */
	0x02,               /* bDescriptorSubType:INPUT_TERMINAL*/
	0x04,               /* bTerminalID*/
	0x01,0x02,          /* wTerminalType: 0x0201 microphone*/
	0x00,               /* bAssocTerminal*/
	REC_CHANNELS,       /* bNrChannels*/
	REC_CH_CFG, 0x00,   /* wChannelConfig*/
	0x00,               /* iChannelNames*/
	0x00,               /* iTerminal*/

	/* Standard AS interface 1, alternate 0 */
	0x09,           /* bLength */
	0x04,           /* bDescriptorType */
	0x01,           /* bInterfaceNumber */
	0x00,           /* bAlternateSetting */
	0x00,           /* bNumEndpoints */
	0x01,           /* bInterfaceClass:AUDIO */
	0x02,           /* bInterfaceSubClass:AUDIOSTREAMING */
	0x00,           /* bInterfaceProtocol */
	0x00,           /* iInterface */                     
	
	/* Standard AS interface 1, alternate 1 */
	0x09,           /* bLength */
	0x04,           /* bDescriptorType */
	0x01,           /* bInterfaceNumber */
	0x01,           /* bAlternateSetting */
	0x01,           /* bNumEndpoints */
	0x01,           /* bInterfaceClass:AUDIO */
	0x02,           /* bInterfaceSubClass:AUDIOSTREAMING */
	0x00,           /* bInterfaceProtocol */
	0x00,           /* iInterface */
	
	/* Class-spec AS interface, this interface's endpoint connect to TID 0x02 */
	0x07,           /* bLength */
	0x24,           /* bDescriptorType:CS_INTERFACE */
	0x01,           /* bDescriptorSubType:AS_GENERAL */
	0x02,           /* bTernimalLink */
	0x01,           /* bDelay */
	0x01,0x00,      /* wFormatTag:0x0001 PCM */
	
	/* Type I format type Descriptor */
	0x0B,           /* bLength */
	0x24,           /* bDescriptorType:CS_INTERFACE */
	0x02,           /* bDescriptorSubType:FORMAT_TYPE */
	0x01,           /* bFormatType:FORMAT_TYPE_I */
	REC_CHANNELS,   /* bNrChannels */
	0x02,           /* bSubFrameSize */
	0x10,           /* bBitResolution */
	0x01,           /* bSamFreqType : 0 continuous; 1 discrete */
	REC_RATE_LO,
	REC_RATE_MD,
	REC_RATE_HI,    /* Sample Frequency */
	
	/* Standard AS ISO Audio Data Endpoint */
	0x09,                       /* bLength */
	0x05,                       /* bDescriptorType */
	ISO_IN_EP_NUM | EP_INPUT,   /* bEndpointAddress */
	0x0d,                       /* bmAttributes */
	MAX_PACKET_SIZE_ISO_IN,0x00,          /* wMaxPacketSize*/
	0x01,                       /* bInterval*/
	0x00,                       /* bRefresh*/
	0x00,                       /* bSynchAddress*/
	
	/* Class-spec AS ISO Audio Data endpoint Descriptor */
	0x07,           /* bLength */
	0x25,           /* bDescriptorType:CS_ENDPOINT */
	0x01,           /* bDescriptorSubType:EP_GENERAL */
	0x00,           /* bmAttributes */
	0x00,           /* bLockDelayUnits */
	0x00, 0x00,     /* wLockDelay */

 	/* Standard AS interface 2, alternate 0 */
    0x09,           /* bLength */
    0x04,           /* bDescriptorType */
    0x02,           /* bInterfaceNumber */
    0x00,           /* bAlternateSetting */
    0x00,           /* bNumEndpoints */
    0x01,           /* bInterfaceClass:AUDIO */
    0x02,           /* bInterfaceSubClass:AUDIOSTREAMING */
    0x00,           /* bInterfaceProtocol */
    0x00,           /* iInterface */

    /* Standard AS interface 2, alternate 1 */
    0x09,           /* bLength */
    0x04,           /* bDescriptorType */
    0x02,           /* bInterfaceNumber */
    0x01,           /* bAlternateSetting */
    0x01,           /* bNumEndpoints */
    0x01,           /* bInterfaceClass:AUDIO */
    0x02,           /* bInterfaceSubClass:AUDIOSTREAMING */
    0x00,           /* bInterfaceProtocol */
	0x00,           /* iInterface */
	
    /* Class-spec AS inf this interface's endpoint connect to TID 0x01 */
    0x07,           /* bLength */
    0x24,           /* bDescriptorType:CS_INTERFACE */
    0x01,           /* bDescriptorSubType:AS_GENERAL */
    0x01,           /* bTernimalLink */
    0x01,           /* bDelay */
    0x01,0x00,      /* wFormatTag:0x0001 PCM */   

    /* Type I format type Descriptor */
    0x0B,           /* bLength */
    0x24,           /* bDescriptorType:CS_INTERFACE */
    0x02,           /* bDescriptorSubType:FORMAT_TYPE */
    0x01,           /* bFormatType:FORMAT_TYPE_I */
    PLAY_CHANNELS,  /* bNrChannels */
    0x02,           /* bSubFrameSize */
    0x10,           /* bBitResolution */
    0x01,           /* bSamFreqType : 0 continuous; 1 discrete */
    PLAY_RATE_LO,
    PLAY_RATE_MD,
    PLAY_RATE_HI,   /* Sample Frequency */
    
    /* Standard AS ISO Audio Data Endpoint, output, addtess 2, Max 0x40 */
    0x09,                       /* bLength */
    0x05,                       /* bDescriptorType */
    ISO_OUT_EP_NUM | EP_OUTPUT, /* bEndpointAddress */
    0x0d,                       /* bmAttributes */
    MAX_PACKET_SIZE_ISO_OUT, 0x00,        /* wMaxPacketSize */
    0x01,                       /* bInterval */
    0x00,                       /* bRefresh */
    0x00,                       /* bSynchAddress */
       
    /* Class-spec AS ISO Audio Data endpoint Descriptor */
    0x07,           /* bLength */
    0x25,           /* bDescriptorType:CS_ENDPOINT */
    0x01,           /* bDescriptorSubType:EP_GENERAL */
    0x80,           /* bmAttributes */
    0x00,           /* bLockDelayUnits */
    0x00, 0x00,     /* wLockDelay */
};

/*!<USB Language String Descriptor */
const uint8_t USB_StringLang[4] =
{
	4,				/* bLength */
	DESC_STRING,	/* bDescriptorType */
	0x09, 0x04
};

/*!<USB Vendor String Descriptor */
const uint8_t USB_VendorStringDesc[] = 
{
	16,
	DESC_STRING,
	'N', 0, 'u', 0, 'v', 0, 'o', 0, 't', 0, 'o', 0, 'n', 0
};

/*!<USB Product String Descriptor */
const uint8_t USB_ProductStringDesc[] = {
	20,
	DESC_STRING,
	'U', 0, 'S', 0, 'B', 0, ' ', 0, 'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0
};
 
/*********************************************************/
/**   
  * @brief  USB_ProcessDescriptor, Process USB Descriptor.
  * @param  None.
  * @retval 1: Standard Request.
  *         0: has some error.
  */
uint32_t USB_ProcessDescriptor(uint8_t *pCtrl, uint32_t CtrlMaxPacketSize)
{
	gUsbCtrl.ReqLen = gUsbCtrl.UsbSetupBuf[6] + ((uint32_t)gUsbCtrl.UsbSetupBuf[7] << 8);
	USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
            
	switch (gUsbCtrl.UsbSetupBuf[3])
	{
		/* Get Device Descriptor */
		case DESC_DEVICE:
		{
			gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, LEN_DEVICE);
	
			if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
			{
				my_memcpy(pCtrl, (void *)USB_DeviceDescriptor, CtrlMaxPacketSize);
				gUsbCtrl.pDesc = (uint8_t *)&USB_DeviceDescriptor[CtrlMaxPacketSize];
				gUsbCtrl.EP0_Toggle = 1;
				USBD->MXPLD0 = CtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
				gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
			}
			else
			{
				my_memcpy(pCtrl, (void *)USB_DeviceDescriptor, gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			}
			USBD->MXPLD1 = 0;
			break;
		}
		/* Get Configuration Descriptor */
		case DESC_CONFIG:
		{
			gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, (USB_ConfigDescriptor[3]<<8)|USB_ConfigDescriptor[2]);
			if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
			{
				my_memcpy(pCtrl, (void *)USB_ConfigDescriptor, CtrlMaxPacketSize);
				gUsbCtrl.pDesc = (uint8_t *)&USB_ConfigDescriptor[CtrlMaxPacketSize];
				gUsbCtrl.EP0_Toggle = 1;
				USBD->MXPLD0 = CtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
				gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
			}
			else
			{
				my_memcpy(pCtrl, (void *)USB_ConfigDescriptor, gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			}
			break;
		}
		/* Get String Descriptor */
		case DESC_STRING:
		{
			/* Get Language */
			if(gUsbCtrl.UsbSetupBuf[2] == 0)
			{
				gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, USB_StringLang[0]);
				my_memcpy(pCtrl, (void *)USB_StringLang, gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				gUsbCtrl.StateFlag = FLAG_OUT_ACK;
				break;
			}
			/* Get Vendor String Descriptor */
			else if (gUsbCtrl.UsbSetupBuf[2] == 1)
			{
				gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, USB_VendorStringDesc[0]);
				if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
				{
					my_memcpy(pCtrl, (void *)USB_VendorStringDesc, CtrlMaxPacketSize);
					gUsbCtrl.pDesc = (uint8_t *)&USB_VendorStringDesc[CtrlMaxPacketSize];
					gUsbCtrl.EP0_Toggle = 1;
					USBD->MXPLD0 = CtrlMaxPacketSize;
					gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
					gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
				}
				else
				{
					my_memcpy(pCtrl, (void *)USB_VendorStringDesc, gUsbCtrl.ReqLen);
					USBD->MXPLD0 = gUsbCtrl.ReqLen;
					gUsbCtrl.StateFlag = FLAG_OUT_ACK;
				}
				break;
			}
			/* Get Product String Descriptor */
			else if (gUsbCtrl.UsbSetupBuf[2] == 2)
			{
				gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, USB_ProductStringDesc[0]);
				if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
				{
					my_memcpy(pCtrl, (void *)USB_ProductStringDesc, CtrlMaxPacketSize);
					gUsbCtrl.pDesc = (uint8_t *)&USB_ProductStringDesc[CtrlMaxPacketSize];
					gUsbCtrl.EP0_Toggle = 1;
					USBD->MXPLD0 = CtrlMaxPacketSize;
					gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
					gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
				}
				else
				{
					my_memcpy(pCtrl, (void *)USB_ProductStringDesc, gUsbCtrl.ReqLen);
					USBD->MXPLD0 = gUsbCtrl.ReqLen;
					gUsbCtrl.StateFlag = FLAG_OUT_ACK;
				}
				break;
			}
		}
		/* Get HID Report Descriptor */
		case DESC_HID_RPT:
		{
		}
		default:
		    /* Setup error, stall the device */
			USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
			USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			return FALSE;
	}
	return TRUE;
}

/**   
  * @brief  USB_ClassRequest, Process USB Class Request.
  * @param  None.
  * @retval 1: Standard Request.
  *         0: has some error.
  */
uint32_t USB_ClassRequest(void)
{
	uint8_t *ptr;

	/* get control pipe SRAM buffer */
	ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0);

	switch(gUsbCtrl.UsbSetupBuf[1])
	{
		case UAC_SET_CUR:
		{
			if (REC_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch ( gUsbCtrl.UsbSetupBuf[3])
				{
					case MUTE_CONTROL:
					case VOLUME_CONTROL:
						USBD->MXPLD1 = gUsbCtrl.UsbSetupBuf[6];
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else if (PLAY_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch ( gUsbCtrl.UsbSetupBuf[3])
				{
					case MUTE_CONTROL:
					case VOLUME_CONTROL:
						USBD->MXPLD1 = gUsbCtrl.UsbSetupBuf[6];
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else
			{
				/* STALL control pipe */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}
			gUsbCtrl.StateFlag = FLAG_UAC_SET_CUR;
			return TRUE;
		}

		case UAC_GET_CUR:
		{
			if(REC_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch ( gUsbCtrl.UsbSetupBuf[3])
				{
					case MUTE_CONTROL:
					{
						*ptr = gUacInfo.RecMute;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 1;
						break;
					}
					case VOLUME_CONTROL:
					{
						*ptr = (uint8_t)gUacInfo.RecVolume;
						*(ptr+1) = (uint8_t)gUacInfo.RecVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					}
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else if (PLAY_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch ( gUsbCtrl.UsbSetupBuf[3])
				{
					case MUTE_CONTROL:
					{
						*ptr = gUacInfo.PlayMute;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 1;
						break;
					}
					case VOLUME_CONTROL:
					{
						if(gUsbCtrl.UsbSetupBuf[2] == 1)
						{
							*ptr = (uint8_t)gUacInfo.PlayVolumeL;
							*(ptr+1) = (uint8_t)gUacInfo.PlayVolumeL >> 8;
						}
						else
						{
							*ptr = (uint8_t)gUacInfo.PlayVolumeR;
							*(ptr+1) = (uint8_t)gUacInfo.PlayVolumeR >> 8;
						}
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					}
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else
			{
				/* STALL control pipe */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}
			// Trigger next Control Out DATA1 Transaction.
			USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD1 = 0;
			return TRUE;
		}

		case UAC_GET_MIN:
		{
			if (REC_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.RecMinVolume;
						*(ptr+1) = (uint8_t)gUacInfo.RecMinVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else if (PLAY_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.PlayMinVolume;
						*(ptr+1) = (uint8_t)gUacInfo.PlayMinVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else
			{
				/* STALL control pipe */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}
			// Trigger next Control Out DATA1 Transaction.
			USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD1 = 0;
			return TRUE;
		}

		case UAC_GET_MAX:
		{
			if (REC_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.RecMaxVolume;
						*(ptr+1) = (uint8_t)gUacInfo.RecMaxVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else if (PLAY_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.PlayMaxVolume;
						*(ptr+1) = (uint8_t)gUacInfo.PlayMaxVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else
			{
				/* STALL control pipe */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}
			// Trigger next Control Out DATA1 Transaction.
			USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD1 = 0;
			return TRUE;
		}

		case UAC_GET_RES:
		{
			if (REC_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.RecResVolume;
						*(ptr+1) = (uint8_t)gUacInfo.RecResVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else if (PLAY_FEATURE_UNITID == gUsbCtrl.UsbSetupBuf[5])
			{
				switch (gUsbCtrl.UsbSetupBuf[3])
				{
					case VOLUME_CONTROL:
						*ptr = (uint8_t)gUacInfo.PlayResVolume;
						*(ptr+1) = (uint8_t)gUacInfo.PlayResVolume >> 8;
						USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD0 = 2;
						break;
					default:
						/* STALL control pipe */
						USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
						USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
						break;
				}
			}
			else
			{
				/* STALL control pipe */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}
			// Trigger next Control Out DATA1 Transaction.
			USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD1 = 0;
			return TRUE;
		}

		default:
		    /* Setup error, stall the device */
			USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
			USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
	}
	return FALSE;
}

/**   
  * @brief  USB_EpAck0, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck0(void)
{
	/* ISO IN transfer ACK */
	if (gUacInfo.AudioState == UAC_START_AUDIO_RECORD)
	{
		UAC_DeviceEnable(UAC_MICROPHONE);
		gUacInfo.AudioState = UAC_PROCESSING_AUDIO_RECORD;
	}
	else if (gUacInfo.AudioState == UAC_PROCESSING_AUDIO_RECORD)
		gUacInfo.AudioState = UAC_BUSY_AUDIO_RECORD;

	if (gUacInfo.AudioState == UAC_BUSY_AUDIO_RECORD)
		SendRecData();
	else
		USBD->MXPLD2 = 0;
	return 0;
}

/**   
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
	/* ISO OUT transfer ACK */
	GetPlayData((int16_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG3), (int16_t)USBD->MXPLD3);

	USBD->MXPLD3 = MAX_PACKET_SIZE_ISO_OUT;
	return 0;
}


/**   
  * @brief  USB_EpAck2, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck2(void)
{
	return 0;
}


/**   
  * @brief  USB_EpAck3, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck3(void)
{
	return 0;
}


/**   
  * @brief  USB_EpAck4, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck4(void)
{
	return 0;
}


/**   
  * @brief  USB_EpAck5, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck5(void)
{
	return 0;
}


/**   
  * @brief  USB_DeviceStart, Start USB Device Transfer.
  * @param  None.
  * @retval None.
  */
void USB_DeviceStart(void)
{
}

/*********************************************************/
/* For I2C transfer */
uint32_t __IO EndFlag0 = 0;
uint8_t Device_Addr0 = 0x1A;	 			/* WAU8822 Device ID */
uint8_t Tx_Data0[2];
uint8_t DataCnt0;

/* Recoder Buffer and its pointer */
uint16_t PcmRecBuff[BUFF_LEN] = {0};
uint32_t u32RecPos_Out = 0;
uint32_t u32RecPos_In = 0;

/* Player Buffer and its pointer */
uint32_t PcmPlayBuff[BUFF_LEN] = {0};
uint32_t u32PlayPos_Out = 0;
uint32_t u32PlayPos_In = 0;


/*********************************************************/
static void RoughDelay(uint32_t t)
{
    volatile int32_t delay;

    delay = t;

    while(delay-- >= 0);
}

/**   
  * @brief  I2C_WriteWAU8822, write 9-bit data to 7-bit address register of WAU8822 with I2C interface.
  * @param  None.
  * @retval None.
  */
static void I2C_WriteWAU8822(uint8_t u8addr, uint16_t u16data)
{		
	DataCnt0 = 0;
	EndFlag0 = 0;
	
	Tx_Data0[0] = (uint8_t)((u8addr << 1) | (u16data >> 8));
	Tx_Data0[1] = (uint8_t)(u16data & 0x00FF);
			
	/* I2C0 as master sends START signal */
	I2C_Ctrl(I2C0, 1, 0, 0);
		
	/* Wait I2C0 Tx Finish */
	while (EndFlag0 == 0);
}


/**   
  * @brief  WAU8822_Setup, configure CODEC WAU8822 with I2C interface.
  * @param  None.
  * @retval None.
  */
static void WAU8822_Setup(void)
{
	I2C_WriteWAU8822(0,  0x000);   /* Reset all registers */ 
	RoughDelay(0x200);
		
	I2C_WriteWAU8822(1,  0x02F);        
	I2C_WriteWAU8822(2,  0x1B3);   /* Enable L/R Headphone, ADC Mix/Boost, ADC */
	I2C_WriteWAU8822(3,  0x07F);   /* Enable L/R main mixer, DAC */			
	I2C_WriteWAU8822(4,  0x010);   /* 16-bit word length, I2S format, Stereo */			
	I2C_WriteWAU8822(5,  0x000);   /* Companding control and loop back mode (all disable) */
	
	I2C_WriteWAU8822(6,  0x1AD);   /* Divide by 6, 16K */
	I2C_WriteWAU8822(7,  0x006);   /* 16K for internal filter cofficients */

	I2C_WriteWAU8822(10, 0x008);   /* DAC softmute is disabled, DAC oversampling rate is 128x */
	I2C_WriteWAU8822(14, 0x108);   /* ADC HP filter is disabled, ADC oversampling rate is 128x */
	I2C_WriteWAU8822(15, 0x1EF);   /* ADC left digital volume control */
	I2C_WriteWAU8822(16, 0x1EF);   /* ADC right digital volume control */
		
	I2C_WriteWAU8822(44, 0x000);   /* LLIN/RLIN is not connected to PGA */
	I2C_WriteWAU8822(47, 0x050);   /* LLIN connected, and its Gain value */
	I2C_WriteWAU8822(48, 0x050);   /* RLIN connected, and its Gain value */

	I2C_WriteWAU8822(50, 0x001);   /* Left DAC connected to LMIX */
	I2C_WriteWAU8822(51, 0x001);   /* Right DAC connected to RMIX */

	I2C_WriteWAU8822(54, 0x133);   /* LSPKOUT Volume */
	I2C_WriteWAU8822(55, 0x133);   /* RSPKOUT Volume */

	/* To reduce audio output with pop */
	GPIO_Open(GPIOE, GPIO_PMD_PMD14_OUTPUT, GPIO_PMD_PMD14_MASK);
	GPIO_ClrBit(GPIOE, 14);
}


/**   
  * @brief  SendRecData, prepare the record data for next ISO transfer.
  * @param  None.
  * @retval None.
  */
void SendRecData(void)
{
	uint16_t *pBuff;

	if ((u32RecPos_Out < u32RecPos_In - REC_LEN) || (u32RecPos_Out > u32RecPos_In))
	{
		pBuff = &PcmRecBuff[u32RecPos_Out];

		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)pBuff, REC_LEN * 2);
		USBD->MXPLD2 = REC_LEN * 2;

		u32RecPos_Out += REC_LEN;

		if (u32RecPos_Out >= BUFF_LEN)
			u32RecPos_Out = 0;
	}
	else
		USBD->MXPLD2 = 0;
}


/**   
  * @brief  UAC_DeviceEnable. To enable the device to play or record audio data.
  * @param  u8Object: To select the device, UAC_MICROPHONE or UAC_SPEAKER.
  * @retval None.
  */
void UAC_DeviceEnable(uint8_t u8Object)
{
	if (u8Object == UAC_MICROPHONE)
	{
		/* Enable record hardware */
		I2S_EnableInt_RxThreshold();
		I2S_EnableRx();
	}
	else
	{
		/* Eanble play hardware */
		I2S_EnableInt_TxThreshold();	
		I2S_EnableTx();
	}
}


/**   
  * @brief  UAC_DeviceDisable. To disable the device to play or record audio data.
  * @param  u8Object: To select the device, UAC_MICROPHONE or UAC_SPEAKER.
  * @retval None.
  */
void UAC_DeviceDisable(uint8_t u8Object)
{
	if (u8Object ==  UAC_MICROPHONE )
	{
		/* Disable record hardware/stop record */
		I2S_DisableInt_RxThreshold();
		I2S_DisableRx();
	}
	else
	{
		/* Disable play hardware/stop play */
		I2S_DisableInt_TxThreshold();	
		I2S_DisableTx();
	}
}


/**   
  * @brief  GetPlayData, To get data from ISO OUT to play buffer.
  * @param  pi16src: The data buffer of ISO OUT.
  *         i16Samples: The sample number in data buffer.
  * @retval None.
  */
void GetPlayData(int16_t *pi16src, int16_t i16Samples)
{
	/* Get play data from ISO out packets */
	uint32_t u32len, i;
	uint32_t *pBuff;
	uint32_t *pSrc;

	u32len = PLAY_RATE/1000;

	pBuff = &PcmPlayBuff[u32PlayPos_In];
	pSrc = (uint32_t *) pi16src;

	for ( i = 0; i < u32len; i++ )
	{
		pBuff[i] = pSrc[i];
	}

	u32PlayPos_In += u32len;

	if (u32PlayPos_In >= BUFF_LEN)
	{
		u32PlayPos_In =	0;
	}
}


/**************************************************/
/**   
  * @brief  UAC_MainProcess, Audio Class main process.
  * @param  None.
  * @retval None.
  */

int32_t UAC_MainProcess(void)
{
	STR_USBD_T sParam;
	I2S_DATA_T st;

	/* Initial GPIO */
	GPIO_Init();
	/* Tri-state for FS and BCLK of CODEC */
	GPIO_Open(GPIOC, GPIO_PMD_PMD0_OPEN_DRAIN, GPIO_PMD_PMD0_MASK);
	GPIO_Open(GPIOC, GPIO_PMD_PMD1_OPEN_DRAIN, GPIO_PMD_PMD1_MASK);
	GPIO_SetBit(GPIOC, 0);
	GPIO_SetBit(GPIOC, 1);

	/* Set I2C0 I/O multi-function pin */
	GCR->PA_H_MFP = GCR->PA_H_MFP & ~(PA8_MFP_MASK|PA9_MFP_MASK) | 
					PA8_MFP_I2C0_SDA | PA9_MFP_I2C0_SCL;
	
	I2C_Init(I2C0);
	
	/* Open I2C0, and set clock = 100Kbps */
	I2C_Open(I2C0, 100000);	
	
	/* Enable I2C0 interrupt and set corresponding NVIC bit */
	I2C_EnableInt(I2C0);
	
	/* Configure I2S */
	st.u32SampleRate 	 = AUDIO_RATE;

	if (AUDIO_BIT_RATE == 16)
		st.u32WordWidth 	 = I2S_DATABIT_16;
	else if (AUDIO_BIT_RATE == 8)
		st.u32WordWidth 	 = I2S_DATABIT_8;

	if (AUDIO_CHANNELS == 2)
		st.u32AudioFormat = I2S_STEREO;
	else /* AUDIO_CHANNELS = 1 */
		st.u32AudioFormat = I2S_MONO;

	st.u32DataFormat  	 = I2S_FORMAT_I2S;   
    st.u32Mode 		 	 = I2S_MODE_SLAVE;
    st.u32TxFIFOThreshold = I2S_FIFO_TX_LEVEL_WORD_4;
    st.u32RxFIFOThreshold = I2S_FIFO_RX_LEVEL_WORD_4;
	
	I2S_Init();	
	I2S_Open(&st);
	
	/* Configure CODEC */
	WAU8822_Setup();

	/* Set I2S I/O multi-function pin */
	/* GPC0, GPC1, GPC2, GPC3, GPA15 */
	MFP_I2S_TO_PORTC();

	/* Set MCLK and enable MCLK */
	I2S_SetMCLKFreq(12000000);	
	I2S_EnableMCLK(); 
 
	/* Initial USB */
	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_AUDIO;
	sParam.u32CtrlMaxPacketSize = UAC_MAX_PACKET_SIZE_EP0;
	sParam.u32EndpointCount = 2;
	sParam.EP[0].u32EpNum = ISO_IN_EP_NUM;
	sParam.EP[0].u32MaxPacketSize = MAX_PACKET_SIZE_ISO_IN;
	sParam.EP[0].IsDirIN = TRUE;
	sParam.EP[0].IsIsoTransfer = TRUE;
	sParam.EP[1].u32EpNum = ISO_OUT_EP_NUM;
	sParam.EP[1].u32MaxPacketSize = MAX_PACKET_SIZE_ISO_OUT;
	sParam.EP[1].IsDirIN = FALSE;
	sParam.EP[1].IsIsoTransfer = TRUE;
	USB_Open(&sParam);

	while(1)
	{
	}
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

