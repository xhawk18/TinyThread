/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "Driver\DrvUSB.h"
#include "USB\VCOMSys.h"
#include "VCOM_API.h"

//#define DEBUG

#ifdef 	DEBUG
# define DBG_PRINTF			printf
#else
# define DBG_PRINTF(...)  
#endif

#define Maximum(a,b)		((a)>(b) ? (a) : (b))
#define Minimum(a,b)		((a)<(b) ? (a) : (b))


/* Endpoint description */
S_DRVUSB_EP_CTRL sEpDescription[] = 
{
	{CTRL_EP_NUM    | EP_INPUT,  VCOM_CTRL_MXPLD, NULL},
	{CTRL_EP_NUM    | EP_OUTPUT, VCOM_CTRL_MXPLD, NULL},
	{BULK_IN_EP_NUM | EP_INPUT,  BULK_EP_MXPLD, NULL},
	{BULK_OUT_EP_NUM| EP_OUTPUT, BULK_EP_MXPLD, NULL},
	{INT_IN_EP_NUM  | EP_INPUT,  INT_EP_MXPLD, NULL},
	{0,0,0}
};

/* Bus event call back functions */
S_DRVUSB_EVENT_PROCESS g_sBusOps[6] = 
{
	{NULL, NULL},                               /* Attach event callback        */
	{NULL, NULL},                               /* Detach event callback        */
	{DrvUSB_BusResetCallback, &gsVcomDevice},   /* Bus reset event callback     */
	{NULL, NULL},                               /* Bus suspend event callback   */
	{NULL, NULL},                               /* Bus resume event callback    */
	{DrvUSB_CtrlSetupAck, &gsVcomDevice},       /* Setup event callback         */
};

/* USB event call back functions */
S_DRVUSB_EVENT_PROCESS g_sUsbOps[12] = 
{
	{DrvUSB_CtrlDataInAck, &gsVcomDevice},
	{DrvUSB_CtrlDataOutAck, &gsVcomDevice},
	
	{VCOM_BulkInAckCallback, &gsVcomDevice},
	{NULL, NULL},

	{NULL, NULL},
	{VCOM_BulkOutAckCallback, &gsVcomDevice},

	{VCOM_IntInAckCallback, &gsVcomDevice},
	{NULL, NULL},
	
	{NULL, NULL},
	{NULL, NULL},
	
	{NULL, NULL},
	{NULL, NULL},
};


/* ctrl pipe call back.                                                                   */
/* it will be call by DrvUSB_CtrlSetupAck, DrvUSB_CtrlDataInAck and DrvUSB_CtrlDataOutAck */
/* if in ack handler and out ack handler is 0, default handler will be called             */
S_DRVUSB_CTRL_CALLBACK_ENTRY g_asCtrlCallbackEntry[] = {
    //request type,command     ,setup ack handler         , in ack handler      ,out ack handler,  parameter
	{REQ_STANDARD, SET_ADDRESS, DrvUSB_CtrlSetupSetAddress, DrvUSB_CtrlDataInSetAddress, 0, &gsVcomDevice},
	{REQ_STANDARD, CLEAR_FEATURE, DrvUSB_CtrlSetupClearSetFeature, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, SET_FEATURE, DrvUSB_CtrlSetupClearSetFeature, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, GET_CONFIGURATION, DrvUSB_CtrlSetupGetConfiguration, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, GET_STATUS, DrvUSB_CtrlSetupGetStatus, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, GET_INTERFACE, DrvUSB_CtrlSetupGetInterface, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, SET_INTERFACE, DrvUSB_CtrlSetupSetInterface, 0, 0, &gsVcomDevice},
	{REQ_STANDARD, GET_DESCRIPTOR, VCOM_CtrlSetupGetDescriptor, VCOM_CtrlDataInGetDescriptor, 0, &gsVcomDevice},
	{REQ_STANDARD, SET_CONFIGURATION, VCOM_CtrlSetupSetConfiguration, 0, 0, &gsVcomDevice},
	{REQ_CLASS, SET_LINE_CODE, VCOM_SetLineCoding, 0, VCOM_SetLineCodingDataOut, &gsVcomDevice},
    {REQ_CLASS, GET_LINE_CODE, VCOM_GetLineCoding, 0, 0, &gsVcomDevice}, 
    {REQ_CLASS, SET_CONTROL_LINE_STATE, VCOM_CtrlLineState, 0, 0, &gsVcomDevice}
};


