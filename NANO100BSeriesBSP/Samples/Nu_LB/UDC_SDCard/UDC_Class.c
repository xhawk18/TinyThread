/******************************************************************************
 * @file     UDC_Class.c
 * @brief    Nano1xx USB Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "nano1xx_usb.h"
#include "UDC_Class.h"
#include "sdcard.h"

/* Define the vendor id and product id */
#define USB_VID		0x0416
#define USB_PID		0x8240

/* Define EP maximum packet size */
#define	MAX_PACKET_SIZE_CTRL        64
#define MAX_PACKET_SIZE_BULK		64

/* Define the interrupt In EP number */
#define BULK_IN_EP_NUM		0x02
#define BULK_OUT_EP_NUM		0x03

/********************* Global Variables **********************/
STR_USBD_MASS_T gMassInfo;


/*********************************************************/
/*  Mass Storage Global Variables                        */
/*********************************************************/
uint32_t MassBlock[MASS_BUFFER_SIZE / 4];
uint32_t Storage_Block[STORAGE_BUFFER_SIZE / 4];

#define MassCMD_BUF        ((uint32_t)&MassBlock[0])
#define STORAGE_DATA_BUF   ((uint32_t)&Storage_Block[0])

/*********************************************************/
/*  Mass Storage Mode Sense                              */
/*********************************************************/
/* code = 5Ah, Mode Sense */
static uint8_t ModePage_01[12] =
{
	0x01, 0x0A, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x00, 0x00, 0x00
};

static uint8_t ModePage_05[32] = 
{
	0x05, 0x1E, 0x13, 0x88, 0x08, 0x20, 0x02, 0x00,
	0x01, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x05, 0x1E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x68, 0x00, 0x00
};

