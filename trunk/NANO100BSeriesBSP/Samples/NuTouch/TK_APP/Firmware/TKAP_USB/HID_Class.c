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
#include "nano1xx_tk.h"
#include "HID_Class.h"
#include "nano1xx_tk2usb.h"

/* Define the vendor id and product id */
#define USB_VID			0x0416  /* Vendor ID */
#define USB_PID			0x5020  /* Product ID */


/* Define EP maximum packet size */
#define	HID_MAX_PACKET_SIZE_CTRL        8
#define HID_MAX_PACKET_SIZE_INT_IN		64
#define HID_MAX_PACKET_SIZE_INT_OUT     64


/* Define the interrupt In EP number */
#define HID_IN_EP_NUM		4
#define HID_OUT_EP_NUM		5


/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL		1
#define HID_IS_SELF_POWERED         	0
#define HID_IS_REMOTE_WAKEUP        	0
#define HID_MAX_POWER               	50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

#define LEN_CONFIG_AND_SUBORDINATE (LEN_CONFIG+LEN_INTERFACE+LEN_HID+LEN_ENDPOINT*2)

uint8_t g_au8DeviceReport[HID_MAX_PACKET_SIZE_INT_IN];
const uint32_t g_u32DeviceReportSize = sizeof(g_au8DeviceReport) / sizeof(g_au8DeviceReport[0]);

/*!<USB HID Report Descriptor */
const uint8_t HID_DeviceReportDescriptor[] =
{

 	0x05, 0x01, /* USAGE_PAGE (Generic Desktop)*/
 	0x09, 0x00, /* USAGE (0)*/
 	0xA1, 0x01, /* COLLECTION (Application)*/
 	0x15, 0x00, /*     LOGICAL_MINIMUM (0)*/
 	0x25, 0xFF, /*     LOGICAL_MAXIMUM (255)*/
 	0x19, 0x01, /*     USAGE_MINIMUM (1)*/
 	0x29, 0x08, /*     USAGE_MAXIMUM (8) */
 	0x95, HID_MAX_PACKET_SIZE_INT_OUT, /*     REPORT_COUNT (8)*/
 	0x75, 0x08, /*     REPORT_SIZE (8)*/
 	0x81, 0x02, /*     INPUT (Data,Var,Abs)*/
 	0x19, 0x01, /*     USAGE_MINIMUM (1)*/
 	0x29, 0x08, /*     USAGE_MAXIMUM (8) */
 	0x91, 0x02, /*   OUTPUT (Data,Var,Abs)*/
 	0xC0        /* END_COLLECTION*/
};

#define HID_DEVICE_REPORT_DESCRIPTOR_SIZE \
	sizeof (HID_DeviceReportDescriptor) / sizeof(HID_DeviceReportDescriptor[0])

const uint32_t gu32DeviceReportDescriptorSize = HID_DEVICE_REPORT_DESCRIPTOR_SIZE;

#define HID_REPORT_DESCRIPTOR_SIZE 		HID_DEVICE_REPORT_DESCRIPTOR_SIZE


