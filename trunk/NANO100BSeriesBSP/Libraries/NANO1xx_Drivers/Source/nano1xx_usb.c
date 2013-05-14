/******************************************************************************
 * @file     nano1xx_usb.c
 * @brief    NANO1xx USB driver source file
 * @version  1.0.2
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <string.h>
#include "nano1xx_usb.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_USB_Driver USB Driver
  @{
*/

/** @addtogroup NANO1xx_USB_EXPORTED_VARIABLES USB Exported Variables
  @{
*/
/********************* Global Variables **********************/
STR_USBD_T gUsbInfo;
STR_USBD_CTRL_T __IO gUsbCtrl;
STR_USBD_UAC_T gUacInfo;
STR_USBD_UDC_T gUdcInfo;
STR_USBD_CDC_T gCdcInfo;
uint32_t gCtrlMaxPacketSize = 8;
uint32_t gCurrentSramBuffer;
uint32_t gTotalEnpointCount=0;

/*@}*/ /* end of group NANO1xx_USB_EXPORTED_VARIABLES */


/** @addtogroup NANO1xx_USB_EXPORTED_FUNCTIONS USB Exported Functions
  @{
*/

/**   
  * @brief  my_memcpy, Copy bytes.
  * @param  dest: Destination pointer.
  * @param  src: Source pointer.
  * @param  size: copy byte count.
  * @retval None.
  */
void my_memcpy(void *dest, void *src, int32_t size)
{
    int32_t i;

    for (i = 0; i < size; i++)
       *((uint8_t *)dest + i) = *((uint8_t *)src + i);
}


/**   
  * @brief  USBD Initial, Enable clock and reset USB.
  * @param  None.
  * @retval None.
  */
void USBD_Init(void)
{
	uint32_t volatile temp= 0x100;
	uint32_t volatile delay=500;
 
    /* Enable USB Clock */
	CLK->APBCLK |= CLK_APBCLK_USBD_EN;

    /* Reset USB */
	GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_USBD;
	while(delay--);
	GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_USBD;

	USBD->CTRL = (USB_CTRL_WAKEUP_EN|USB_CTRL_DPPU_EN|USB_CTRL_DRVSE0|USB_CTRL_PWRDB|USB_CTRL_PHY_EN|USB_CTRL_USB_EN);
	USBD->PDMA |= USB_PDMA_BYTEM_BYTE;
	USB_ENABLE_USB();
	USB_ENABLE_SE0();	
    while(temp--);   
	USB_DISABLE_SE0();
}


/**   
  * @brief  USBD DeInit, Disable clock.
  * @param  None.
  * @retval None.
  */
void USBD_DeInit(void)
{
    /* Disable USB */
	USB_DISABLE_USB();
	
	/* Disable the USB clock */
	CLK->APBCLK &= ~CLK_APBCLK_USBD_EN;
}


/**   
  * @brief  USB_Open, Configure Endpoint Pipe.
  * @param  sParam: USB information structure.
  * @retval None.
  */