static uint8_t ModePage_1B[12] = 
{
	0x1B, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static uint8_t ModePage_1C[8] = 
{
	0x1C, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00
};

const uint8_t InquiryID[36] = 
{
	0x00,					/* Peripheral Device Type */
	0x80,					/* RMB */
	0x00,					/* ISO/ECMA, ANSI Version */
	0x00,					/* Response Data Format */
	0x1F, 0x00, 0x00, 0x00,	/* Additional Length */

	/* Vendor Identification */
	'N', 'u', 'v', 'o', 't', 'o', 'n', ' ',

	/* Product Identification */
	'1', '0', '0', ' ', 'M', 'a', 's', 's', ' ', 'S', 't', 'o', 'r', 'a', 'g', 'e',

	/* Product Revision */
	'1', '.', '0', '0'
};

/********************************************************/
static __INLINE uint32_t get_be32(uint8_t *buf)
{
	return ((uint32_t) buf[0] << 24) | ((uint32_t) buf[1] << 16) |
			((uint32_t) buf[2] << 8) | ((uint32_t) buf[3]);
}

/*********************************************************/

/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[] =
{
	LEN_DEVICE,		    /* bLength */
	DESC_DEVICE,	    /* bDescriptorType */
	0x10, 0x01,		    /* bcdUSB */
	0x00,			    /* bDeviceClass */
	0x00,			    /* bDeviceSubClass */
	0x00,			    /* bDeviceProtocol */
	MAX_PACKET_SIZE_CTRL,	/* bMaxPacketSize0 */
	/* idVendor */
	USB_VID & 0x00FF,
	(USB_VID & 0xFF00) >> 8,
	/* idProduct */
	USB_PID & 0x00FF,
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x00,		    /* bcdDevice */
	0x01,			    /* iManufacture */
	0x02,			    /* iProduct */
	0x01,			    /* iSerialNumber */
	0x01			    /* bNumConfigurations */
};


/*!<USB Configure Descriptor */
const uint8_t USB_ConfigDescriptor[] =
{
	LEN_CONFIG,		                                        /* bLength */
	DESC_CONFIG,	                                        /* bDescriptorType */
	(LEN_CONFIG+LEN_INTERFACE+LEN_ENDPOINT*2), 0x00,		/* wTotalLength */
	0x01,			                                        /* bNumInterfaces */
	0x01,			                                        /* bConfigurationValue */
	0x00,			                                        /* iConfiguration */
	0xC0,			                                        /* bmAttributes */
	0x32,			                                        /* MaxPower */

/* const BYTE cbyInterfaceDescriptor[LEN_INTERFACE] = */
	LEN_INTERFACE,	/* bLength */
	DESC_INTERFACE,	/* bDescriptorType */
	0x00,			/* bInterfaceNumber */
	0x00,			/* bAlternateSetting */
	0x02,			/* bNumEndpoints */
	0x08,			/* bInterfaceClass */
	0x05,			/* bInterfaceSubClass */
	0x50,			/* bInterfaceProtocol */
	0x00,			/* iInterface */

/* const BYTE cbyEndpointDescriptor1[LEN_ENDPOINT] = */
	LEN_ENDPOINT,	        /* bLength */
	DESC_ENDPOINT,	        /* bDescriptorType */
	0x82,			        /* bEndpointAddress */
	EP_BULK,		        /* bmAttributes */
	MAX_PACKET_SIZE_BULK, 0x00,	/* wMaxPacketSize */
	0x00,			        /* bInterval */

/* const BYTE cbyEndpointDescriptor2[LEN_ENDPOINT] = */
	LEN_ENDPOINT,	        /* bLength */
	DESC_ENDPOINT,	        /* bDescriptorType */
	0x03,			        /* bEndpointAddress */
	EP_BULK,		        /* bmAttributes */
	MAX_PACKET_SIZE_BULK, 0x00,	/* wMaxPacketSize */
	0x00			        /* bInterval */
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
	24,
	DESC_STRING,
	'M', 0, 'a', 0, 's', 0, 's', 0, 'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0
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
		/* Bulk-Only Mass Storage Reset */
		case BULK_ONLY_MASS_STORAGE_RESET:
		{
			if ((gUsbCtrl.UsbSetupBuf[6] != 0) || (gUsbCtrl.UsbSetupBuf[7] != 0))
            	return FALSE;

			gUdcInfo.CBWInvalid = FALSE;
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
			USB_DeviceStart();
			return TRUE;
		}

		/* Get Max LUN */
		case GET_MAX_LUN:
		{
            if ((gUsbCtrl.UsbSetupBuf[6] != 1) || (gUsbCtrl.UsbSetupBuf[7] != 0))
				return FALSE;
			*(uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0) = 0;
            
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 1;
			
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
	/* Bulk IN */
	/* Update new report data */
	int32_t volatile idx;

	if (gMassInfo.BulkState == BULK_CSW)
	{
	    /* Prepare to receive the CBW */
		gMassInfo.BulkState = BULK_CBW;

       	USBD->BUFSEG3 = gMassInfo.BulkBuf0;
		USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
	}
	else if (gMassInfo.BulkState == BULK_IN)
	{
		switch (gMassInfo.sCBW.u8OPCode)
		{
			case UFI_READ_FORMAT_CAPACITY:
			case UFI_READ_CAPACITY:
			case UFI_MODE_SENSE_10:
			{
				if (gMassInfo.Length > 0)
				{
					UsbRead();
					return TRUE;
				}
			}
			case UFI_READ_10:
			{
				if (gMassInfo.Length > 0)
				{
					UsbReadTrig();
					return TRUE;
				}			
			}
			
			case UFI_REQUEST_SENSE:
			case UFI_INQUIRY:
			{
				gMassInfo.sCSW.dCSWDataResidue = 0;
				gMassInfo.sCSW.bCSWStatus = 0;
				break;
			}

			case UFI_PREVENT_ALLOW_MEDIUM_REMOVAL:
			case UFI_VERIFY_10:
			case UFI_START_STOP:
			case UFI_WRITE_10:
			{
				int32_t tmp;
				
				tmp = gMassInfo.sCBW.dCBWDataTransferLength - STORAGE_BUFFER_SIZE;
				if (tmp < 0) 
					tmp = 0;
				
				gMassInfo.sCSW.dCSWDataResidue = tmp;
				gMassInfo.sCSW.bCSWStatus = 0;
				break;
			}
			case UFI_TEST_UNIT_READY:
			{
				gMassInfo.sCSW.dCSWDataResidue = 0;
				gMassInfo.sCSW.bCSWStatus = 0;
				break;
			}
			default:
			{
			    /* Unsupported commmand. Return command fail status */
				gMassInfo.sCSW.dCSWDataResidue = gMassInfo.sCBW.dCBWDataTransferLength;
				gMassInfo.sCSW.bCSWStatus = 0x01;
				break;
			}
		}
		
		/* Return the CSW */
       	USBD->BUFSEG2 = gMassInfo.BulkBuf1;

        /* Bulk IN buffer */
		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), &gMassInfo.sCSW.dCSWSignature, 16);

		gMassInfo.BulkState = BULK_CSW;
		USBD->MXPLD2 = 13;
	}
	return TRUE;
}