/*!<USB Device Descriptor */
const uint8_t USB_DeviceDescriptor[LEN_DEVICE] =
{
	LEN_DEVICE,		    		/* bLength 				*/
	DESC_DEVICE,	    		/* bDescriptorType 		*/
	0x10, 0x01,		    		/* bcdUSB 				*/
	0x00,			    		/* bDeviceClass 		*/
	0x00,			    		/* bDeviceSubClass 		*/
	0x00,			    		/* bDeviceProtocol 		*/
	HID_MAX_PACKET_SIZE_CTRL,	/* bMaxPacketSize0 	    */
	USB_VID & 0x00FF,			/* Vendor ID 			*/
	(USB_VID & 0xFF00) >> 8,
	USB_PID & 0x00FF,			/* Product ID 			*/
	(USB_PID & 0xFF00) >> 8,
	0x00, 0x00,		    		/* bcdDevice 			*/
	0x01,			    		/* iManufacture 		*/
	0x02,			    		/* iProduct				*/
	0x00,						/* iSerialNumber - no serial */
	0x01			    		/* bNumConfigurations 	*/
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
	0xC0,			/* bmAttributes */
	0x32,			/* MaxPower */

 	LEN_INTERFACE,
 	DESC_INTERFACE,
 	0x00,           /* bInterfaceNumber */
 	0x00,           /* bAlternateSetting */
 	0x02,           /* bNumEndpoints */
 	0x03,           /* bInterfaceClass */
 	0x00,           /* bInterfaceSubClass */
 	0x00,   		/* bInterfaceProtocol */
 	0x00,           /* iInterface */

 	LEN_HID,        /* bLength */
 	DESC_HID,       /* bDescriptorType */
 	0x10, 0x01,     /* bcdHID */
 	0x00,           /* bCountryCode */
 	0x01,           /* bNumDescriptor */
 	DESC_HID_RPT,   /* bDescriptorType */
	HID_REPORT_DESCRIPTOR_SIZE & 0x00FF,            /* wDescriptorLen */
	(HID_REPORT_DESCRIPTOR_SIZE & 0xFF00) >> 8,
	
 	LEN_ENDPOINT,                                   /* bLength */
 	DESC_ENDPOINT,                                  /* bDescriptorType */
 	HID_IN_EP_NUM | 0x80,                           /* bEndpointAddress */
 	EP_INT,                                         /* bmAttributes */
 	HID_MAX_PACKET_SIZE_INT_IN & 0x00FF,            /* wMaxPacketSize */
	(HID_MAX_PACKET_SIZE_INT_IN & 0xFF00) >> 8,
 	HID_DEFAULT_INT_IN_INTERVAL,                    /* bInterval */

 	LEN_ENDPOINT,                                   /* bLength */
 	DESC_ENDPOINT,                                  /* bDescriptorType */
 	HID_OUT_EP_NUM,                                 /* bEndpointAddress */
 	EP_INT,                                         /* bmAttributes */
	HID_MAX_PACKET_SIZE_INT_OUT & 0x00FF,           /* wMaxPacketSize */
	(HID_MAX_PACKET_SIZE_INT_OUT & 0xFF00) >> 8,
 	HID_DEFAULT_INT_IN_INTERVAL                     /* bInterval */
};

/*!<USB Language String Descriptor */
const uint8_t USB_StringLang[] =
{
	4,				/* bLength                  */
	DESC_STRING,	/* bDescriptorType          */
	0x09, 0x04      /* Language ID: USA(0x0409) */
};

/*!<USB Vendor String Descriptor */
const uint8_t USB_VendorStringDesc[] =
{
	16,             /* bLength          */
	DESC_STRING,    /* bDescriptorType  */
	'N', 0,
	'u', 0,
	'v', 0,
	'o', 0,
	't', 0,
	'o', 0,
	'n', 0
};

/*!<USB Product String Descriptor */
const uint8_t USB_ProductStringDesc[] =
{
	26,             /* bLength          */
	DESC_STRING,    /* bDescriptorType  */
	'H', 0,
	'I', 0,
	'D', 0,
	' ', 0,
	'T', 0,
	'r', 0,
	'a', 0,
	'n', 0,
	's', 0,
	'f', 0,
	'e', 0,
	'r', 0
};

/*********************************************************/
/* HID Transfer Function                                 */
/*********************************************************/
void HID_GetInReport(void);
void HID_SetOutReport(uint8_t *pu8EpBuf, uint32_t u32Size);

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
			gUsbCtrl.ReqLen = Minimum (gUsbCtrl.ReqLen, HID_REPORT_DESCRIPTOR_SIZE);
			if (gUsbCtrl.ReqLen >= CtrlMaxPacketSize)
			{
				my_memcpy(pCtrl, (void *)HID_DeviceReportDescriptor, CtrlMaxPacketSize);
				gUsbCtrl.pDesc = (uint8_t *)&HID_DeviceReportDescriptor[CtrlMaxPacketSize];
				gUsbCtrl.EP0_Toggle = 1;
				USBD->MXPLD0 = CtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= CtrlMaxPacketSize;
				gUsbCtrl.StateFlag = FLAG_EP0_DATA_IN;
			}
			else
			{
				my_memcpy(pCtrl, (void *)HID_DeviceReportDescriptor, gUsbCtrl.ReqLen);
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
	/* Interrupt IN */
	HID_GetInReport();
	return 0;
}

/**
  * @brief  USB_EpAck1, Ack Transfer Pipe.
  * @param  None.
  * @retval None.
  */
