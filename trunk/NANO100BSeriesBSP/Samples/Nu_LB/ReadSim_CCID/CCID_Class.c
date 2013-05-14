/******************************************************************************
 * @file     CCID_Class.c
 * @brief    Nano1xx USB CCID sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "nano1xx_sys.h"
#include "nano1xx_usb.h"
#include "CCID_Class.h"
#include "ccid_if.h"

/* Define the vendor id and product id */
#define USB_VID		0x0416
#define USB_PID		0xC146

/* Define EP maximum packet size */
#define	MAX_PACKET_SIZE_CTRL        16
#define MAX_PACKET_SIZE_INT			16
#define MAX_PACKET_SIZE_BULK		64

/* Define the EP number */
#define INT_IN_EP_NUM		0x03
#define BULK_OUT_EP_NUM		0x02
#define BULK_IN_EP_NUM		0x02

/* Define Descriptor information */
#define DEFAULT_INTERVAL_INT		0x20
#define DEFAULT_INTERVAL_BULK		0x00
#define IS_SELF_POWERED				0
#define IS_REMOTE_WAKEUP			0
#define MAX_POWER					50	/* The unit is in 2mA. ex: 50 * 2mA = 100mA */

/***********************************/
/* CCID global variable */
#define INT_BUFFER_SIZE		64    /* Interrupt message buffer size */
#define BULK_BUFFER_SIZE	512   /* bulk message buffer size */

uint8_t UsbIntMessageBuffer[INT_BUFFER_SIZE];
uint8_t UsbMessageBuffer[BULK_BUFFER_SIZE];

STR_USBD_CCID_T gCcidInfo;

/***********************************/
#define LEN_CONFIG_AND_SUBORDINATE (LEN_CONFIG+LEN_INTERFACE+LEN_CCID+LEN_ENDPOINT*3)

