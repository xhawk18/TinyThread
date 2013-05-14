/******************************************************************************
 * @file     CDC_Class.c
 * @brief    Nano1xx USB Sample code
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "nano1xx_uart.h"
#include "nano1xx_usb.h"
#include "nano1xx_lcd.h"
#include "CDC_Class.h"


/* Define the vendor id and product id */
#define USB_VID		0x0416
#define USB_PID		0x5011

/* Define EP maximum packet size */
#define	MAX_PACKET_SIZE_CTRL        64
#define MAX_PACKET_SIZE_BULK		64
#define MAX_PACKET_SIZE_INT			8

/* Define the interrupt In EP number */
#define BULK_IN_EP_NUM      1
#define BULK_OUT_EP_NUM     2
#define INT_IN_EP_NUM       3   


/********************* Global Variables **********************/
STR_USBD_VCOM_T gVcomInfo;

static uint8_t u8KeyValue=0;
uint8_t Inflag = 0;

/********************************************************/
/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[] =
{
	LEN_DEVICE,		/* bLength              */
	DESC_DEVICE,	/* bDescriptorType      */
	0x00, 0x02,		/* bcdUSB               */
	0x02,			/* bDeviceClass         */
	0x00,			/* bDeviceSubClass      */
	0x00,			/* bDeviceProtocol      */
	MAX_PACKET_SIZE_CTRL,	/* bMaxPacketSize0 */
	/* idVendor */
	USB_VID & 0x00FF,
	(USB_VID & 0xFF00) >> 8,
	/* idProduct */
	USB_PID & 0x00FF,
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x03,		/* bcdDevice            */
	0x01,			/* iManufacture         */
	0x02,			/* iProduct             */
	0x00,			/* iSerialNumber        */
	0x01			/* bNumConfigurations   */
};


/*!<USB Configure Descriptor */
const uint8_t USB_ConfigDescriptor[] =
{
	LEN_CONFIG,		/* bLength              */
	DESC_CONFIG,	/* bDescriptorType      */
	0x43, 0x00,		/* wTotalLength         */
	0x02,			/* bNumInterfaces       */
	0x01,			/* bConfigurationValue  */
	0x00,			/* iConfiguration       */
	0xC0,			/* bmAttributes         */
	0x32,			/* MaxPower             */

	/* INTERFACE descriptor */
	LEN_INTERFACE,	/* bLength              */
	DESC_INTERFACE,	/* bDescriptorType      */
	0x00,			/* bInterfaceNumber     */
	0x00,			/* bAlternateSetting    */
	0x01,			/* bNumEndpoints        */
	0x02,			/* bInterfaceClass      */
	0x02,			/* bInterfaceSubClass   */
	0x01,			/* bInterfaceProtocol   */
	0x00,			/* iInterface           */

	/* Communication Class Specified INTERFACE descriptor */
    0x05,           /* Size of the descriptor, in bytes */
    0x24,           /* CS_INTERFACE descriptor type */
    0x00,           /* Header functional descriptor subtype */
    0x10, 0x01,     /* Communication device compliant to the communication spec. ver. 1.10 */
    
	/* Communication Class Specified INTERFACE descriptor */
    0x05,           /* Size of the descriptor, in bytes */
    0x24,           /* CS_INTERFACE descriptor type */
    0x01,           /* Call management functional descriptor */
    0x00,           /* BIT0: Whether device handle call management itself. */
                    /* BIT1: Whether device can send/receive call management information over a Data Class Interface 0 */
    0x01,           /* Interface number of data class interface optionally used for call management */

	/* Communication Class Specified INTERFACE descriptor */
    0x04,           /* Size of the descriptor, in bytes */
    0x24,           /* CS_INTERFACE descriptor type */
    0x02,           /* Abstract control management funcational descriptor subtype */
    0x00,           /* bmCapabilities       */
    
	/* Communication Class Specified INTERFACE descriptor */
    0x05,           /* bLength              */
    0x24,           /* bDescriptorType: CS_INTERFACE descriptor type */
    0x06,           /* bDescriptorSubType   */
    0x00,           /* bMasterInterface     */
    0x01,           /* bSlaveInterface0     */
    
	/* ENDPOINT descriptor */
	LEN_ENDPOINT,	                /* bLength          */
	DESC_ENDPOINT,	                /* bDescriptorType  */
	(EP_INPUT | INT_IN_EP_NUM),     /* bEndpointAddress */
	EP_INT,		                    /* bmAttributes     */
	MAX_PACKET_SIZE_INT, 0x00,	    /* wMaxPacketSize   */
	0x01,	                        /* bInterval        */
			
	/* INTERFACE descriptor */
	LEN_INTERFACE,	/* bLength              */
	DESC_INTERFACE,	/* bDescriptorType      */
	0x01,			/* bInterfaceNumber     */
	0x00,			/* bAlternateSetting    */
	0x02,			/* bNumEndpoints        */
	0x0A,			/* bInterfaceClass      */
	0x00,			/* bInterfaceSubClass   */
	0x00,			/* bInterfaceProtocol   */
	0x00,			/* iInterface           */
			
	/* ENDPOINT descriptor */
	LEN_ENDPOINT,	                /* bLength          */
	DESC_ENDPOINT,	                /* bDescriptorType  */
	(EP_INPUT | BULK_IN_EP_NUM),	/* bEndpointAddress */
	EP_BULK,		                /* bmAttributes     */
	MAX_PACKET_SIZE_BULK, 0x00,	    /* wMaxPacketSize   */
	0x00,			                /* bInterval        */

	/* ENDPOINT descriptor */
	LEN_ENDPOINT,	                /* bLength          */
	DESC_ENDPOINT,	                /* bDescriptorType  */
	(EP_OUTPUT | BULK_OUT_EP_NUM),	/* bEndpointAddress */
	EP_BULK,		                /* bmAttributes     */
	MAX_PACKET_SIZE_BULK, 0x00,     /* wMaxPacketSize   */
	0x00,			                /* bInterval        */
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
const uint8_t USB_ProductStringDesc[] =
{
	32,             /* bLength          */
	DESC_STRING,    /* bDescriptorType  */
	'U', 0, 'S', 0,	'B', 0,	' ', 0,	'V', 0,	'i', 0,	'r', 0,	't', 0,	'u', 0,	'a', 0,	'l', 0,	
	' ', 0,	'C', 0,	'O', 0,	'M', 0
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
	switch(gUsbCtrl.UsbSetupBuf[1])
	{
		case SET_LINE_CODE:
		{
			/* Ready to get next Ctrl out */
			USBD->MXPLD1 = 7;
			USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
			gUsbCtrl.StateFlag = FLAG_SET_LINE_CODE;
			return TRUE;
		}

		case GET_LINE_CODE:
		{
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0), (void *)&gCdcInfo, 7);
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 7;
			gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			return TRUE;
		}

		case SET_CONTROL_LINE_STATE:
		{
			gVcomInfo.CtrlSignal = gUsbCtrl.UsbSetupBuf[3];
			gVcomInfo.CtrlSignal = (gVcomInfo.CtrlSignal << 8) | gUsbCtrl.UsbSetupBuf[2];

			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
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
	/* Bulk IN */
	if (Inflag == 1)
	{
		u8KeyValue += 0x30;
		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)&u8KeyValue, 1);
		USBD->MXPLD2 = 1;
		Inflag = 0;
	}
	else
	{
		USBD->MXPLD2 = 0;
	}

	return 0;
}