uint32_t USB_EpAck1(void)
{
	uint8_t *ptr;
	/* Interrupt OUT */
	ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG3);
	HID_SetOutReport(ptr, USBD->MXPLD3);
	USBD->MXPLD3 = HID_MAX_PACKET_SIZE_INT_OUT;
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
	/* receive OUT data */
	USBD->MXPLD3 = HID_MAX_PACKET_SIZE_INT_OUT;
}

/*********************************************************/
/**
  * @brief  HID_MainProcess, HID mouse main process.
  * @param  None.
  * @retval None.
  */
 extern void env_test(void);
int32_t HID_MainProcess(void)
{
	STR_USBD_T sParam;

	USBD_Init();

	sParam.UsbDeviceClass = USB_CLASS_HID;
	sParam.u32CtrlMaxPacketSize = HID_MAX_PACKET_SIZE_CTRL;
	sParam.u32EndpointCount = 2;
	sParam.EP[0].u32EpNum = HID_IN_EP_NUM;
	sParam.EP[0].u32MaxPacketSize = HID_MAX_PACKET_SIZE_INT_IN;
	sParam.EP[0].IsDirIN = TRUE;
	sParam.EP[0].IsIsoTransfer = FALSE;
	sParam.EP[1].u32EpNum = HID_OUT_EP_NUM;
	sParam.EP[1].u32MaxPacketSize = HID_MAX_PACKET_SIZE_INT_OUT;
	sParam.EP[1].IsDirIN = FALSE;
	sParam.EP[1].IsIsoTransfer = FALSE;
	USB_Open(&sParam);


	env_test();
	return 0;

}


/***************************************************************/
#define HID_CMD_SIGNATURE   0x43444948

/* HID Transfer Commands */
#define HID_CMD_NONE     0x00
#define HID_CMD_ERASE    0x71
#define HID_CMD_READ     0xD2
#define HID_CMD_WRITE    0xC3
#define HID_CMD_TEST     0xB4
#define HID_CMD_TKREAD   0xC4
#define HID_CMD_TKWRITE  0x51


#define OP_GET_CONFIG			0x10
#define OP_SET_CHANNEL_CONFIG	0x11
#define OP_START_RECORD			0x12
#define OP_STOP_RECORD			0x13
#define OP_LISTEN_STATUS		0x14
#define OP_AUTO_CHANNEL_CONFIG  0x15


#define PAGE_SIZE		 256

typedef __packed struct {
    uint8_t u8Cmd;
    uint8_t u8Size;
    uint32_t u32Arg1;
    uint32_t u32Arg2;
    uint32_t u32Signature;
    uint32_t u32Checksum;
}CMD_T;

CMD_T gCmd;

static uint8_t  g_u8PageBuff[PAGE_SIZE] = {0};    /* Page buffer to upload/download through HID report */
static uint32_t g_u32BytesInPageBuf = 0;          /* The bytes of data in g_u8PageBuff */



int32_t HID_CmdEraseSectors(CMD_T *pCmd)
{
    uint32_t u32StartSector;
    uint32_t u32Sectors;
    uint32_t i;

    u32StartSector = pCmd->u32Arg1;
    u32Sectors = pCmd->u32Arg2;

	printf("Erase command - Sector: %d   Sector Cnt: %d\n", u32StartSector, u32Sectors);
	
	for(i=0;i<u32Sectors;i++)
	{
	    printf("Erase sector - %d\n",u32StartSector + i);
		/* TODO: To erase the sector of storage */
	
	}
	/* To note the command has been done */
	pCmd->u8Cmd = HID_CMD_NONE;
	
	return 0;
}


int32_t HID_CmdReadPages(CMD_T *pCmd)
{
    uint32_t u32StartPage;
    uint32_t u32Pages;
    int32_t i;

    u32StartPage = pCmd->u32Arg1;
    u32Pages     = pCmd->u32Arg2;

	printf("Read command - Start page: %d    Pages Numbers: %d\n", u32StartPage, u32Pages);

    if(u32Pages)
    {
        /* Update data to page buffer to upload */
		/* TODO: We need to update the page data if got a page read command. (0xFF is used in this sample code) */
        for(i=0;i<PAGE_SIZE;i++)
            g_u8PageBuff[i] = 0xFF;
        g_u32BytesInPageBuf = PAGE_SIZE;

        /* The signature word is used as page counter */
        pCmd->u32Signature = 1;

	    /* Trigger HID IN */
		my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)g_u8PageBuff, HID_MAX_PACKET_SIZE_INT_IN);
		USBD->MXPLD2 = HID_MAX_PACKET_SIZE_INT_IN;
		g_u32BytesInPageBuf-= HID_MAX_PACKET_SIZE_INT_IN;
    }

	return 0;
}