/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[] =
{
	LEN_DEVICE,		/* bLength */
	DESC_DEVICE,	/* bDescriptorType */
	0x10, 0x01,		/* bcdUSB */
	0x00,			/* bDeviceClass */
	0x00,			/* bDeviceSubClass */
	0x00,			/* bDeviceProtocol */
	MAX_PACKET_SIZE_CTRL,	/* bMaxPacketSize0 */
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
	0x80 | (IS_SELF_POWERED << 6) | (IS_REMOTE_WAKEUP << 5),/* bmAttributes */
	MAX_POWER,		/* MaxPower */

	/* Interface descriptor (Interface 0 = Smart Card Reader) */
	LEN_INTERFACE,	/* bLength */
	DESC_INTERFACE,	/* bDescriptorType */
	0x00,			/* bInterfaceNumber */
	0x00,			/* bAlternateSetting */
	0x03,			/* bNumEndpoints */
	0x0B,			/* bInterfaceClass */
	0x00,			/* bInterfaceSubClass */
	0x00,			/* bInterfaceProtocol (mouse protocol) */
	0x00,			/* iInterface */

	/* CCID class descriptor */
	0x36, 			/* bLength: CCID Descriptor size */
	0x21, 			/* bDescriptorType: HID To be updated with CCID specific number */
	0x00, 			/* bcdHID(LSB): CCID Class Spec release number (1.10) */
	0x01, 			/* bcdHID(MSB) */
	0x01,			/* bMaxSlotIndex */
	0x07,			/* bVoltageSupport: 5v, 3v and 1.8v */
	0x03, 0x00, 0x00, 0x00, 		/* dwProtocols: supports T=0 and T=1 */
	0xA0,0x0F,0x00,0x00,			/* dwDefaultClock: 4 Mhz (0x00000FA0) */
	0xA0,0x0F,0x00,0x00,			/* dwMaximumClock: 4 Mhz (0x00000FA0) */
	0x00,							/* bNumClockSupported => no manual setting */
	0xDA, 0x26, 0x00, 0x00, 		/* dwDataRate: 10080 bps  //10752 bps (0x00002A00) */
	0x48, 0xDB, 0x04, 0x00, 		/* dwMaxDataRate: 312500 bps  // 129032 bps (0x0001F808) */
	0x00,							/* bNumDataRatesSupported => no manual setting */
	0xFE, 0x00, 0x00, 0x00, 		/* dwMaxIFSD: 0 (T=0 only)   */
	0x07, 0x00, 0x00, 0x00, 		/* dwSynchProtocols  */
	0x00, 0x00, 0x00, 0x00,			/* dwMechanical: no special characteristics */	
	0xBE, 0x04, 0x02, 0x00,			/* dwFeatures: clk, baud rate, voltage : automatic, clock stop mode, */
	0x0F, 0x01, 0x00, 0x00, 		/* dwMaxCCIDMessageLength : Maximun block size + header*/
	0xFF, 					/* bClassGetResponse*/
	0xFF, 					/* bClassEnvelope */
	0x00, 0x00, 			/* wLcdLayout */
	0x00, 					/* bPINSupport : no PIN verif and modif  */
	0x01,					/* bMaxCCIDBusySlots */


	/* Endpoint 1 descriptor (Interrupt in) */
	LEN_ENDPOINT,								/* bLength */
	DESC_ENDPOINT,								/* bDescriptorType */
	INT_IN_EP_NUM | 0x80,						/* bEndpointAddress */
	EP_INT,										/* bmAttributes */
	MAX_PACKET_SIZE_INT & 0x00FF,				/* wMaxPacketSize */
	(MAX_PACKET_SIZE_INT & 0xFF00) >> 8,         
	DEFAULT_INTERVAL_INT,						/* bInterval */

	/* Endpoint 2 descriptor (Bulk out) */
	LEN_ENDPOINT,								/* bLength */
	DESC_ENDPOINT,								/* bDescriptorType */
	BULK_OUT_EP_NUM,							/* bEndpointAddress */
	EP_BULK,									/* bmAttributes	*/
	MAX_PACKET_SIZE_BULK & 0x00FF,				/* wMaxPacketSize */
	(MAX_PACKET_SIZE_BULK & 0xFF00) >> 8,
	DEFAULT_INTERVAL_BULK,						/* bInterval */

	/* Endpoint 2 descriptor (Bulk in) */
 	LEN_ENDPOINT,								/* bLength */
 	DESC_ENDPOINT,								/* bDescriptorType */
 	BULK_IN_EP_NUM | 0x80,						/* bEndpointAddress */
 	EP_BULK,									/* bmAttributes */
	MAX_PACKET_SIZE_BULK & 0x00FF,				/* wMaxPacketSize */
	(MAX_PACKET_SIZE_BULK & 0xFF00) >> 8,
 	DEFAULT_INTERVAL_BULK						/* bInterval */
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
	52,             /* bLength          */
	DESC_STRING,    /* bDescriptorType  */
	'N', 0, 'u', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, ' ', 0,
	'S', 0, 'm', 0, 'a', 0, 'r', 0, 't', 0, ' ', 0, 
	'C', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 
	'R', 0, 'e', 0, 'a', 0, 'd', 0, 'e', 0, 'r', 0
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
		/* CCID_ABORT */
		case CCID_ABORT:
		{
			gCcidInfo.bAbortRequestFlag = 1;
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
			return TRUE;
		}

		/* CCID_GET_CLOCK_FREQUENCIES */
		case CCID_GET_CLOCK_FREQUENCIES:
		/* CCID_GET_DATA_RATES */
		case CCID_GET_DATA_RATES:
		{
		    uint8_t pData[1] = {0};
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0), pData, sizeof(pData));
			USBD->MXPLD0 = sizeof(pData);	/* CTRL IN */
			gUsbCtrl.StateFlag = FLAG_OUT_ACK;

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
	/* BULK IN transfer */
	if (gCcidInfo.isBulkInReady)
	{
		if(gCcidInfo.UsbMessageLength >= MAX_PACKET_SIZE_BULK)
		{
			gCcidInfo.isBulkInReady = 1;
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), gCcidInfo.pUsbMessageBuffer, MAX_PACKET_SIZE_BULK);
			USBD->MXPLD2 = MAX_PACKET_SIZE_BULK;	/* BULK IN */

			gCcidInfo.pUsbMessageBuffer += MAX_PACKET_SIZE_BULK;
			gCcidInfo.UsbMessageLength -= MAX_PACKET_SIZE_BULK;
		}
		else
		{
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), gCcidInfo.pUsbMessageBuffer, gCcidInfo.UsbMessageLength);
			USBD->MXPLD2 = gCcidInfo.UsbMessageLength;	/* BULK IN */
			gCcidInfo.UsbMessageLength = 0;
			gCcidInfo.isBulkInReady = 0;
		}
	}
	if (!gCcidInfo.isBulkOutReady)
		USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;

	return 0;
}

/**   
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
	/* BULK OUT */
	static int offset = 0;
	uint32_t len;

	len = USBD->MXPLD3;

	my_memcpy(&UsbMessageBuffer[offset], (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG3), len);

	if((len >= 0x0A && len != 0xFF) || offset != 0)
	{
		if(offset == 0) {
			/* Calculate number of byte to receive to finish the message  */
			gCcidInfo.UsbMessageLength = USB_MESSAGE_HEADER_SIZE + make32(&UsbMessageBuffer[OFFSET_DWLENGTH]);
		}
			
		gCcidInfo.UsbMessageLength -= (int) len;
		/* Prepare next reception if whole message not received */
		if(gCcidInfo.UsbMessageLength > 0) {
			gCcidInfo.pUsbMessageBuffer = UsbMessageBuffer + len;
			offset += len;
		}
		
		if(gCcidInfo.UsbMessageLength == 0) {
			gCcidInfo.isBulkOutReady = 1;
			offset = 0;
		}
			if(gCcidInfo.UsbMessageLength < 0)
		{
			UsbMessageBuffer[OFFSET_DWLENGTH] = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0xFF;
			gCcidInfo.isBulkOutReady = 1;
		}
	}
	CCID_DispatchMessage();

	/* trigger next out packet */
	if (gCcidInfo.UsbMessageLength > 0)
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
	/* INT IN transfer */
	if (gCcidInfo.isDeviceReady)
	{
		RDR_to_PC_NotifySlotChange();
		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG4), gCcidInfo.pu8IntInBuf, 2);
		USBD->MXPLD4 = 2;	/* INT IN */
		gCcidInfo.isDeviceReady = 0;
	}
	if (!gCcidInfo.isBulkOutReady)
		USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;

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
	gCcidInfo.isDeviceReady = 1;
	RDR_to_PC_NotifySlotChange();
	my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG4), gCcidInfo.pu8IntInBuf, 2);
	USBD->MXPLD4 = 2;	/* INT IN */
	USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;	/* BULK OUT */
}