void USB_Open(STR_USBD_T *sParam)
{
	uint32_t volatile i, regAddr, regVal;
	
	gCtrlMaxPacketSize = 8;

	memset((void *)&gUsbCtrl, 0, sizeof(STR_USBD_CTRL_T));
	//memset((void *)&gUsbInfo, 0, sizeof(STR_USBD_T));
	//memcpy(&gUsbInfo, sParam, sizeof(STR_USBD_T));
	memmove(&gUsbInfo, sParam, sizeof(STR_USBD_T));

	if (sParam->UsbDeviceClass & USB_CLASS_AUDIO)
		memset((void *)&gUacInfo, 0, sizeof(STR_USBD_UAC_T));
	if (sParam->UsbDeviceClass & USB_CLASS_MASS_STORAGE)
		memset((void *)&gUdcInfo, 0, sizeof(STR_USBD_UDC_T));
	if (sParam->UsbDeviceClass & USB_CLASS_CDC)
	{
		memset((void *)&gCdcInfo, 0, sizeof(STR_USBD_CDC_T));
		gCdcInfo.u32DTERate = 115200;	/* Baud rate : 115200 */
		gCdcInfo.u8CharFormat = 0;		/* stop bit */
		gCdcInfo.u8ParityType = 0;		/* parity */
		gCdcInfo.u8DataBits = 8;		/* data bits */
	}
    /***************************************************************/
	/* Init control end point */
	USBD->BUFSEG = 0;					/* Buffer for setup packet */

	/* EP0 ==> control IN end point, address 0 */
    USBD->CFG0 = (USB_CFG_CSTALL | USB_CFG_EPMODE_IN  | 0 | 0x8000);	/* Clear EP0 CTRL IN Ready Flag */
    USBD->BUFSEG0 = 0x8;				/* Buffer for EP0 */
	gUsbCtrl.EPInfo[0] = 0;

	/* EP1 ==> control OUT end point, address 0 */
    USBD->CFG1 = (USB_CFG_CSTALL | USB_CFG_EPMODE_OUT | 0 | 0x8000); 	/* Clear EP1 CTRL OUT Ready Flag */
    USBD->BUFSEG1 = 0x8;  				/* Buffer for EP1 */
	gUsbCtrl.EPInfo[1] = 0;

	/* save Ctrl Max. Packet Size and current SRAM buffer */
	gCtrlMaxPacketSize = sParam->u32CtrlMaxPacketSize;
	gCurrentSramBuffer = 0x8 + sParam->u32CtrlMaxPacketSize;
	gTotalEnpointCount = sParam->u32EndpointCount;

    /***************************************************************/
	/* Init other endpoint */
	for (i=0; i<sParam->u32EndpointCount; i++)
	{
		gUsbCtrl.EPInfo[i+2] = sParam->EP[i].u32EpNum;
		regAddr = (uint32_t)(0x40060048 + 0x10 * i);	/* USBD CFG2 */
		regVal = ((sParam->EP[i].IsDirIN? USB_CFG_EPMODE_IN: USB_CFG_EPMODE_OUT) | sParam->EP[i].u32EpNum | 0x8000);
		if (sParam->EP[i].IsIsoTransfer)
			regVal |= USB_CFG_ISOCH;

		*((__IO uint32_t *) (regAddr)) = regVal;
		regAddr = (uint32_t)(0x40060040 + 0x10 * i);	/* USBD BUFSEG2 */
		*((__IO uint32_t *) (regAddr)) = gCurrentSramBuffer;
		gCurrentSramBuffer += sParam->EP[i].u32MaxPacketSize;
	}

	gUsbCtrl.CurrentState = USB_STATE_DETACHED;
	USB_FloatDetect();

	/* Enable Interrupt Handler */
	USBD->INTEN = (USB_INTEN_BUSEVT | USB_INTEN_USBEVT | USB_INTEN_FLDET | USB_INTEN_WAKEUP);
	//NVIC_SetPriority (USBD_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ(USBD_IRQn);
}


/***************************************************************/
/* USB Device Interrupt Handler Sub-Routine                    */
/***************************************************************/
/**   
  * @brief  USB_FloatDetect, USB Floating Detect.
  * @param  None.
  * @retval None.
  */
void USB_FloatDetect(void)
{
	uint32_t u32FLDET = (USBD->BUSSTS & USB_BUSSTS_FLDET_IN)? TRUE : FALSE;

	USBD->INTSTS = USB_INTSTS_FLD_STS;
	
	if (u32FLDET)
	{
	    /* USB Plug In */
		gUsbCtrl.CurrentState = USB_STATE_ATTACHED;
	    /* Enable USB and enable PHY */
        USB_ENABLE_USB();
	}
	else
	{
	    /* USB Un-plug */
		gUsbCtrl.CurrentState = USB_STATE_DETACHED;
	    /* Disable USB, Disable remote weak up, and disable PHY */
        USB_DISABLE_USB();
	}
}


/**   
  * @brief  USB_BusEvent, USB bus event: Reset, Suspend, and Resume.
  * @param  None.
  * @retval None.
  */
void USB_BusEvent(void)
{
	uint8_t u8BUSSTS = USBD->BUSSTS;
	
    /* Clear event flag */
	USBD->INTSTS = USB_INTSTS_BUS_STS;
	
	/* Just return if USB detached. */
	if (gUsbCtrl.CurrentState == USB_STATE_DETACHED)
		return;

    /* Process USB bus command if USB is attached. */
	if (u8BUSSTS & USB_BUSSTS_USBRST)
	{	
	    /* Enable USB and enable PHY */
        USB_ENABLE_USB();		

		USBD->DADDR = 0;
		//memset((void *)&gUsbCtrl, 0, sizeof(STR_USBD_CTRL_T));
		gUsbCtrl.DeviceAddress = 0;
		gUsbCtrl.ConfigState = 0;
		gUsbCtrl.EP0_Toggle = 0;
		gUsbCtrl.StateFlag = 0;

		/* Enter default state */
		gUsbCtrl.CurrentState = USB_STATE_DEFAULT;
	}
	else if (u8BUSSTS & USB_BUSSTS_SUSPEND)
	{	
	    /* Enable USB but disable PHY */
        USB_DISABLE_PHY();
		
		/* Update the USB state */
		gUsbCtrl.CurrentState |= USB_STATE_FLAG_SUSPENDED;
	}
	else if (u8BUSSTS & USB_BUSSTS_RESUME)
	{
	    /* Enable USB and enable PHY */
		USB_ENABLE_USB();
		
		/* Clear the suspend state */
		gUsbCtrl.CurrentState &= ~USB_STATE_FLAG_SUSPENDED;
	}
}