extern void TK_GetConfig(uint8_t *SendBuf,uint32_t *SendLen,uint8_t set); //shanchun
extern void TK_SetConfig(uint8_t *ReceiveBuf,uint32_t ReceiveLen);  //shanchun
extern void TK_ListenStatus(uint32_t CNum,uint8_t *SendBuf,uint32_t *SendLen); //shanchun
extern void Get_TK_Data(uint16_t ch);

extern uint8_t AutoCh;
extern uint8_t AutoType;
extern uint8_t AutoProgress;
extern best_cfg final_result;

int32_t HID_CmdTKRead(CMD_T *pCmd)	 //shanchun20121116
{
    uint32_t u32OpCode;
    uint32_t u32Num;	

	uint8_t ch,type;
	TK_CNANNEL_CONFIG *ptk;
    u32OpCode 	= pCmd->u32Arg1;
    u32Num   = pCmd->u32Arg2;
	
	
	printf("u32OpCode: %08x   u32Num: 0x%08x\n",u32OpCode, u32Num);

    switch(u32OpCode)
	{
		case OP_GET_CONFIG:
	    {

					TK_GetConfig(g_u8PageBuff,&g_u32BytesInPageBuf,u32Num);						
	        		/* The signature word is used as page counter */
	        		pCmd->u32Signature = 1;	
		    		/* Trigger HID IN */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)g_u8PageBuff, HID_MAX_PACKET_SIZE_INT_IN);
					USBD->MXPLD2 = HID_MAX_PACKET_SIZE_INT_IN;
					//g_u32BytesInPageBuf-= HID_MAX_PACKET_SIZE_INT_IN;
					g_u32BytesInPageBuf=0;

			break;
	    }

		case OP_LISTEN_STATUS:
		{				
				   TK_ListenStatus(u32Num,g_u8PageBuff,&g_u32BytesInPageBuf);
		  		   /* The signature word is used as page counter */
	        		pCmd->u32Signature = 1;	
		    		/* Trigger HID IN */
					my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)g_u8PageBuff, HID_MAX_PACKET_SIZE_INT_IN);
					USBD->MXPLD2 = HID_MAX_PACKET_SIZE_INT_IN;
					//g_u32BytesInPageBuf-= HID_MAX_PACKET_SIZE_INT_IN;
					g_u32BytesInPageBuf=0;
			break;
		}

		case OP_AUTO_CHANNEL_CONFIG:
		{
			
			ch=(pCmd->u32Arg2 & 0xff00)>>8;			
			type=(pCmd->u32Arg2 & 0x00ff);			
			//printf("======type=%d  AutoType=%d\n",type,AutoType);
			switch(type)
			{
				case 0:
					g_u8PageBuff[0]=AutoType;				
					g_u8PageBuff[1]=AutoProgress;
				break;
				case 1:					
					AutoCh=ch;
					AutoType=0x1;
				break;

				case 2:
					AutoCh=ch;
					AutoType=0x2;
				break;

				case 3:
					ptk=(TK_CNANNEL_CONFIG *)g_u8PageBuff;
					ptk->channel=AutoCh;					
					ptk->base=final_result.base;
					ptk->diff=final_result.diff;
					ptk->current=final_result.current;
					ptk->div=final_result.div;
					AutoProgress=0;
					printf("ptk.channel=0x%02x\n",ptk->channel);
					printf("ptk.base=0x%04x\n",ptk->base);
					printf("ptk.diff=0x%04x\n",ptk->diff);
					printf("ptk.current=0x%02x\n",ptk->current);
					printf("ptk.div=0x%02x\n",ptk->div);							
				break;

				default:
				break;
			}		
			 /* The signature word is used as page counter */
	        pCmd->u32Signature = 1;
				/* Trigger HID IN */
			my_memcpy((uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2), (void *)g_u8PageBuff, HID_MAX_PACKET_SIZE_INT_IN);
			USBD->MXPLD2 = HID_MAX_PACKET_SIZE_INT_IN;
			//g_u32BytesInPageBuf-= HID_MAX_PACKET_SIZE_INT_IN;
			g_u32BytesInPageBuf=0;						
			break;
		}

		default:
		break;
	}
	return 0;
}