/*********************/
/* CCID functions    */
/*********************/
void UsbBulkInMessage(void)
{
	gCcidInfo.UsbMessageLength = USB_MESSAGE_HEADER_SIZE + make32(&UsbMessageBuffer[OFFSET_DWLENGTH]);

	gCcidInfo.pUsbMessageBuffer = UsbMessageBuffer;

	if (gCcidInfo.isBulkInReady)
	{
		if(gCcidInfo.UsbMessageLength >= MAX_PACKET_SIZE_BULK)
		{
			gCcidInfo.isBulkInReady = 1;
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), gCcidInfo.pUsbMessageBuffer, MAX_PACKET_SIZE_BULK);
			USBD->MXPLD2 = MAX_PACKET_SIZE_BULK;	/* BULK IN */

			gCcidInfo.pUsbMessageBuffer += MAX_PACKET_SIZE_BULK;
			gCcidInfo.UsbMessageLength -= MAX_PACKET_SIZE_BULK;
		}
		else
		{
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), gCcidInfo.pUsbMessageBuffer, gCcidInfo.UsbMessageLength);
			USBD->MXPLD2 = gCcidInfo.UsbMessageLength;	/* BULK IN */
			gCcidInfo.UsbMessageLength = 0;
			gCcidInfo.isBulkInReady = 0;
		}
	}
}

void CCID_DispatchMessage(void)
{
	uint8_t ErrorCode;

	if(gCcidInfo.isBulkOutReady)
	{
		switch(UsbMessageBuffer[OFFSET_BMESSAGETYPE])
		{
			case PC_TO_RDR_ICCPOWERON:
				ErrorCode = PC_to_RDR_IccPowerOn();
				RDR_to_PC_DataBlock(ErrorCode);
				break;
			case PC_TO_RDR_ICCPOWEROFF:
				ErrorCode = PC_to_RDR_IccPowerOff();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_GETSLOTSTATUS:
				ErrorCode = PC_to_RDR_GetSlotStatus();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_XFRBLOCK:
				ErrorCode = PC_to_RDR_XfrBlock();
				RDR_to_PC_DataBlock(ErrorCode);
				break;
			case PC_TO_RDR_GETPARAMETERS:
				ErrorCode = PC_to_RDR_GetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_RESETPARAMETERS:
				ErrorCode = PC_to_RDR_ResetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_SETPARAMETERS:
				ErrorCode = PC_to_RDR_SetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_ESCAPE:
				ErrorCode = PC_to_RDR_Escape();
				RDR_to_PC_Escape(ErrorCode);
				break;
			case PC_TO_RDR_ICCCLOCK:
				ErrorCode = PC_to_RDR_IccClock();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_ABORT:
				ErrorCode = PC_to_RDR_Abort();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY:
			case PC_TO_RDR_SECURE:
			case PC_TO_RDR_T0APDU:
			case PC_TO_RDR_MECHANICAL:
			default:
				CmdNotSupported();
				break;
		}

		UsbBulkInMessage();
		gCcidInfo.isBulkOutReady = 0;
	}
}

void SC_CardState(void)
{
	RDR_to_PC_NotifySlotChange();
	my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG4), gCcidInfo.pu8IntInBuf, 2);
	USBD->MXPLD4 = 2;	/* INT IN */
}

extern volatile uint8_t g_gpioINTChange;

/*********************************************************/
/**   
  * @brief  CCID_MainProcess, CCID main process.
  * @param  None.
  * @retval None.
  */

int32_t CCID_MainProcess(void)
{
	STR_USBD_T sParam;

	memset((void *)&gCcidInfo, 0, sizeof(STR_USBD_CCID_T));
	gCcidInfo.pu8IntInBuf = UsbIntMessageBuffer;


	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_CCID;
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

	while(1)
	{

		if (gUsbCtrl.CurrentState == USB_STATE_DETACHED)
		{
			break;
		}

		if(g_gpioINTChange==1)
			break;
			
	}

	USB_ENABLE_SE0();
	return(0);
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