/**   
  * @brief  USB_UsbEvent, USB Protocol/Clase event, endpoint status.
  * @param  u32INTSTS Interrupt status need to be processed.
  * @retval None.
  */
void USB_UsbEvent(uint32_t u32INTSTS)
{
	/* Just return if USB detached. */
	if (gUsbCtrl.CurrentState == USB_STATE_DETACHED)
		return;

	if (u32INTSTS & USB_INTSTS_SETUP)
	{	
	    /* Clear setup event */
		USBD->INTSTS = USB_INTSTS_SETUP;
		USB_ProcessSetup();
	}
	else if(u32INTSTS & USB_INTSTS_EPEVT0)
	{	
	    /* Ctrl In */
		USBD->INTSTS = USB_INTSTS_EPEVT0;
		USB_CtrlInAck();
	}
	else if(u32INTSTS & USB_INTSTS_EPEVT1)
	{	
	    /* Ctrl Out */
		USBD->INTSTS = USB_INTSTS_EPEVT1;
		USB_CtrlOutAck();
	}
	else if(u32INTSTS & USB_INTSTS_EPEVT2)
	{	
		USBD->INTSTS = USB_INTSTS_EPEVT2;
		if (gUsbInfo.UsbDeviceClass & USB_CLASS_MASS_STORAGE)
			gUdcInfo.OutToggle = USBD->EPSTS;
		USB_EpAck0();
	}
	else if(u32INTSTS & USB_INTSTS_EPEVT3)
	{	
		USBD->INTSTS = USB_INTSTS_EPEVT3;
		if (gUsbInfo.UsbDeviceClass & USB_CLASS_MASS_STORAGE)
		{
			if (gUdcInfo.OutToggle == USBD->EPSTS & 0x00F00000)
				USBD->MXPLD3 = gUsbInfo.EP[1].u32MaxPacketSize;
			else
			{
				if (USB_EpAck1())
					gUdcInfo.OutToggle = USBD->EPSTS & 0x00F00000;
				else
				{
				    /* stall */
					gUdcInfo.CBWInvalid = TRUE;
				    USBD->CFG2 |= (USB_CFG_SSTALL | 0x8000);
				    USBD->CFG3 |= (USB_CFG_SSTALL | 0x8000);
				}
			}
		}
		else
			USB_EpAck1();
	}
	else if(u32INTSTS & USB_INTSTS_EPEVT4)
	{
		USBD->INTSTS = USB_INTSTS_EPEVT4;
		USB_EpAck2();
	}	
	else if(u32INTSTS & USB_INTSTS_EPEVT5)
	{
		USBD->INTSTS = USB_INTSTS_EPEVT5;
		USB_EpAck3();
	}	
	else if(u32INTSTS & USB_INTSTS_EPEVT6)
	{
		USBD->INTSTS = USB_INTSTS_EPEVT6;
		USB_EpAck4();
	}	
	else if(u32INTSTS & USB_INTSTS_EPEVT7)
	{
		USBD->INTSTS = USB_INTSTS_EPEVT7;
		USB_EpAck5();
	}	
	else
	{	/* Unknow event */
		USBD->INTSTS = USB_INTSTS_USB_STS;
	}
}

/***************************************************************/

/**   
  * @brief  USB_StandardRequest, Process USB Standard Request.
  * @param  None.
  * @retval 1: Standard Request.
  *         0: has some error.
  */
