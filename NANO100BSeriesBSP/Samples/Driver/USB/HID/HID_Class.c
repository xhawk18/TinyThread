/******************************************************************************
 * @file     HID_Class.c
 * @brief    Nano1xx USB Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_usb.h"
#include "HID_Class.h"

/* Define the vendor id and product id */
#define USB_VID		0x0416
#define USB_PID		0x8240

/* Define EP maximum packet size */
#define	HID_MAX_PACKET_SIZE_CTRL        8
#define HID_MAX_PACKET_SIZE_INT_IN		8

/* Define the interrupt In EP number */
#define INT_IN_EP_NUM	0x01

/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL	20
#define HID_IS_SELF_POWERED         0
#define HID_IS_REMOTE_WAKEUP        0
#define HID_MAX_POWER               50	/* The unit is in 2mA. ex: 50 * 2mA = 100mA */

#define HID_REPORT_SIZE				4	/* The unit is in bytes */

#define LEN_CONFIG_AND_SUBORDINATE (LEN_CONFIG+LEN_INTERFACE+LEN_HID+LEN_ENDPOINT)

signed char mouse_table[] = {-4, -4, -4, 0, 4, 4, 4, 0};
uint8_t mouse_idx = 0;
uint8_t move_len, mouse_mode;

/*!<USB HID Report Descriptor */
const uint8_t HID_MouseReportDescriptor[] =
{
		0x05, 0x01, 	/* Usage Page(Generic Desktop Controls) */
		0x09, 0x02, 	/* Usage(Mouse) */
		0xA1, 0x01, 	/* Collection(Application) */
		0x09, 0x01, 		/* Usage(Pointer) */
		0xA1, 0x00, 		/* Collection(Physical) */
		0x05, 0x09, 			/* Usage Page(Button) */
		0x19, 0x01, 			/* Usage Minimum(0x1) */
		0x29, 0x03, 			/* Usage Maximum(0x3) */
		0x15, 0x00, 			/* Logical Minimum(0x0) */
		0x25, 0x01, 			/* Logical Maximum(0x1) */
		0x75, 0x01, 			/* Report Size(0x1) */
		0x95, 0x03, 			/* Report Count(0x3) */
		0x81, 0x02, 			/* Input(3 button bit) */
		0x75, 0x05, 			/* Report Size(0x5) */
		0x95, 0x01, 			/* Report Count(0x1) */
		0x81, 0x01, 			/* Input(5 bit padding) */
		0x05, 0x01, 			/* Usage Page(Generic Desktop Controls) */
		0x09, 0x30, 			/* Usage(X) */
		0x09, 0x31, 			/* Usage(Y) */
		0x09, 0x38, 			/* Usage(Wheel) */
		0x15, 0x81, 			/* Logical Minimum(0x81)(-127) */
		0x25, 0x7F, 			/* Logical Maximum(0x7F)(127) */
		0x75, 0x08, 			/* Report Size(0x8) */
		0x95, 0x03, 			/* Report Count(0x3) */
		0x81, 0x06, 			/* Input(1 byte wheel) */
		0xC0, 				/* End Collection */
		0xC0, 			/* End Collection */
};

#define HID_MOUSE_REPORT_DESCRIPTOR_SIZE \
	sizeof (HID_MouseReportDescriptor) / sizeof(HID_MouseReportDescriptor[0])

#define HID_REPORT_DESCRIPTOR_SIZE HID_MOUSE_REPORT_DESCRIPTOR_SIZE

/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[] =
{
	LEN_DEVICE,		/* bLength */
	DESC_DEVICE,	/* bDescriptorType */
	0x10, 0x01,		/* bcdUSB */
	0x00,			/* bDeviceClass */
	0x00,			/* bDeviceSubClass */
	0x00,			/* bDeviceProtocol */
	HID_MAX_PACKET_SIZE_CTRL,	/* bMaxPacketSize0 */
	/* idVendor */
	USB_VID & 0x00FF,
	(USB_VID & 0xFF00) >> 8,
	/* idProduct */
	USB_PID & 0x00FF,
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x00,		/* bcdDevice */
	0x01,			/* iManufacture */
	0x02,			/* iProduct */
	0x00,			/* iSerialNumber - no serial */
	0x01			/* bNumConfigurations */
};