int32_t HID_CmdWritePages(CMD_T *pCmd)
{
    uint32_t u32StartPage;
    uint32_t u32Pages;

    u32StartPage = pCmd->u32Arg1;
    u32Pages     = pCmd->u32Arg2;

	printf("Write command - Start page: %d    Pages Numbers: %d\n", u32StartPage, u32Pages);
    g_u32BytesInPageBuf = 0;

    /* The signature is used to page counter */
    pCmd->u32Signature = 0;

	return 0;
}

int32_t HID_CmdTKWrite(CMD_T *pCmd)	 //shanchun20121116
{

    g_u32BytesInPageBuf = 0;



    /* The signature is used to page counter */
    pCmd->u32Signature = 0;

	return 0;
}


int32_t gi32CmdTestCnt = 0;
int32_t HID_CmdTest(CMD_T *pCmd)
{
    int32_t i;
    uint8_t *pu8;

    pu8 = (uint8_t *)pCmd;
    printf("Get test command #%d (%d bytes)\n", gi32CmdTestCnt++, pCmd->u8Size);
    for(i=0;i<pCmd->u8Size;i++)
    {
        if((i&0xF) == 0)
        {
            printf("\n");
        }
        printf(" %02x", pu8[i]);
    }

    printf("\n");


	/* To note the command has been done */
	pCmd->u8Cmd = HID_CMD_NONE;
	
	return 0;
}


uint32_t CalCheckSum(uint8_t *buf, uint32_t size)
{
    uint32_t sum;
    int32_t i;

    i = 0;
    sum = 0;
    while(size--)
    {
        sum+=buf[i++];
    }

    return sum;

}


int32_t ProcessCommand(uint8_t *pu8Buffer, uint32_t u32BufferLen)
{
	uint32_t u32sum;
	printf("ProcessCommand\n");  //shanchun20121116
	
    my_memcpy((uint8_t *)&gCmd, pu8Buffer, u32BufferLen);

    /* Check size */
    if((gCmd.u8Size > sizeof(gCmd)) || (gCmd.u8Size > u32BufferLen))
        return -1;

    /* Check signature */
    if(gCmd.u32Signature != HID_CMD_SIGNATURE)
        return -1;

    /* Calculate checksum & check it*/
    u32sum = CalCheckSum((uint8_t *)&gCmd, gCmd.u8Size);
    if(u32sum != gCmd.u32Checksum)
        return -1;

	switch(gCmd.u8Cmd)
	{
		case HID_CMD_ERASE:
		{
			HID_CmdEraseSectors(&gCmd);
			break;
		}		
		case HID_CMD_READ:
		{
			HID_CmdReadPages(&gCmd);
			break;
		}		
		case HID_CMD_WRITE:
		{
			HID_CmdWritePages(&gCmd);
			break;		
		}
		case HID_CMD_TEST:
		{
		    HID_CmdTest(&gCmd);
		    break;
		}

		case HID_CMD_TKWRITE:
		{
			HID_CmdTKWrite(&gCmd);
			break;
		}

		case HID_CMD_TKREAD:
		{
			HID_CmdTKRead(&gCmd);
			break;
		}
		default:
			return -1;
	}	
	
	return 0;
}