/**   
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
	/* Bulk Out */
	uint8_t u8Len;
	int32_t i;

	if (gMassInfo.BulkState == BULK_CBW)
	{
		u8Len = USBD->MXPLD3;
		
		/* Check Signature & length of CBW */
		/* Bulk Out buffer */
		if ((*(uint32_t *) ((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0) != CBW_SIGNATURE) || (u8Len != 31))
		{
			return FALSE;
		}
		
		/* Get the CBW */
		for (i = 0; i < u8Len; i++)
			*((uint8_t *) (&gMassInfo.sCBW.dCBWSignature) + i) = *(uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0 + i);
			
		/* Prepare to echo the tag from CBW to CSW */
		gMassInfo.sCSW.dCSWTag = gMassInfo.sCBW.dCBWTag;
		
		/* Parse Op-Code of CBW */
		switch (gMassInfo.sCBW.u8OPCode)
		{
			case UFI_PREVENT_ALLOW_MEDIUM_REMOVAL:
			{
				if (gMassInfo.sCBW.au8Data[2] & 0x01)
				{
					gMassInfo.SenseKey[0] = 0x05;  //INVALID COMMAND
					gMassInfo.SenseKey[1] = 0x24;
					gMassInfo.SenseKey[2] = 0;
					gMassInfo.preventflag = 1;
				}
				else
					gMassInfo.preventflag = 0;
				gMassInfo.BulkState = BULK_IN;
				USB_EpAck0();
				return TRUE;
			}
			case UFI_VERIFY_10:
			case UFI_START_STOP:
			case UFI_TEST_UNIT_READY:
			{
				gMassInfo.BulkState = BULK_IN;
				USB_EpAck0();
				return TRUE;
			}
			case UFI_REQUEST_SENSE:
			{
				ReqSenCommand();
				gMassInfo.BulkState = BULK_IN;
				USBD->MXPLD2 = 18;
				return TRUE;
			}
			case UFI_READ_FORMAT_CAPACITY:
			{
				if (gMassInfo.Length == 0)
				{
					gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
					gMassInfo.Address = MassCMD_BUF;
				}
				RdFmtCapCommand();
				gMassInfo.BulkState = BULK_IN;
				if (gMassInfo.Length > 0)
				{
					if (gMassInfo.Length > MAX_PACKET_SIZE_BULK)
						gMassInfo.Size = MAX_PACKET_SIZE_BULK;
					else
						gMassInfo.Size = gMassInfo.Length;

					/* Bulk IN buffer */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (char *)gMassInfo.Address, gMassInfo.Size);

					gMassInfo.Address += gMassInfo.Size;
					USBD->BUFSEG2 = gMassInfo.BulkBuf0;
					UsbRead ();
				}
				else
					USB_EpAck0();
				return TRUE;
			}
			case UFI_READ_CAPACITY:
			{
				if (gMassInfo.Length == 0)
				{
    				gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
					gMassInfo.Address = MassCMD_BUF;
				}
				
				RdCurCapCommand();
				gMassInfo.BulkState = BULK_IN;
				if (gMassInfo.Length > 0)
				{
					if (gMassInfo.Length > MAX_PACKET_SIZE_BULK)
						gMassInfo.Size = MAX_PACKET_SIZE_BULK;
					else
						gMassInfo.Size = gMassInfo.Length;

					/* Bulk IN buffer */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (char *)gMassInfo.Address, gMassInfo.Size);

					gMassInfo.Address += gMassInfo.Size;
			       	USBD->BUFSEG2 = gMassInfo.BulkBuf0;
					UsbRead();
				}
				else
					USB_EpAck0();
				return TRUE;
			}
			case UFI_MODE_SELECT_10:
			{
				gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
				gMassInfo.Address = MassCMD_BUF;

				if (gMassInfo.Length > 0)
				{
					USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
					gMassInfo.BulkState = BULK_OUT;
				}
				return TRUE;
			}
			case UFI_MODE_SENSE_10:
			{
				if (gMassInfo.Length == 0)
				{
					gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
					gMassInfo.Address = MassCMD_BUF;
				}
				
				ModeSenseCommand();
				gMassInfo.BulkState = BULK_IN;
				if (gMassInfo.Length > 0)
				{
					if (gMassInfo.Length > MAX_PACKET_SIZE_BULK)
						gMassInfo.Size = MAX_PACKET_SIZE_BULK;
					else
						gMassInfo.Size = gMassInfo.Length;
					/* Bulk IN buffer */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (char *)gMassInfo.Address, gMassInfo.Size);

					gMassInfo.Address += gMassInfo.Size;

					USBD->BUFSEG2 = gMassInfo.BulkBuf0;
					UsbRead();
				}
				else
					USB_EpAck0();
				return TRUE;
			}
			case UFI_INQUIRY:
			{
                /* Bulk IN buffer */
				my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (void *)InquiryID, 36);

				gMassInfo.BulkState = BULK_IN;

				USBD->MXPLD2 = 36;
				return TRUE;
			}
			case UFI_READ_10:
			{
			    /* Check if it is a new transfer */
				if(gMassInfo.Length == 0)
				{
				    /* Prepare the data for Bulk IN transfer */
				
					/* Get LBA address */
					gMassInfo.LbaAddress = get_be32(&gMassInfo.sCBW.au8Data[0]);
					gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
					gMassInfo.BytesInStorageBuf = gMassInfo.Length;

					if(gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
					{
					    i = gMassInfo.Length;
					    if(i > STORAGE_BUFFER_SIZE)
					        i = STORAGE_BUFFER_SIZE;
					    
						SpiRead(gMassInfo.LbaAddress, i, (uint8_t*)STORAGE_DATA_BUF);
					    gMassInfo.BytesInStorageBuf = i;
					    gMassInfo.LbaAddress += (i/UDC_SECTOR_SIZE);
					}
				}
				gMassInfo.Address = STORAGE_DATA_BUF;

                /* Indicate the next packet should be Bulk IN Data packet */
				gMassInfo.BulkState = BULK_IN;
				
				if(gMassInfo.BytesInStorageBuf > 0)
				{
				    /* Set the packet size */
					if (gMassInfo.BytesInStorageBuf > MAX_PACKET_SIZE_BULK)
						gMassInfo.Size = MAX_PACKET_SIZE_BULK;
					else
						gMassInfo.Size = gMassInfo.BytesInStorageBuf;
					
					/* Prepare the first data packet (DATA1) */
					/* Bulk IN buffer */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (char *)gMassInfo.Address, gMassInfo.Size);
					gMassInfo.Address += gMassInfo.Size;
					
					/* kick - start */
					USBD->BUFSEG2 = gMassInfo.BulkBuf1;
					/* Trigger to send out the data packet */	
					USBD->MXPLD2 = gMassInfo.Size;
					gMassInfo.Length -= gMassInfo.Size;
					gMassInfo.BytesInStorageBuf -= gMassInfo.Size;
				}
				else
				{
					USB_EpAck0();
				}
					
				return TRUE;
			}
			case UFI_WRITE_10:
			{
				if (gMassInfo.Length == 0)
				{
					gMassInfo.Length = gMassInfo.sCBW.dCBWDataTransferLength;
					gMassInfo.Address = STORAGE_DATA_BUF;
					gMassInfo.DataFlashStartAddr = get_be32(&gMassInfo.sCBW.au8Data[0]);
				}

				if ((gMassInfo.Length > 0))
				{
					USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
					gMassInfo.BulkState = BULK_OUT;
				}
				    
				return TRUE;
			}
			default:
			{
        		/* Unsupported command */
        		gMassInfo.SenseKey[0] = 0x05;
        		gMassInfo.SenseKey[1] = 0x20;
        		gMassInfo.SenseKey[2] = 0x00;
        		
        		/* If CBW request for data phase, just return zero packet to end data phase */
        		if (gMassInfo.sCBW.dCBWDataTransferLength > 0)
        		{
        			/* Data Phase, zero/short packet */
        			if ((gMassInfo.sCBW.bmCBWFlags & 0x80) != 0)
        			{	
        			    /* Data-In */
        				gMassInfo.BulkState = BULK_IN;
						USBD->MXPLD2 = 0;
        			}
        		}
        		else
        		{	/* Status Phase */
        			gMassInfo.BulkState = BULK_IN;
        			USB_EpAck0();
        		}
			    return TRUE;
			}
		}
	}
	else if (gMassInfo.BulkState == BULK_OUT)
	{	
		switch (gMassInfo.sCBW.u8OPCode)
		{
			case UFI_WRITE_10:
			case UFI_MODE_SELECT_10:
			{
				UsbWrite();
				return TRUE;
			}
		}
	}
	return TRUE;
}