S_VCOM_DEVICE gsVcomDevice;

//the structure registered to USB driver
S_DRVUSB_CLASS sVcomClass = 
{
	(void*)&gsVcomDevice, 
	NULL,
	NULL
};


/* Line coding structure
  0-3 dwDTERate    Data terminal rate (baudrate), in bits per second
  4   bCharFormat  Stop bits: 0 - 1 Stop bit, 1 - 1.5 Stop bits, 2 - 2 Stop bits
  5   bParityType  Parity:    0 - None, 1 - Odd, 2 - Even, 3 - Mark, 4 - Space
  6   bDataBits    Data bits: 5, 6, 7, 8, 16  */

typedef struct { 
    uint32_t  u32DTERate;     /* Baud rate    */
    uint8_t   u8CharFormat;   /* stop bit     */
    uint8_t   u8ParityType;   /* parity       */
    uint8_t   u8DataBits;     /* data bits    */
} S_VCOM_LINE_CODING;


//S_VCOM_LINE_CODING gLineCoding = {0};
S_VCOM_LINE_CODING gLineCoding = {115200, 0, 0, 8};    /* Baud rate : 115200    */
													   /* Stop bit     */
       												   /* parity       */
  													   /* data bits    */
   
      

uint16_t gCtrlSignal = 0;     /* BIT0: DTR(Data Terminal Ready) , BIT1: RTS(Request To Send) */