void HID_SetOutReport(uint8_t *pu8EpBuf, uint32_t u32Size)
{
    uint8_t  u8Cmd;
    uint32_t u32StartPage;
    uint32_t u32Pages;
    uint32_t u32PageCnt;

    /* Get command information */
    u8Cmd        = gCmd.u8Cmd;
    u32StartPage = gCmd.u32Arg1;
    u32Pages     = gCmd.u32Arg2;
    u32PageCnt   = gCmd.u32Signature; /* The signature word is used to count pages */


    /* Check if it is in the data phase of write command */
    if((u8Cmd == HID_CMD_WRITE) &&  (u32PageCnt < u32Pages))
    {
        /* Process the data phase of write command */


        /* Get data from HID OUT */
		my_memcpy(&g_u8PageBuff[g_u32BytesInPageBuf], pu8EpBuf, HID_MAX_PACKET_SIZE_INT_OUT);
		g_u32BytesInPageBuf += HID_MAX_PACKET_SIZE_INT_OUT;

        /* The HOST must make sure the data is PAGE_SIZE alignment */
		if(g_u32BytesInPageBuf >= PAGE_SIZE)
		{
		    printf("Writing page %d\n", u32StartPage + u32PageCnt);
			/* TODO: We should program received data to storage here */
			u32PageCnt++;
		
		    /* Write command complete! */
			if(u32PageCnt >= u32Pages)
			{
				u8Cmd = HID_CMD_NONE;	
		
		        printf("Write command complete.\n");
		    }
  			g_u32BytesInPageBuf = 0;
		}

		/* Update command status */
		gCmd.u8Cmd        = u8Cmd;
		gCmd.u32Signature = u32PageCnt;
    }else 	
	if(u8Cmd == HID_CMD_TKWRITE)   //shanchun
	{
	  	printf("u8Cmd == HID_CMD_TKWRITE\n") ;	
	    /* Get data from HID OUT */
		my_memcpy(&g_u8PageBuff[g_u32BytesInPageBuf], pu8EpBuf, HID_MAX_PACKET_SIZE_INT_OUT);
		g_u32BytesInPageBuf += HID_MAX_PACKET_SIZE_INT_OUT;

	  	switch(gCmd.u32Arg1)
		{
			case OP_SET_CHANNEL_CONFIG:				
				TK_SetConfig(g_u8PageBuff,gCmd.u32Arg2);
			break;

			case OP_LISTEN_STATUS:
				
			break;
			default:
			break;											
		}
		g_u32BytesInPageBuf = 0;	   	

		/* Update command status */
		gCmd.u8Cmd        = HID_CMD_NONE;	
		gCmd.u32Signature = 0;

	}
    else	
    {
        /* Check and process the command packet */
        if(ProcessCommand(pu8EpBuf, u32Size))
        {
            printf("Unknown HID command!\n");
        }
    }
}

void HID_GetInReport(void)
{
    uint32_t u32StartPage;
    uint32_t u32TotalPages;
    uint32_t u32PageCnt;
	uint8_t *ptr;
	int32_t i;
    uint8_t u8Cmd;

    u8Cmd        = gCmd.u8Cmd;
    u32StartPage = gCmd.u32Arg1;
    u32TotalPages= gCmd.u32Arg2;
    u32PageCnt   = gCmd.u32Signature;

    /* Check if it is in data phase of read command */
	if(u8Cmd == HID_CMD_READ)
	{
	    /* Process the data phase of read command */
        if((u32PageCnt >= u32TotalPages) && (g_u32BytesInPageBuf == 0))
        {
    		/* The data transfer is complete. */
            u8Cmd = HID_CMD_NONE;
            printf("Read command complete!\n");
        }
	    else
	    {
	        if(g_u32BytesInPageBuf == 0)
	        {
	            /* The previous page has sent out. Read new page to page buffer */
				/* TODO: We should update new page data here. (0xFF is used in this sample code) */
				printf("Reading page %d\n", u32StartPage + u32PageCnt);
		        for(i=0;i<PAGE_SIZE;i++)
		            g_u8PageBuff[i] = 0xFF;
					
	            g_u32BytesInPageBuf = PAGE_SIZE;
	
	            /* Update the page counter */
	            u32PageCnt++;
	        }
	
            /* Prepare the data for next HID IN transfer */	
			ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG2);
			my_memcpy(ptr, (void *)&g_u8PageBuff[PAGE_SIZE - g_u32BytesInPageBuf], HID_MAX_PACKET_SIZE_INT_IN);
			USBD->MXPLD2 = HID_MAX_PACKET_SIZE_INT_IN;
    		g_u32BytesInPageBuf -= HID_MAX_PACKET_SIZE_INT_IN;
		}
	}
	
	gCmd.u8Cmd        = u8Cmd;
	gCmd.u32Signature = u32PageCnt;
	
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