/**   
  * @brief  USB_EpAck2, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck2(void)
{
	return TRUE;
}


/**   
  * @brief  USB_EpAck3, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck3(void)
{
	return TRUE;
}


/**   
  * @brief  USB_EpAck4, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck4(void)
{
	return TRUE;
}


/**   
  * @brief  USB_EpAck5, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck5(void)
{
	return TRUE;
}


/**   
  * @brief  USB_DeviceStart, Start USB Device Transfer.
  * @param  None.
  * @retval None.
  */
void USB_DeviceStart(void)
{
	/* Set toggle bit to DATA0 */
	USBD->CFG2 &= ~USB_CFG_DSQ_SYNC_DATA1;
	USBD->CFG3 &= ~USB_CFG_DSQ_SYNC_DATA1;

	gMassInfo.BulkState = BULK_CBW;
	USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
}


/*********************************************************/
/*  Mass Storage Command APIs                            */
/*********************************************************/
/**   
  * @brief  ModeSenseCommand.
  * @param  None.
  * @retval None.
  */
void ModeSenseCommand(void)
{
	uint8_t i,j;
	uint8_t NumHead,NumSector;
	uint16_t NumCyl=0;

    /* Clear the command buffer */
    *((uint32_t *)MassCMD_BUF  ) = 0;
    *((uint32_t *)MassCMD_BUF+1) = 0;

	switch (gMassInfo.sCBW.au8Data[0])
	{
		case 0x01:
		{
			*((uint8_t *)MassCMD_BUF) = 19;
			i = 8;
			for (j = 0; j<12; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_01[j];
			break;
		}
		case 0x05:
		{
			*((uint8_t *)MassCMD_BUF) = 39;
			i = 8;
			for (j = 0; j<32; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_05[j];
			
			NumHead = 2;
			NumSector = 64;

			if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
				NumCyl = gMassInfo.dataFlashTotalSectors/128;

			*((uint8_t *)(MassCMD_BUF+12)) = NumHead;
			*((uint8_t *)(MassCMD_BUF+13)) = NumSector;
			*((uint8_t *)(MassCMD_BUF+16)) = (uint8_t)(NumCyl >> 8);
			*((uint8_t *)(MassCMD_BUF+17)) = (uint8_t)(NumCyl & 0x00ff);
			break;
		}
		case 0x1B:
		{
			*((uint8_t *)MassCMD_BUF) = 19;
			i = 8;
			for (j = 0; j<12; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_1B[j];
			break;
		}
		case 0x1C:
		{
			*((uint8_t *)MassCMD_BUF) = 15;
			i = 8;
			for (j = 0; j<8; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_1C[j];
			break;
		}
		case 0x3F:
		{
			*((uint8_t *)MassCMD_BUF) = 0x47;
			i = 8;
			for (j = 0; j<12; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_01[j];
			for (j = 0; j<32; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_05[j];
			for (j = 0; j<12; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_1B[j];
			for (j = 0; j<8; j++, i++)
				*((uint8_t *)(MassCMD_BUF+i)) = ModePage_1C[j];
			
			NumHead = 2;
			NumSector = 64;

			if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
				NumCyl = gMassInfo.dataFlashTotalSectors/128;

			*((uint8_t *)(MassCMD_BUF+24)) = NumHead;
			*((uint8_t *)(MassCMD_BUF+25)) = NumSector;
			*((uint8_t *)(MassCMD_BUF+28)) = (uint8_t)(NumCyl >> 8);
			*((uint8_t *)(MassCMD_BUF+29)) = (uint8_t)(NumCyl & 0x00ff);
			break;
		}
		default:
			gMassInfo.SenseKey[0] = 0x05;
			gMassInfo.SenseKey[1] = 0x24;
			gMassInfo.SenseKey[2] = 0x00;
	}
}


/**   
  * @brief  ReqSenCommand.
  * @param  None.
  * @retval None.
  */
void ReqSenCommand(void)
{
    uint8_t tmp[20];
    
    if (gMassInfo.preventflag==1)
    {
        gMassInfo.preventflag=0;
		tmp[0] = 0x70;
    }
    else
		tmp[0] = 0xf0;

	tmp[1] = 0;
	tmp[3] = 0;
	tmp[4] = 0;
	tmp[5] = 0;
	tmp[6] = 0;
	tmp[7] = 0x0A;
	tmp[8] = 0;
	tmp[9] = 0;
	tmp[10] = 0;
	tmp[11] = 0;
	tmp[14] = 0;
	tmp[15] = 0;
	tmp[16] = 0;
	tmp[17] = 0;

	tmp[2] = gMassInfo.SenseKey[0];
	tmp[12] = gMassInfo.SenseKey[1];
	tmp[13] = gMassInfo.SenseKey[2];

	/* Bulk IN buffer */
	my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (void *)tmp, 20);

	// Sense Key
	gMassInfo.SenseKey[0] = 0x00;
	gMassInfo.SenseKey[1] = 0x00;
	gMassInfo.SenseKey[2] = 0x00;
}


/**   
  * @brief  RdFmtCapCommand.
  * @param  None.
  * @retval None.
  */
void RdFmtCapCommand(void)
{
    uint32_t i;
    uint32_t TotalSectors = 0;

	for (i = 0; i < 36; i++)
		*((uint8_t *)(MassCMD_BUF+i)) = 0;

	*((uint8_t *)(MassCMD_BUF+3)) = 0x10;

    if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
		TotalSectors = gMassInfo.dataFlashTotalSectors;

	*((uint8_t *)(MassCMD_BUF+4)) = *((uint8_t *)&TotalSectors+3);
	*((uint8_t *)(MassCMD_BUF+5)) = *((uint8_t *)&TotalSectors+2);
	*((uint8_t *)(MassCMD_BUF+6)) = *((uint8_t *)&TotalSectors+1);
	*((uint8_t *)(MassCMD_BUF+7)) = *((uint8_t *)&TotalSectors+0);
	*((uint8_t *)(MassCMD_BUF+8)) = 0x02;
	*((uint8_t *)(MassCMD_BUF+10)) = 0x02;
	*((uint8_t *)(MassCMD_BUF+12)) = *((uint8_t *)&TotalSectors+3);
	*((uint8_t *)(MassCMD_BUF+13)) = *((uint8_t *)&TotalSectors+2);
	*((uint8_t *)(MassCMD_BUF+14)) = *((uint8_t *)&TotalSectors+1);
	*((uint8_t *)(MassCMD_BUF+15)) = *((uint8_t *)&TotalSectors+0);
	*((uint8_t *)(MassCMD_BUF+18)) = 0x02;
}


/**   
  * @brief  RdCurCapCommand.
  * @param  None.
  * @retval None.
  */
void RdCurCapCommand(void)
{
    int i;
    uint32_t temp;

	for (i = 0; i < 36; i++)
		*((uint8_t *)(MassCMD_BUF+i)) = 0;

    if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
        temp = gMassInfo.dataFlashTotalSectors - 1;

	*((uint8_t *)(MassCMD_BUF+0)) = *((uint8_t *)&temp+3);
	*((uint8_t *)(MassCMD_BUF+1)) = *((uint8_t *)&temp+2);
	*((uint8_t *)(MassCMD_BUF+2)) = *((uint8_t *)&temp+1);
	*((uint8_t *)(MassCMD_BUF+3)) = *((uint8_t *)&temp+0);
	*((uint8_t *)(MassCMD_BUF+6)) = 0x02;
}


/**   
  * @brief  UsbRead.
  * @param  None.
  * @retval None.
  */
void UsbRead(void)
{
    uint32_t u32Len;

	/* DATA0/DATA1 Toggle */
	if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
		USBD->BUFSEG2 = gMassInfo.BulkBuf0;
	else
		USBD->BUFSEG2 = gMassInfo.BulkBuf1;
		
	/* Trigger to send out the data packet */	
	USBD->MXPLD2 = gMassInfo.Size;
	
	gMassInfo.Length -= gMassInfo.Size;
	gMassInfo.BytesInStorageBuf -= gMassInfo.Size;

    if (gMassInfo.Length)
    {
        if (gMassInfo.BytesInStorageBuf)
        {
        	/* Prepare next data packet */
        	gMassInfo.Size = MAX_PACKET_SIZE_BULK;
        	if (gMassInfo.Size > gMassInfo.Length)
        	    gMassInfo.Size = gMassInfo.Length;

			if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), (int8_t *)gMassInfo.Address, gMassInfo.Size);
			else
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (int8_t *)gMassInfo.Address, gMassInfo.Size);
        	gMassInfo.Address += gMassInfo.Size;
    	}
    	else
    	{
		    u32Len = gMassInfo.Length;
		    if (u32Len > STORAGE_BUFFER_SIZE)
		        u32Len = STORAGE_BUFFER_SIZE;
		    
			SpiRead(gMassInfo.LbaAddress, u32Len, (uint8_t*)STORAGE_DATA_BUF);
		    gMassInfo.BytesInStorageBuf = u32Len;
		    gMassInfo.LbaAddress += (u32Len/UDC_SECTOR_SIZE);
            gMassInfo.Address = STORAGE_DATA_BUF;		    

        	/* Prepare next data packet */
        	gMassInfo.Size = MAX_PACKET_SIZE_BULK;
        	if (gMassInfo.Size > gMassInfo.Length)
        	    gMassInfo.Size = gMassInfo.Length;

			if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), (int8_t *)gMassInfo.Address, gMassInfo.Size);
			else
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (int8_t *)gMassInfo.Address, gMassInfo.Size);
        	gMassInfo.Address += gMassInfo.Size;		    
    	}
	}
}