/*!<USB Configure Descriptor */
const uint8_t USB_ConfigDescriptor[] =
{
	LEN_CONFIG,		/* bLength */
	DESC_CONFIG,	/* bDescriptorType */
	/* wTotalLength */
	LEN_CONFIG_AND_SUBORDINATE & 0x00FF,
	(LEN_CONFIG_AND_SUBORDINATE & 0xFF00) >> 8,
	0x01,			/* bNumInterfaces */
	0x01,			/* bConfigurationValue */
	0x00,			/* iConfiguration */
	0x80 | (HID_IS_SELF_POWERED << 6) | (HID_IS_REMOTE_WAKEUP << 5),/* bmAttributes */
	HID_MAX_POWER,			/* MaxPower */

	/* I/F descr: HID */
	LEN_INTERFACE,	/* bLength */
	DESC_INTERFACE,	/* bDescriptorType */
	0x00,			/* bInterfaceNumber */
	0x00,			/* bAlternateSetting */
	0x01,			/* bNumEndpoints */
	0x03,			/* bInterfaceClass */
	0x01,			/* bInterfaceSubClass */
	0x02,			/* bInterfaceProtocol (mouse protocol) */
	0x00,			/* iInterface */

	/* HID Descriptor */
	LEN_HID,		/* Size of this descriptor in UINT8s. */
	DESC_HID,		/* HID descriptor type. */
	0x10, 0x01, 	/* HID Class Spec. release number. */
	0x00,			/* H/W target country. */
	0x01,			/* Number of HID class descriptors to follow. */
	DESC_HID_RPT,	/* Dscriptor type. */
	/* Total length of report descriptor. */
	HID_REPORT_DESCRIPTOR_SIZE & 0x00FF,
	(HID_REPORT_DESCRIPTOR_SIZE & 0xFF00) >> 8,

	/* EP Descriptor: interrupt in. */
	LEN_ENDPOINT,	/* bLength */
	DESC_ENDPOINT,	/* bDescriptorType */
	(INT_IN_EP_NUM | EP_INPUT),	/* bEndpointAddress */
	EP_INT,			/* bmAttributes */
	/* wMaxPacketSize */
	HID_MAX_PACKET_SIZE_INT_IN & 0x00FF,
	(HID_MAX_PACKET_SIZE_INT_IN & 0xFF00) >> 8,
	HID_DEFAULT_INT_IN_INTERVAL		/* bInterval */
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
	20,
	DESC_STRING,
	'H', 0, 'I', 0, 'D', 0, ' ', 0, 'M', 0, 'o', 0, 'u', 0, 's', 0, 'e', 0
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
		/* Get HID Descriptor */
		case DESC_HID:
		{
			gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, LEN_HID);
	
			if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
			{
				my_memcpy(pCtrl, (void *)&USB_ConfigDescriptor[LEN_CONFIG+LEN_INTERFACE], CtrlMaxPacketSize);
				gUsbCtrl.pDesc = (uint8_t *)&USB_ConfigDescriptor[LEN_CONFIG+LEN_INTERFACE+CtrlMaxPacketSize];
				gUsbCtrl.EP0_Toggle = 1;
				USBD->MXPLD0 = CtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
				gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
			}
			else
			{
				my_memcpy(pCtrl, (void *)&USB_ConfigDescriptor[LEN_CONFIG+LEN_INTERFACE], gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			}
			break;
		}
		/* Get HID Report Descriptor */
		case DESC_HID_RPT:
		{
			gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, HID_REPORT_DESCRIPTOR_SIZE);
			if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
			{
				my_memcpy(pCtrl, (void *)HID_MouseReportDescriptor, CtrlMaxPacketSize);
				gUsbCtrl.pDesc = (uint8_t *)&HID_MouseReportDescriptor[CtrlMaxPacketSize];
				gUsbCtrl.EP0_Toggle = 1;
				USBD->MXPLD0 = CtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
				gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
			}
			else
			{
				my_memcpy(pCtrl, (void *)HID_MouseReportDescriptor, gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			}
			break;
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
		/* SET_REPORT */
		case SET_REPORT:
		{
			if ((gUsbCtrl.UsbSetupBuf[6] != 0) || (gUsbCtrl.UsbSetupBuf[7] != 0))
				return FALSE;

			if (gUsbCtrl.UsbSetupBuf[3] == 2)
			{
				/* Request Type = Output */
				USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
				USBD->MXPLD1 = 0;
			}
			else if (gUsbCtrl.UsbSetupBuf[3] == 3)
			{
				/* Request Type = Feature */
				USBD->CFG1 |= USB_CFG_DSQ_SYNC_DATA1;
				USBD->MXPLD1 = 0;
			}
			else
			{
				/* Not support. Reply STALL. */
				USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
				USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
			}

			return TRUE;
		}

		/* SET_IDLE */
		case SET_IDLE:
		{
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
	uint8_t *buf;

	buf = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2);
	mouse_mode ^= 1; 

	if (mouse_mode)
	{
		if (move_len > 14)
		{
			/* Update new report data */
			buf[0] = 0x00;
			buf[1] = mouse_table[mouse_idx & 0x07];
			buf[2] = mouse_table[(mouse_idx + 2) & 0x07];
			buf[3] = 0x00;
			mouse_idx++;
			move_len = 0;
		}
	}
	else
	{
		buf[0] = buf[1] = buf[2] = buf[3] = 0;
	}
	move_len++;
	USBD->MXPLD2 = 4;

	return 0;
}

/**   
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
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
	mouse_mode = 1;
	/* start to IN data */
	USB_EpAck0();
}

/*********************************************************/
/**   
  * @brief  HID_MainProcess, HID mouse main process.
  * @param  None.
  * @retval None.
  */

int32_t HID_MainProcess(void)
{
	STR_USBD_T sParam;

	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_HID;
	sParam.u32CtrlMaxPacketSize = HID_MAX_PACKET_SIZE_CTRL;
	sParam.u32EndpointCount = 1;
	sParam.EP[0].u32EpNum = INT_IN_EP_NUM;
	sParam.EP[0].u32MaxPacketSize = HID_MAX_PACKET_SIZE_INT_IN;
	sParam.EP[0].IsDirIN = TRUE;
	sParam.EP[0].IsIsoTransfer = FALSE;
	USB_Open(&sParam);
    
	while(1)
	{
    }
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
