/******************************************************************************
 * @file     nano1xx_usb.h
 * @brief    NANO1xx USB driver header file
 * @version  1.0.2
 * @date     02, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_USB_H
#define __NANO1XX_USB_H

#include "nano1xx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_USB_Driver USB Driver
  @{
*/

/** @addtogroup NANO1xx_USB_EXPORTED_CONSTANTS USB Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define Maximum(a,b)		(a)>(b) ? (a) : (b)
#define Minimum(a,b)		(a)<(b) ? (a) : (b)

/*---------------------------------------------------------------------------------------------------------*/
/* USB Device Class                                                           		  					   */
/*---------------------------------------------------------------------------------------------------------*/
/*!<USB Device Class Definition */
#define	USB_CLASS_NONE					0x00000000
#define	USB_CLASS_HID					0x00000001
#define	USB_CLASS_CDC					0x00000002
#define	USB_CLASS_AUDIO					0x00000004
#define	USB_CLASS_VIDEO					0x00000008
#define	USB_CLASS_MASS_STORAGE			0x00000010
#define	USB_CLASS_CCID					0x00000020

/*---------------------------------------------------------------------------------------------------------*/
/* USB Request                                                                    						   */
/*---------------------------------------------------------------------------------------------------------*/
/*!<USB Request Type */
#define	REQ_STANDARD		0x00
#define	REQ_CLASS			0x20
#define	REQ_VENDOR			0x40

/*!<USB Standard Request */
#define	GET_STATUS			0x00
#define	CLEAR_FEATURE		0x01
#define	SET_FEATURE			0x03
#define	SET_ADDRESS			0x05
#define	GET_DESCRIPTOR		0x06
#define	SET_DESCRIPTOR		0x07
#define	GET_CONFIGURATION	0x08
#define	SET_CONFIGURATION	0x09
#define	GET_INTERFACE		0x0A
#define	SET_INTERFACE		0x0B
#define	SYNC_FRAME			0x0C


/*---------------------------------------------------------------------------------------------------------*/
/* USB Descriptor Type                                                                 					   */
/*---------------------------------------------------------------------------------------------------------*/
/*!<USB Descriptor Type */
#define	DESC_DEVICE			0x01
#define	DESC_CONFIG			0x02
#define	DESC_STRING			0x03
#define	DESC_INTERFACE		0x04
#define	DESC_ENDPOINT		0x05
#define	DESC_QUALIFIER		0x06
#define	DESC_OTHERSPEED		0x07

/*!<USB HID Descriptor Type */
#define DESC_HID 			0x21
#define DESC_HID_RPT 		0x22

/*---------------------------------------------------------------------------------------------------------*/
/* USB Descriptor Length                                                              					   */
/*---------------------------------------------------------------------------------------------------------*/
/*!<USB Descriptor Length */
#define	LEN_DEVICE			18
#define	LEN_CONFIG			9
#define	LEN_INTERFACE		9
#define	LEN_ENDPOINT		7

/*!<USB HID Descriptor Length */
#define LEN_HID				0x09

/*!<USB CCID Descriptor Length */
#define LEN_CCID			0x36

/*!<USB Endpoint Type */
#define	EP_ISO				0x01
#define	EP_BULK				0x02
#define	EP_INT				0x03

#define EP_INPUT			0x80
#define EP_OUTPUT			0x00

/*---------------------------------------------------------------------------------------------------------*/
/* USB Feature Selector                                                             					   */
/*---------------------------------------------------------------------------------------------------------*/
#define	FEATURE_DEVICE_REMOTE_WAKEUP	0x01
#define	FEATURE_ENDPOINT_HALT			0x00

/*---------------------------------------------------------------------------------------------------------*/
/* USB Device state                                                           		  					   */
/*---------------------------------------------------------------------------------------------------------*/
/*!<USB Device State Definition */
#define	USB_STATE_DETACHED			0x00
#define	USB_STATE_ATTACHED			0x01
#define	USB_STATE_DEFAULT			0x11
#define	USB_STATE_ADDRESS			0x31
#define	USB_STATE_CONFIGURED		0x71