/**   
  * @brief  UsbReadTrig.
  * @param  None.
  * @retval None.
  */
void UsbReadTrig(void)
{
    uint32_t u32Len;
    
    if (gMassInfo.Length)
    {
        if (gMassInfo.BytesInStorageBuf)
        {
        	/* Prepare next data packet */
        	gMassInfo.Size = MAX_PACKET_SIZE_BULK;
        	if (gMassInfo.Size > gMassInfo.Length)
        	    gMassInfo.Size = gMassInfo.Length;

			if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), (int8_t *)gMassInfo.Address, gMassInfo.Size);
			else
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (int8_t *)gMassInfo.Address, gMassInfo.Size);
        	gMassInfo.Address += gMassInfo.Size;
    	}
    	else
    	{
		    u32Len = gMassInfo.Length;
		    if (u32Len > STORAGE_BUFFER_SIZE)
		        u32Len = STORAGE_BUFFER_SIZE;
		    
			SpiRead(gMassInfo.LbaAddress, u32Len, (uint8_t*)STORAGE_DATA_BUF);
		    gMassInfo.BytesInStorageBuf = u32Len;
		    gMassInfo.LbaAddress += (u32Len/UDC_SECTOR_SIZE);
            gMassInfo.Address = STORAGE_DATA_BUF;		    
		    
        	/* Prepare next data packet */
        	gMassInfo.Size = MAX_PACKET_SIZE_BULK;
        	if (gMassInfo.Size > gMassInfo.Length)
        	    gMassInfo.Size = gMassInfo.Length;

			if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), (int8_t *)gMassInfo.Address, gMassInfo.Size);
			else
        		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), (int8_t *)gMassInfo.Address, gMassInfo.Size);
        	gMassInfo.Address += gMassInfo.Size;		    
    	}
	
		/* DATA0/DATA1 Toggle */
		if (USBD->BUFSEG2 == gMassInfo.BulkBuf1)
        	USBD->BUFSEG2 = gMassInfo.BulkBuf0;
		else
        	USBD->BUFSEG2 = gMassInfo.BulkBuf1;
		
		/* Trigger to send out the data packet */	
		USBD->MXPLD2 = gMassInfo.Size;
	
		gMassInfo.Length -= gMassInfo.Size;
		gMassInfo.BytesInStorageBuf -= gMassInfo.Size;

	}
	else
		USBD->MXPLD2 = 0;
}