/**   
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
	char KeyValue[5];   	
	uint8_t u8Cmd;

	/* Bulk OUT */
	KeyValue[0] = 'K';
	KeyValue[1] = 'E';
	KeyValue[2] = 'Y';
	my_memcpy(&u8Cmd, (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG3), sizeof(u8Cmd));

	Inflag = 1;
    switch (u8Cmd)
    {
    case '9':
		u8KeyValue = 9;
		KeyValue[3] = '9';
		LCD_Write(&KeyValue[0]);
		break;
    case '1':
		u8KeyValue = 1;
		KeyValue[3] = '1';
		LCD_Write(&KeyValue[0]);
		break;
    case '2':
		u8KeyValue = 2;
		KeyValue[3] = '2';
		LCD_Write(&KeyValue[0]);
		break;
    case '3':
		u8KeyValue = 3;
		KeyValue[3] = '3';
		LCD_Write(&KeyValue[0]);
		break;
    case '4':
		u8KeyValue = 4;
		KeyValue[3] = '4';
		LCD_Write(&KeyValue[0]);
		break;
    case '0':
		u8KeyValue = 0;
		KeyValue[3] = '0';
		LCD_Write(&KeyValue[0]);
		break;
    case '5':
		u8KeyValue = 5;
		KeyValue[3] = '5';
		LCD_Write(&KeyValue[0]);
		break;
    case '6':
		u8KeyValue = 6;
		KeyValue[3] = '6';
		LCD_Write(&KeyValue[0]);
		break;
    case '7':
		u8KeyValue = 7;
		KeyValue[3] = '7';
		LCD_Write(&KeyValue[0]);
		break;
    case '8':
		u8KeyValue = 8;
		KeyValue[3] = '8';
		LCD_Write(&KeyValue[0]);
		break;
    default:
		LCD_AlphaNumberOff();
		break;
    }

	/* trigger next bulk out */
	USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
	return 0;
}


/**   
  * @brief  USB_EpAck2, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck2(void)
{
	/* Interrupt IN */
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
	USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
	USBD->MXPLD2 = 0;
}

/*********************************************************/
/**   
  * @brief  VCOM_MainProcess, VCOM main process.
  * @param  None.
  * @retval None.
  */
void VCOM_MainProcess(void)
{
	STR_USBD_T sParam;

	/* Initial USB */
	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_CDC;
	sParam.u32CtrlMaxPacketSize = MAX_PACKET_SIZE_CTRL;
	sParam.u32EndpointCount = 3;
	sParam.EP[0].u32EpNum = BULK_IN_EP_NUM;
	sParam.EP[0].u32MaxPacketSize = MAX_PACKET_SIZE_BULK;
	sParam.EP[0].IsDirIN = TRUE;
	sParam.EP[0].IsIsoTransfer = FALSE;
	sParam.EP[1].u32EpNum = BULK_OUT_EP_NUM;
	sParam.EP[1].u32MaxPacketSize = MAX_PACKET_SIZE_BULK;
	sParam.EP[1].IsDirIN = FALSE;
	sParam.EP[1].IsIsoTransfer = FALSE;
	sParam.EP[2].u32EpNum = INT_IN_EP_NUM;
	sParam.EP[2].u32MaxPacketSize = MAX_PACKET_SIZE_INT;
	sParam.EP[2].IsDirIN = TRUE;
	sParam.EP[2].IsIsoTransfer = FALSE;
	USB_Open(&sParam);

	memset((void *)&gVcomInfo, 0, sizeof(STR_USBD_VCOM_T));

	while(1)
	{
	}
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/