#define	USB_STATE_FLAG_DEFAULT		0x10
#define	USB_STATE_FLAG_ADDRESS		0x20
#define	USB_STATE_FLAG_CONFIGURED	0x40
#define	USB_STATE_FLAG_SUSPENDED	0x80

/*!<USB Device State Flag Definition */
#define	FLAG_OUT_ACK				0x0001
#define	FLAG_SET_ADDRESS			0x0002
#define	FLAG_SET_CONFIG				0x0004
#define	FLAG_SET_INTERFACE			0x0008
#define	FLAG_EP0_DATA_OUT			0x0010
#define	FLAG_EP0_DATA_IN			0x0020
#define	FLAG_EP0_OUT_END			0x0040

/* Audio Class */
#define FLAG_UAC_SET_CUR			0x0100
/* CDC Class */
#define	FLAG_SET_LINE_CODE			0x0200



/******************************************************************************/
/*                USB Specific Macros                                         */
/******************************************************************************/

/*!<Enable USB , PHY, and Pull-Up Resistor */
#define USB_ENABLE_USB()	((uint32_t)(USBD->CTRL |= 0xF))

/*!<Disable USB */
#define USB_DISABLE_USB()	((uint32_t)(USBD->CTRL &= ~USB_CTRL_USB_EN))

/*!<Disable PHY */
#define USB_DISABLE_PHY()	((uint32_t)(USBD->CTRL &= ~USB_CTRL_PHY_EN))

/*!<Enable SE0, Force USB PHY Transceiver to Drive SE0 */
#define USB_ENABLE_SE0()	((uint32_t)(USBD->CTRL |= USB_CTRL_DRVSE0))

/*!<Disable SE0 */
#define USB_DISABLE_SE0()	((uint32_t)(USBD->CTRL &= ~USB_CTRL_DRVSE0))


/*@}*/ /* end of group NANO1xx_USB_EXPORTED_CONSTANTS */


/******************************************************************************/
/*                USBD Specific Structure                                     */
/******************************************************************************/
/** @addtogroup NANO1xx_USB_EXPORTED_STRUCTS USB Exported Structs
  @{
*/

/*!<USB Init Endpoint Structure */
typedef struct DRVUSBD_EP_STRUCT	/* USBD endpoint structure */
{
	uint32_t u32EpNum;				/* Endpoint Number */
	uint32_t u32MaxPacketSize;		/* Max Packet Size */
	uint32_t IsDirIN;				/* IN endpoint */
	uint32_t IsIsoTransfer;			/* ISO Pipe */

}STR_USBD_EP_T;

/*!<USB Structure */
typedef struct DRVUSBD_STRUCT		/* USBD endpoint structure */
{
	uint32_t UsbDeviceClass;		/* Select USB Device Class */
	uint32_t u32CtrlMaxPacketSize;	/* Assign Control pipe maximum packet size */
	uint32_t u32EndpointCount;		/* Total endpoint count */
	STR_USBD_EP_T EP[6];			/* Endpoint Information */

}STR_USBD_T;

/*!<USB Flow Control Structure */
typedef struct DRVUSBD_CTRL_STRUCT	/* USBD Flow Control structure */
{
	uint8_t CurrentState;			/* USB Current State */
	uint8_t DeviceAddress;			/* USB Device Address */
	uint8_t ConfigState;			/* USB Configure State */
	uint8_t EP0_Toggle;				/* Record Toggle bit state */
	uint32_t StateFlag;				/* USB State Flag */

	uint8_t UsbSetupBuf[8];			/* Setup Packet */
	uint8_t EPInfo[8];				/* EP address */

	uint8_t *pDesc;					/* remain descriptor pointer */
	uint32_t ReqLen;				/* request length */

}STR_USBD_CTRL_T;

/*!<USB Audio Class Information Structure */
typedef struct DRVUSBD_UAC_STRUCT	/* USBD Flow Control structure */
{
	uint8_t AudioState;
	uint8_t AlternativeSetting;

	uint8_t RecMute;
	uint8_t PlayMute;

	int16_t RecVolume;
	int16_t RecMaxVolume;
	int16_t RecMinVolume;
	int16_t RecResVolume;

	int16_t PlayVolumeL;
	int16_t PlayVolumeR;
	int16_t PlayMaxVolume;
	int16_t PlayMinVolume;
	int16_t PlayResVolume;

}STR_USBD_UAC_T;