uint32_t USB_StandardRequest(void)
{
	uint8_t *ptr;

	/* check if after DEFAULT state (RESET) */
	if ((gUsbCtrl.CurrentState & USB_STATE_FLAG_DEFAULT) == 0x00)
		return FALSE;

	/* get control pipe SRAM buffer */
	ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0);

	/* parse the request in setup packet */	
    switch (gUsbCtrl.UsbSetupBuf[1])
	{
		case GET_DESCRIPTOR:
		{
			return USB_ProcessDescriptor(ptr, gCtrlMaxPacketSize);
		}
		case SET_ADDRESS:
		{
			/* Accept set address command only before USB configured */
			if((gUsbCtrl.CurrentState & USB_STATE_FLAG_CONFIGURED) == 0)
			{
				/* Get the device address */
				gUsbCtrl.DeviceAddress = gUsbCtrl.UsbSetupBuf[2];
				USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
				USBD->MXPLD0 = 0;

				gUsbCtrl.StateFlag = FLAG_SET_ADDRESS;
				return TRUE;
			}
			return FALSE;
		}
		case SET_CONFIGURATION:
		{
			gUsbCtrl.ConfigState = gUsbCtrl.UsbSetupBuf[2];
            
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
			gUsbCtrl.StateFlag = FLAG_SET_CONFIG;
			USB_DeviceStart();
			return TRUE;
		}
		case CLEAR_FEATURE:
		case SET_FEATURE:
		{
			if (gUsbInfo.UsbDeviceClass & USB_CLASS_MASS_STORAGE)
			{
				if (gUdcInfo.CBWInvalid)
				{
					USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
					USBD->MXPLD0 = 0;
					return TRUE;
				}
			}

			if((gUsbCtrl.UsbSetupBuf[0] == 2) && (gUsbCtrl.UsbSetupBuf[2] == FEATURE_ENDPOINT_HALT))
            {
				uint32_t u32CfgAddr;
				uint32_t u32Cfg;
				uint32_t i;

				u32Cfg = gUsbCtrl.UsbSetupBuf[4] & 0xF;
				for (i = 0; i < gTotalEnpointCount+2; i++)
				{
					if (gUsbCtrl.EPInfo[i] == u32Cfg)
					{
						u32Cfg = i;
						break;
					}
				}
				u32CfgAddr = (uint32_t)(u32Cfg << 4) + (uint32_t)0x40060028; /* USBD_CFG0 */
				u32Cfg = *((__IO uint32_t *) (u32CfgAddr));

				if(gUsbCtrl.UsbSetupBuf[1] == CLEAR_FEATURE)
					u32Cfg &= ~USB_CFG_SSTALL;
				else
					u32Cfg |= USB_CFG_SSTALL;
				*((__IO uint32_t *) (u32CfgAddr)) = u32Cfg;
			}
			else
				return FALSE;
				
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
			return TRUE;
		}
		case GET_CONFIGURATION:
		{
			*ptr = gUsbCtrl.ConfigState;
            
			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 1;
			gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			return TRUE;
		}
		case GET_STATUS:
		{
            uint8_t u8Data;

            u8Data = gUsbCtrl.UsbSetupBuf[0];
			// Device
			if (u8Data == 0x80)
			{
            	if (USB_ConfigDescriptor[7] & 0x40)
					ptr[0] = 1;	// Self-Powered
				else
					ptr[0] = 0;	// bus-Powered
			}
			// Interface
			else if (u8Data == 0x81)
            	ptr[0] = 0;
			
			// Endpoint
			else if (u8Data == 0x82)
            {
				uint32_t u32CfgAddr;
				uint32_t u32Cfg;
				uint32_t i;

				u32Cfg = gUsbCtrl.UsbSetupBuf[4] & 0xF;
				for (i = 0; i < gTotalEnpointCount+2; i++)
				{
					if (gUsbCtrl.EPInfo[i] == u32Cfg)
					{
						u32Cfg = i;
						break;
					}
				}
				u32CfgAddr = (uint32_t)(u32Cfg << 4) + (uint32_t)0x40060028; /* USBD_CFG0 */
				ptr[0] = *((__IO uint32_t *) (u32CfgAddr)) & USB_CFG_SSTALL? 1: 0;
			}
			else
				return FALSE;
   			
            ptr[1] = 0;

			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 2;
			gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			return TRUE;
		}
		case GET_INTERFACE:
		{
			if ((gUsbInfo.UsbDeviceClass & USB_CLASS_AUDIO) || (gUsbInfo.UsbDeviceClass & USB_CLASS_VIDEO))
				ptr[0] = gUacInfo.AlternativeSetting;
			else
				ptr[0] = 0;

			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 1;
			gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			return TRUE;
		}
		case SET_INTERFACE:
		{
			gUsbCtrl.StateFlag = FLAG_SET_INTERFACE;

			if (gUsbInfo.UsbDeviceClass & USB_CLASS_AUDIO)
			{
				gUacInfo.AlternativeSetting = gUsbCtrl.UsbSetupBuf[2];

				if (gUsbCtrl.UsbSetupBuf[4] == 1)
				{ /* Audio Iso IN interface */
					if (gUacInfo.AlternativeSetting == 1)
					{
						gUacInfo.AudioState = 1;	/* UAC_START_AUDIO_RECORD */
						USBD->CFG2 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD2 = 0;
						UAC_DeviceEnable(0);		/* UAC_MICROPHONE */
					}
					else if (gUacInfo.AlternativeSetting == 0)
					{
						UAC_DeviceDisable(0);		/* UAC_MICROPHONE */
						USBD->CFG2 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD2 = 0;
						gUacInfo.AudioState = 0;	/* UAC_STOP_AUDIO_RECORD */
					}
				}
				else if (gUsbCtrl.UsbSetupBuf[4] == 2)
				{ /* Audio Iso OUT interface */
					if (gUacInfo.AlternativeSetting == 1)
					{
						USBD->MXPLD3 = gUsbInfo.EP[gUsbCtrl.UsbSetupBuf[4]-1].u32MaxPacketSize;
						UAC_DeviceEnable(1);		/* UAC_SPEAKER */
					}
					else
						UAC_DeviceDisable(1);		/* UAC_SPEAKER */
				}
			}

			if (gUsbInfo.UsbDeviceClass & USB_CLASS_VIDEO)
			{
				gUacInfo.AlternativeSetting = gUsbCtrl.UsbSetupBuf[2];

				if (gUsbCtrl.UsbSetupBuf[4] == 1)
				{ /* Audio Iso IN interface */
					if (gUacInfo.AlternativeSetting == 1)
					{
						USBD->CFG2 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD2 = 0;
					}
					else if (gUacInfo.AlternativeSetting == 0)
					{
						USBD->CFG2 |= USB_CFG_DSQ_SYNC_DATA1;
						USBD->MXPLD2 = 0;
					}
				}
				else if (gUsbCtrl.UsbSetupBuf[4] == 2)
				{ /* Audio Iso OUT interface */
				}
			}

			USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
			USBD->MXPLD0 = 0;
			return TRUE;
		}
		default:
			return FALSE;
	}
}