/* To handle the data transfer size > maximum packet size */
static uint16_t gu16TransferLen = 0;
static uint16_t gu16PacketLen = 0;
static uint16_t gu16TransferIndex = 0;


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_CtrlDataInGetDescriptor                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The USB get descriptor  data in event callback function.                                           */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_CtrlDataInGetDescriptor(void* pVoid)
{
	S_DRVUSB_DEVICE *psDevice = &gsUsbDevice;
	
	if (psDevice->au8Setup[0] & 0x80)
	{
		if(gu16TransferLen)
		{
			if(gu16TransferLen >= gu16PacketLen)
			{		
				DrvUSB_DataIn(0, gau8ConfigDescriptor + gu16TransferIndex, gu16PacketLen);
				gu16TransferLen -= gu16PacketLen;
				gu16TransferIndex += gu16PacketLen;
			}
			else
			{
				DrvUSB_DataIn(0, gau8ConfigDescriptor + gu16TransferIndex, gu16TransferLen);
				gu16TransferLen = 0;
			}
			
			if (gu16TransferLen == 0)
				_DRVUSB_TRIG_EP(1, 0x00); //Ctl data out
		}
		else
		{
			_DRVUSB_TRIG_EP(1, 0x00);
		}
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_CtrlSetupGetDescriptor                                                                   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The USB get descriptor event callback function.                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_CtrlSetupGetDescriptor(void* pVoid)
{
    S_VCOM_DEVICE *psVcomDevice = (S_VCOM_DEVICE *)pVoid;
	S_DRVUSB_DEVICE *psDevice = (S_DRVUSB_DEVICE *)psVcomDevice->device;
	
	uint16_t u16Len, u16ConfigLen;
	
	u16Len = 0;
	u16Len = psDevice->au8Setup[7];
	u16Len <<= 8;
	u16Len += psDevice->au8Setup[6];

	switch (psDevice->au8Setup[3])
	{
		/* Get Device Descriptor */
		case DESC_DEVICE:
		{
			u16Len = Minimum(u16Len, LEN_DEVICE);

			DrvUSB_DataIn(0, psVcomDevice->au8DeviceDescriptor, u16Len);
			break;
		}

		/* Get Configuration Descriptor	*/
		case DESC_CONFIG:
		{
			u16ConfigLen = psVcomDevice->au8ConfigDescriptor[3];
			u16ConfigLen <<= 8;
			u16ConfigLen |= psVcomDevice->au8ConfigDescriptor[2];

			gu16TransferLen = Minimum(u16Len, u16ConfigLen);
			gu16PacketLen = 64;
			gu16TransferIndex = 0;		

			if(gu16TransferLen > gu16PacketLen)
			{
				DrvUSB_DataIn(0, psVcomDevice->au8ConfigDescriptor + gu16TransferIndex, gu16PacketLen);
				gu16TransferLen -= gu16PacketLen;
				gu16TransferIndex += gu16PacketLen;
			}
			else
			{
				DrvUSB_DataIn(0, psVcomDevice->au8ConfigDescriptor + gu16TransferIndex, gu16TransferLen);
				gu16TransferLen = 0;
			}
			break;
		}

		/* Get String Descriptor */
		case DESC_STRING:
		{
			/* Get Language	*/
			if (psDevice->au8Setup[2] == 0)
			{
				u16Len = Minimum(u16Len, 4);
				DrvUSB_DataIn(0, gau8StringLang, u16Len);
			}
			else
			{
				/* Get String Descriptor */
				switch (psDevice->au8Setup[2])
				{
					case 1:
						u16Len = Minimum(u16Len, psVcomDevice->sVendorStringDescriptor[0]);
				
						DrvUSB_DataIn(0, (const uint8_t *)psVcomDevice->sProductStringDescriptor, u16Len);
						break;
					case 2:
						u16Len = Minimum(u16Len, psVcomDevice->sProductStringDescriptor[0]);
						DrvUSB_DataIn(0, (const uint8_t *)psVcomDevice->sProductStringDescriptor, u16Len);
						break;
					case 3:
						u16Len = Minimum(u16Len, gau8StringSerial[0]);
						DrvUSB_DataIn(0, gau8StringSerial, u16Len);
						break;
					default:
						// Not support. Reply STALL.
						DrvUSB_ClrCtrlReadyAndTrigStall();
				}
			}
			break;
		}
		default:
			// Not support. Reply STALL.
			DrvUSB_ClrCtrlReadyAndTrigStall();
	}
	
	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_SetLineCoding                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The callback function when get SET_LINE_CODING request.                                            */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_SetLineCoding(void* pVoid)
{
    S_DRVUSB_DEVICE *psDevice = (S_DRVUSB_DEVICE *)((S_VCOM_DEVICE *)pVoid)->device;
    DBG_PRINTF("Set line coding\n");
    psDevice->abData0[0] = FALSE;
    DrvUSB_DataOutTrigger(0, 7);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_SetLineCodingDataOut                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The callback function when get the out data of set line coding request.                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_SetLineCodingDataOut(void* pVoid)
{
    S_DRVUSB_DEVICE *psDevice = (S_DRVUSB_DEVICE *)((S_VCOM_DEVICE *)pVoid)->device;
    uint8_t *buf;
    uint32_t size;
    
    buf = DrvUSB_GetOutData(0, &size);
    memcpy((void *)&gLineCoding, buf, size);
    
    /* ACK to end the transfer */
    psDevice->abData0[0] = FALSE;
    DrvUSB_DataIn(0, 0, 0);

    DBG_PRINTF("Baudrate : %d\n", gLineCoding.u32DTERate);
    DBG_PRINTF("Stop bit : %s\n", (gLineCoding.u8CharFormat==2)?"2-bit":((gLineCoding.u8CharFormat)?"1.5-bit":"1-bit"));
    {
#ifdef 	DEBUG
        char s[5][6] = {"None", "Odd", "Even", "Mark", "Space"};
#endif
    	DBG_PRINTF("Parity   : %s\n", &s[gLineCoding.u8ParityType][0]);
    }
    DBG_PRINTF("Data bits: %d\n", gLineCoding.u8DataBits);  
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_GetLineCoding                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The callback function when get GET_LINE_CODING request.                                            */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_GetLineCoding(void* pVoid)
{
    S_DRVUSB_DEVICE *psDevice = (S_DRVUSB_DEVICE *)((S_VCOM_DEVICE *)pVoid)->device;

    psDevice->abData0[0] = TRUE;
    DrvUSB_DataIn(0, (void *)&gLineCoding, 7);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_CtrlLineState                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The callback function when get SET_CONTROL_LINE_STATE request.                                     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_CtrlLineState(void* pVoid)
{
    S_DRVUSB_DEVICE *psDevice = (S_DRVUSB_DEVICE *)((S_VCOM_DEVICE *)pVoid)->device;
    
    gCtrlSignal = psDevice->au8Setup[3];
    gCtrlSignal = (gCtrlSignal << 8) | psDevice->au8Setup[2];
    
    DrvUSB_DataIn(0, 0, 0); 
    
    DBG_PRINTF("RTS=%d  DTR=%d\n", (gCtrlSignal>>1)&1, gCtrlSignal&1);    
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_CtrlSetupSetConfiguration                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      pVoid - [in],   A pointer to USB class device structure (gsVcomDevice).                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      The callback function when get SET_CONFIGURATION request.                                          */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_CtrlSetupSetConfiguration(void* pVoid)
{
	S_DRVUSB_DEVICE *pDrvDevice = &gsUsbDevice;
	S_DRVUSB_CLASS *psUsbClass = pDrvDevice->psUsbClass;
	int32_t bIsDeviceConfigure;
	
	bIsDeviceConfigure = psUsbClass->pfnCompare ? psUsbClass->pfnCompare(pDrvDevice->au8Setup[2]) : 1;
	
	if (pDrvDevice->au8Setup[2] == 0)
	{
		// USB address state.
		DrvUSB_SetUsbState(eDRVUSB_ADDRESS);
		pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];
		DrvUSB_DataIn(0, NULL, 0);
		
	}
	else if(bIsDeviceConfigure)
	{
		// USB configured state.
		DrvUSB_SetUsbState(eDRVUSB_CONFIGURED);

		// Call USB class's start function
		if(psUsbClass->pfnStart)
			psUsbClass->pfnStart(pVoid);

		pDrvDevice->u8UsbConfiguration = pDrvDevice->au8Setup[2];

		DrvUSB_DataIn(0, NULL, 0);
		
		/* Reset bulk in/out endpoint */
		DrvUSB_DataOutTrigger(BULK_OUT_EP_NUM, 64);
		DrvUSB_DataIn(BULK_IN_EP_NUM, 0, 0);
	}
	else
	{
		// Not support. Reply STALL.
		DrvUSB_ClrCtrlReadyAndTrigStall();
	}	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_GetVersion                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      The virtual com driver version number                                                              */
/*                                                                                                         */
/* Description:                                                                                            */
/*      To return the virtual com drvier version number.                                                   */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t VCOM_GetVersion(void)
{
	return VCOM_VERSION_NUM;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_Open                                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      E_SUCCESS                                                                                          */
/*                                                                                                         */
/* Description:                                                                                            */
/*      To install the virtual com to USB device driver and install the relative descriptors.              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t VCOM_Open(void)
{
    S_VCOM_DEVICE *psVcomDevice = &gsVcomDevice;
	
	/* Install the virtual com to USB device */
	psVcomDevice->device = (void*)DrvUSB_InstallClassDevice(&sVcomClass);
	
	/* Configure the descritors */
	psVcomDevice->au8DeviceDescriptor = gau8DeviceDescriptor;
	psVcomDevice->au8ConfigDescriptor = gau8ConfigDescriptor;	
	psVcomDevice->sVendorStringDescriptor = gau8VendorStringDescriptor;
	psVcomDevice->sProductStringDescriptor = gau8ProductStringDescriptor;
	
	/* Install the USB event callbacks */
	DrvUSB_InstallCtrlHandler(psVcomDevice->device, g_asCtrlCallbackEntry, 
	    sizeof(g_asCtrlCallbackEntry) / sizeof(g_asCtrlCallbackEntry[0]));

	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: VCOM_Close                                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*      None                                                                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*      None                                                                                               */
/*                                                                                                         */
/* Description:                                                                                            */
/*      To close virtual com.                                                                              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void VCOM_Close(void)
{

}