/**   
  * @brief  UsbWrite.
  * @param  None.
  * @retval None.
  */
void UsbWrite(void)
{
	uint32_t lba, len;

	if (gMassInfo.Length > MAX_PACKET_SIZE_BULK)
	{
		if (USBD->BUFSEG3 == gMassInfo.BulkBuf0)
		{
	       	USBD->BUFSEG3 = gMassInfo.BulkBuf1;
			USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
			my_memcpy((char *)gMassInfo.Address, (uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), MAX_PACKET_SIZE_BULK);
		}
		else
		{
       		USBD->BUFSEG3 = gMassInfo.BulkBuf0;
			USBD->MXPLD3 = MAX_PACKET_SIZE_BULK;
			my_memcpy((char *)gMassInfo.Address, (uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), MAX_PACKET_SIZE_BULK);
		}
				
		gMassInfo.Address += MAX_PACKET_SIZE_BULK;
		gMassInfo.Length -= MAX_PACKET_SIZE_BULK;
		
		/* Buffer full. Writer it to storage first. */
		if (gMassInfo.Address >= (STORAGE_DATA_BUF + STORAGE_BUFFER_SIZE))
		{
			if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
				SpiWrite(gMassInfo.DataFlashStartAddr, STORAGE_BUFFER_SIZE, (uint8_t*)STORAGE_DATA_BUF);
		    
		    gMassInfo.Address = STORAGE_DATA_BUF;
		    gMassInfo.DataFlashStartAddr += STORAGE_BUFFER_SIZE/UDC_SECTOR_SIZE;
		}
	}
	else
	{
		if (USBD->BUFSEG3 == gMassInfo.BulkBuf0)
			my_memcpy((char *)gMassInfo.Address, (uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf0), gMassInfo.Length);
		else
			my_memcpy((char *)gMassInfo.Address, (uint8_t *)((uint32_t)USBD_SRAM_BASE + gMassInfo.BulkBuf1), gMassInfo.Length);
		gMassInfo.Address += gMassInfo.Length;
		gMassInfo.Length = 0;

		if (gMassInfo.sCBW.u8OPCode == UFI_WRITE_10)
		{
			lba = get_be32(&gMassInfo.sCBW.au8Data[0]);
			len = gMassInfo.sCBW.dCBWDataTransferLength;

			if (gMassInfo.sCBW.bCBWLUN == gMassInfo.F_DATA_FLASH_LUN)
			{
			    len = gMassInfo.sCBW.dCBWDataTransferLength - (gMassInfo.DataFlashStartAddr - lba) * UDC_SECTOR_SIZE ;
			    if (len)
			    {
				    SpiWrite(gMassInfo.DataFlashStartAddr, len, (uint8_t*)STORAGE_DATA_BUF);
				}
		    }
		}

		gMassInfo.BulkState = BULK_IN;
		USB_EpAck0();
	}
}