/*!<USB Mass Storage Class Information Structure */
typedef struct DRVUSBD_UDC_STRUCT
{
	uint8_t CBWInvalid;
	uint32_t OutToggle;		/* for Error Detection */

}STR_USBD_UDC_T;


/*!<USB CDC Class Information Structure */
typedef struct DRVUSBD_CDC_STRUCT
{
	uint32_t  u32DTERate;     /* Baud rate    */
	uint8_t   u8CharFormat;   /* stop bit     */
	uint8_t   u8ParityType;   /* parity       */
	uint8_t   u8DataBits;     /* data bits    */

}STR_USBD_CDC_T;

/*@}*/ /* end of group NANO1xx_USB_EXPORTED_STRUCTS */



/******************************************************************************/
/*                USBD Specific inline                                        */
/******************************************************************************/
/** @addtogroup NANO1xx_USB_EXPORTED_FUNCTIONS USB Exported Functions
  @{
*/


/**   
  * @brief  USB_GET_EPSTS, USB Get endpoint status.
  * @param  None.
  * @retval USB endpoint status.
  */
static __INLINE uint32_t USB_GET_EPSTS()
{
	return (USBD->EPSTS);
}

/**   
  * @brief  USB_GET_EPSTS2, USB Get endpoint status.
  * @param  None.
  * @retval USB endpoint status.
  */
static __INLINE uint32_t USB_GET_EPSTS2()
{
	return (USBD->EPSTS2);
}

/*@}*/ /* end of group NANO1xx_USB_EXPORTED_FUNCTIONS */


/** @addtogroup NANO1xx_USB_EXPORTED_VARIABLES USB Exported Variables
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Extern Global Variables                                                           		  			   */
/*---------------------------------------------------------------------------------------------------------*/
extern STR_USBD_CTRL_T __IO gUsbCtrl;
extern STR_USBD_T gUsbInfo;
extern STR_USBD_UAC_T gUacInfo;
extern STR_USBD_UDC_T gUdcInfo;
extern STR_USBD_CDC_T gCdcInfo;

extern const uint8_t USB_ConfigDescriptor[];

/*@}*/ /* end of group NANO1xx_USB_EXPORTED_VARIABLES */

/** @addtogroup NANO1xx_USB_EXPORTED_FUNCTIONS USB Exported Functions
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* APIs declare                                                           		  						   */
/*---------------------------------------------------------------------------------------------------------*/
void my_memcpy(void *dest, void *src, int32_t size);
void USBD_Init(void);
void USBD_DeInit(void);
void USB_Open(STR_USBD_T *sParam);
void USB_FloatDetect(void);
void USB_BusEvent(void);
void USB_UsbEvent(uint32_t u32INTSTS);
uint32_t USB_StandardRequest(void);
uint32_t USB_VendorRequest(void);
void USB_ProcessSetup(void);
void USB_CtrlInAck(void);
void USB_CtrlOutAck(void);

uint32_t USB_ProcessDescriptor(uint8_t *pCtrl, uint32_t CtrlMaxPacketSize);
uint32_t USB_ClassRequest(void);
uint32_t USB_EpAck0(void);
uint32_t USB_EpAck1(void);
uint32_t USB_EpAck2(void);
uint32_t USB_EpAck3(void);
uint32_t USB_EpAck4(void);
uint32_t USB_EpAck5(void);
void USB_DeviceStart(void);

/* UAC Function API */
#ifdef __GNUC__
extern void UAC_DeviceEnable(uint8_t u8Object) __attribute__ ((weak));	/* GNU mode */
extern void UAC_DeviceDisable(uint8_t u8Object) __attribute__ ((weak));
#else
__weak void UAC_DeviceEnable(uint8_t u8Object);		/* non-GNU */
__weak void UAC_DeviceDisable(uint8_t u8Object);
#endif

/*@}*/ /* end of group NANO1xx_USB_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_USB_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

	
#ifdef __cplusplus
}
#endif

#endif 

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