/**   
  * @brief  USB_VendorRequest, Process USB Vendor Request.
  * @param  None.
  * @retval 1: Standard Request.
  *         0: has some error.
  */
uint32_t USB_VendorRequest(void)
{
	return FALSE;
}


/**   
  * @brief  USB_ProcessSetup, Process Setup Packet.
  * @param  None.
  * @retval None.
  */
void USB_ProcessSetup(void)
{
	uint32_t i;

    /* Clear the data IN/OUT ready flag of control end-points */
    USBD->CFG0 |= 0x8000;
    USBD->CFG1 |= 0x8000;
	
	/* Copy Setup Packet */
	for (i = 0; i < 8; i++)
		gUsbCtrl.UsbSetupBuf[i] = *(uint8_t *)((uint32_t)USBD_SRAM_BASE + i);

	/* USB device request in setup packet: offset 0, D[6..5]: 0=Stardard, 1=Class, 2=Vendor, 3=Reserved */
	switch (gUsbCtrl.UsbSetupBuf[0] & 0x60)
	{
		case REQ_STANDARD:
			USB_StandardRequest();
			break;

		case REQ_CLASS:
			USB_ClassRequest();
			break;

		case REQ_VENDOR:
			USB_VendorRequest();
			break;

		default:
		{
		    /* Setup error, stall the device */
			USBD->CFG0 |= (USB_CFG_SSTALL | 0x8000);
			USBD->CFG1 |= (USB_CFG_SSTALL | 0x8000);
		}
	}
}


/**   
  * @brief  USB_CtrlInAck, Ack Control Pipe IN.
  * @param  None.
  * @retval None.
  */