uint8_t Flash_Identify(uint8_t tLUN)
{
    if (tLUN == gMassInfo.F_DATA_FLASH_LUN)
    {
        gMassInfo.dataFlashTotalSectors = GetLogicSector();
        
		if(gMassInfo.dataFlashTotalSectors < 0)
        {
            /* cannot read format */
			gMassInfo.SenseKey[0] = 0x03;
			gMassInfo.SenseKey[1] = 0x30;
			gMassInfo.SenseKey[2] = 0x01;
            return FALSE;  
        }
    }

    return TRUE;
}


uint8_t udcFlashInit(void)
{
	/* control SD power pin */
	GPIOE->PMD |= GPIO_PMD_PMD6_OUTPUT;
	GPIOBITE->GP_BIT6 = 1;
	GPIOBITE->GP_BIT6 = 0;

	if(SDCARD_Open() == E_SUCCESS)
	{		
		printf("SDCard Open success\n");
	}
	else
	{
		printf("SDCard Open failed\n");
		return 1;
	}

    gMassInfo.F_DATA_FLASH_LUN = 0;
    
	if (!Flash_Identify(gMassInfo.F_DATA_FLASH_LUN))
        return 0;

    return 1;
}

/*********************************************************/
/**   
  * @brief  HID_MainProcess, HID mouse main process.
  * @param  None.
  * @retval None.
  */

int32_t UDC_MainProcess(void)
{
	STR_USBD_T sParam;

	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_MASS_STORAGE;
	sParam.u32CtrlMaxPacketSize = MAX_PACKET_SIZE_CTRL;
	sParam.u32EndpointCount = 2;
	sParam.EP[0].u32EpNum = BULK_IN_EP_NUM;
	sParam.EP[0].u32MaxPacketSize = MAX_PACKET_SIZE_BULK;
	sParam.EP[0].IsDirIN = TRUE;
	sParam.EP[0].IsIsoTransfer = FALSE;
	sParam.EP[1].u32EpNum = BULK_OUT_EP_NUM;
	sParam.EP[1].u32MaxPacketSize = MAX_PACKET_SIZE_BULK;
	sParam.EP[1].IsDirIN = FALSE;
	sParam.EP[1].IsIsoTransfer = FALSE;
	USB_Open(&sParam);

	/* Initialize mass storage information */
	memset((void *)&gMassInfo, 0, sizeof(STR_USBD_MASS_T));
	gMassInfo.BulkState = BULK_CBW;
	gMassInfo.F_DATA_FLASH_LUN = 0xFF;
	gMassInfo.sCSW.dCSWSignature = CSW_SIGNATURE;
	gMassInfo.BulkBuf0 = USBD->BUFSEG3;
	gMassInfo.BulkBuf1 = USBD->BUFSEG2;

	/* Initialize mass storage device */
	udcFlashInit();
    
	while(1)
	{
    }
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