void USB_CtrlInAck(void)
{
	uint8_t *ptr;

	/* get control pipe SRAM buffer */
	ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0);

	switch (gUsbCtrl.StateFlag)
	{
		/* Out ACK */
		case FLAG_OUT_ACK:
			gUsbCtrl.pDesc = 0;
			gUsbCtrl.ReqLen = 0;
			USBD->MXPLD1 = 0;
			break;
		/* Set Address */
		case FLAG_SET_ADDRESS:
		{
			if (gUsbCtrl.DeviceAddress == 0)
				gUsbCtrl.CurrentState = USB_STATE_DEFAULT;
			else
				gUsbCtrl.CurrentState = USB_STATE_ADDRESS;

			USBD->DADDR = gUsbCtrl.DeviceAddress;
			break;
		}
		/* Set Interface */
		case FLAG_SET_INTERFACE:
		/* Set Configuration */
		case FLAG_SET_CONFIG:
		{
			if (gUsbCtrl.ConfigState == 0)
				gUsbCtrl.CurrentState = USB_STATE_ADDRESS;
			else
				gUsbCtrl.CurrentState = USB_STATE_CONFIGURED;
			break;
		}
		/* EP0 Data In */
		case FLAG_EP0_DATA_IN:
			if (gUsbCtrl.ReqLen >= gCtrlMaxPacketSize)
			{
				my_memcpy(ptr, (void *)gUsbCtrl.pDesc, gCtrlMaxPacketSize);
				gUsbCtrl.pDesc = gUsbCtrl.pDesc + gCtrlMaxPacketSize;
				if (gUsbCtrl.EP0_Toggle)
				{
					gUsbCtrl.EP0_Toggle = 0;
					USBD->CFG0 &= ~USB_CFG_DSQ_SYNC_DATA1;
				}
				else
				{
					gUsbCtrl.EP0_Toggle = 1;
					USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
				}
				USBD->MXPLD0 = gCtrlMaxPacketSize;
				gUsbCtrl.ReqLen -= gCtrlMaxPacketSize;
			}
			else
			{
				my_memcpy(ptr, (void *)gUsbCtrl.pDesc, gUsbCtrl.ReqLen);
				USBD->MXPLD0 = gUsbCtrl.ReqLen;
				if (gUsbCtrl.ReqLen == 0)
					USBD->MXPLD1 = 0;
				else
					gUsbCtrl.StateFlag = FLAG_OUT_ACK;
			}
			return;
		default:
			return;
	}
	gUsbCtrl.StateFlag = 0;
}


/**   
  * @brief  USB_CtrlOutAck, Ack Control Pipe IN.
  * @param  None.
  * @retval None.
  */
void USB_CtrlOutAck(void)
{
	switch (gUsbCtrl.StateFlag)
	{
		case FLAG_UAC_SET_CUR:
		{
			if (gUsbInfo.UsbDeviceClass & USB_CLASS_AUDIO)
			{
				uint8_t *ptr;
				/* get control pipe SRAM buffer */
				ptr = (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0);

				if (gUsbCtrl.UsbSetupBuf[5] == 0x05)	/* REC_FEATURE_UNITID */
				{
					if (gUsbCtrl.UsbSetupBuf[3] == 0x01)	/* MUTE_CONTROL */
						gUacInfo.RecMute = *ptr;
					else if (gUsbCtrl.UsbSetupBuf[3] == 0x02)	/* VOLUME_CONTROL */
						gUacInfo.RecVolume = (int16_t) (*ptr | (*(ptr+1) << 8));
				}
				else if (gUsbCtrl.UsbSetupBuf[5] == 0x06)	/* PLAY_FEATURE_UNITID */
				{
					if (gUsbCtrl.UsbSetupBuf[3] == 0x01)	/* MUTE_CONTROL */
						gUacInfo.PlayMute = *ptr;
					else if (gUsbCtrl.UsbSetupBuf[3] == 0x02)	/* VOLUME_CONTROL */
					{
						if (gUsbCtrl.UsbSetupBuf[2] == 1)
							gUacInfo.PlayVolumeL = (int16_t) (*ptr | (*(ptr+1) << 8));
						else
							gUacInfo.PlayVolumeR = (int16_t) (*ptr | (*(ptr+1) << 8));
					}
				}
				USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
				USBD->MXPLD0 = 0;
			}
			break;
		}

		case FLAG_SET_LINE_CODE:
		{
			if (gUsbInfo.UsbDeviceClass & USB_CLASS_CDC)
			{
				my_memcpy((void *)&gCdcInfo, (uint8_t *)((uint32_t)USBD_SRAM_BASE + (uint32_t)USBD->BUFSEG0), USBD->MXPLD1);
				USBD->CFG0 |= USB_CFG_DSQ_SYNC_DATA1;
				USBD->MXPLD0 = 0;
				break;
			}
		}
		default:
			;
	}
	gUsbCtrl.StateFlag = 0;
}


/*@}*/ /* end of group NANO1xx_USB_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_USB_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

