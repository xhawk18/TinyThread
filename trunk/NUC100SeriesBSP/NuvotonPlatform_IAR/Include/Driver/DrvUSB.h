/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef _DRVUSB_H
#define _DRVUSB_H

#include "NUC1xx.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define DRVUSB_MAJOR_NUM    1
#define DRVUSB_MINOR_NUM    2
#define DRVUSB_BUILD_NUM    2

/* Define module version number */
#define DRVUSB_VERSION_NUM    _SYSINFRA_VERSION(DRVUSB_MAJOR_NUM, DRVUSB_MINOR_NUM, DRVUSB_BUILD_NUM)


//E_DRVUSB_NULL_POINTER             Callback function is NULL.
//E_DRVUSB_INVALID_EP_NUM           EndPoint number error.
//E_DRVUSB_SIZE_TOO_LONG            Data size is too long.
//E_DRVUSB_VALUE_INVALID            Input argument is invalid.  

#define E_DRVUSB_NULL_POINTER       _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUSB, 1)
#define E_DRVUSB_INVALID_EP_NUM     _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUSB, 2)
#define E_DRVUSB_SIZE_TOO_LONG      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUSB, 3)
#define E_DRVUSB_VALUE_INVALID      _SYSINFRA_ERRCODE(TRUE, MODULE_ID_DRVUSB, 4)


/* Define USB Register Address */
#define USBD_INTEN          (USBD_BASE + 0x00)
#define USBD_INTSTS         (USBD_BASE + 0x04)
#define USBD_FADDR          (USBD_BASE + 0x08)
#define USBD_EPSTS          (USBD_BASE + 0x0C)
#define USBD_ATTR           (USBD_BASE + 0x10)
#define USBD_FLDET          (USBD_BASE + 0x14)
#define USBD_BUFSEG         (USBD_BASE + 0x18)
#define USBD_BUFSEG0        (USBD_BASE + 0x20)
#define USBD_MXPLD0         (USBD_BASE + 0x24)
#define USBD_CFG0           (USBD_BASE + 0x28)
#define USBD_CFGP0          (USBD_BASE + 0x2C)
#define USBD_BUFSEG1        (USBD_BASE + 0x30)
#define USBD_MXPLD1         (USBD_BASE + 0x34)
#define USBD_CFG1           (USBD_BASE + 0x38)
#define USBD_CFGP1          (USBD_BASE + 0x3C)
#define USBD_BUFSEG2        (USBD_BASE + 0x40)
#define USBD_MXPLD2         (USBD_BASE + 0x44)
#define USBD_CFG2           (USBD_BASE + 0x48)
#define USBD_CFGP2          (USBD_BASE + 0x4C)
#define USBD_BUFSEG3        (USBD_BASE + 0x50)
#define USBD_MXPLD3         (USBD_BASE + 0x54)
#define USBD_CFG3           (USBD_BASE + 0x58)
#define USBD_CFGP3          (USBD_BASE + 0x5C)
#define USBD_BUFSEG4        (USBD_BASE + 0x60)
#define USBD_MXPLD4         (USBD_BASE + 0x64)
#define USBD_CFG4           (USBD_BASE + 0x68)
#define USBD_CFGP4          (USBD_BASE + 0x6C)
#define USBD_BUFSEG5        (USBD_BASE + 0x70)
#define USBD_MXPLD5         (USBD_BASE + 0x74)
#define USBD_CFG5           (USBD_BASE + 0x78)
#define USBD_CFGP5          (USBD_BASE + 0x7C)
#define USBD_DRVSE0         (USBD_BASE + 0x90)
#define USB_SRAM_BASE       (USBD_BASE + 0x100)

#define INTEN_INNAK         0x00008000
#define INTEN_WAKEUPEN      0x00000100
#define INTEN_WAKEUP        0x00000008
#define INTEN_FLDET         0x00000004
#define INTEN_USB           0x00000002
#define INTEN_BUS           0x00000001

#define INTSTS_SETUP        0x80000000
#define INTSTS_EPTF5        0x00200000
#define INTSTS_EPTF4        0x00100000
#define INTSTS_EPTF3        0x00080000
#define INTSTS_EPTF2        0x00040000
#define INTSTS_EPTF1        0x00020000
#define INTSTS_EPTF0        0x00010000
#define INTSTS_WAKEUP       0x00000008
#define INTSTS_FLDET        0x00000004
#define INTSTS_USB          0x00000002
#define INTSTS_BUS          0x00000001

#define ATTR_BYTEM          0x00000400
#define ATTR_PWRDN          0x00000200
#define ATTR_DPPU_EN        0x00000100
#define ATTR_USB_EN         0x00000080
#define ATTR_RWAKEUP        0x00000020
#define ATTR_PHY_EN         0x00000010
#define ATTR_TIMEOUT        0x00000008
#define ATTR_RESUME         0x00000004
#define ATTR_SUSPEND        0x00000002
#define ATTR_USBRST         0x00000001

#define CFG_CSTALL          0x00000200
#define CFG_DSQ_SYNC        0x00000080
#define CFG_STATE           0x00000060
#define CFG_EPT_IN          0x00000040          /*  10: IN endpoint  */
#define CFG_EPT_OUT         0x00000020          /*  01: Out endpoint */
#define CFG_ISOCH           0x00000010
#define CFG_EPT             0x0000000F

#define CFGP_SSTALL         0x00000002
#define CFGP_CLRRDY         0x00000001

#define EPSTS_IN_ACK        0x00            /*  000: In ACK */
#define EPSTS_IN_NAK        0x10            /*  001: In NAK */
#define EPSTS_OUT0_ACK      0x20            /*  010: Out 0 ACK */
#define EPSTS_OUT1_ACK      0x60            /*  110: Out 1 ACK */
#define EPSTS_SETUP_ACK     0x30            /*  011: Setup ACK */
#define EPSTS_ISO           0x70            /*  111: Isochronous translation end */
#define EPSTS_STS           0x70            /*  A mask to get EP state */

#define _DRVUSB_DATA0(Data)           ((Data) & 0x37F)
#define _DRVUSB_DATA1(Data)           ((Data) | 0x80)
#define _DRVUSB_ENABLE_MISC_INT(u32Flags)   outp32(USBD_INTEN, (u32Flags) & (INTEN_WAKEUP + INTEN_FLDET + INTEN_USB + INTEN_BUS))

#define _DRVUSB_ENABLE_WAKEUP()       USBD->INTEN.WAKEUP_EN = 1
#define _DRVUSB_DISABLE_WAKEUP()      USBD->INTEN.WAKEUP_EN = 0
#define _DRVUSB_ENABLE_WAKEUP_INT()   USBD->INTEN.WAKEUP_IE = 1
#define _DRVUSB_DISABLE_WAKEUP_INT()  USBD->INTEN.WAKEUP_IE = 0
#define _DRVUSB_ENABLE_FLDET_INT()    USBD->INTEN.FLDET_IE = 1
#define _DRVUSB_DISABLE_FLDET_INT()   USBD->INTEN.FLDET_IE = 0
#define _DRVUSB_ENABLE_USB_INT()      USBD->INTEN.USB_IE = 1
#define _DRVUSB_DISABLE_USB_INT()     USBD->INTEN.USB_IE = 0
#define _DRVUSB_ENABLE_BUS_INT()      USBD->INTEN.BUS_IE = 1
#define _DRVUSB_DISABLE_BUS_INT()     USBD->INTEN.BUS_IE = 0

/* Respond EPx (x = 0 ~ 5) STALL packet */
#define _DRVUSB_CLEAR_EP_READY_AND_TRIG_STALL(u32EpId)      outp32(USBD_CFGP0+(0x10*(u32EpId)), (inp32(USBD_CFGP0+(0x10*(u32EpId))) | (CFGP_SSTALL | CFGP_CLRRDY)))
    
/* Clear EPx (x = 0 ~ 5) In/Out ready */
#define _DRVUSB_CLEAR_EP_READY(u32EpId)                     outp32(USBD_CFGP0+(0x10*(u32EpId)), (inp32(USBD_CFGP0+(0x10*(u32EpId))) | CFGP_CLRRDY))

/* Specify USB buffer for SETUP packet */
#define _DRVUSB_SET_SETUP_BUF(u32BufAddr)                   outp32(USBD_BUFSEG, (u32BufAddr) - USB_SRAM_BASE)

/* Specify USB buffer for EPx (x = 0 ~ 5) */
#define _DRVUSB_SET_EP_BUF(u32EpId, u32BufAddr)             outp32(USBD_BUFSEG0+(0x10 * (u32EpId)), (u32BufAddr) - USB_SRAM_BASE)
        
/* Trigger next transaction for EPx (x = 0 ~ 5) */
#define _DRVUSB_TRIG_EP(u32EpId, u32TrigSize)               outp32((USBD_MXPLD0+(0x10*(u32EpId))), (u32TrigSize))

/* Set toggle bit for EPx (x = 0 ~ 5) */
#define _DRVUSB_SET_EP_TOG_BIT(u32EpId, bData0)             outp32(USBD_CFG0+(0x10*(u32EpId)), bData0 ? _DRVUSB_DATA0(inp32(USBD_CFG0+(0x10*(u32EpId)))) : _DRVUSB_DATA1(inp32(USBD_CFG0+(0x10*(u32EpId)))))

/* Get Data Out size for EPx (x = 0 ~ 5) */
#define _DRVUSB_GET_EP_DATA_SIZE(u32EpId)                   inp16(USBD_MXPLD0 + (0x10*(u32EpId)))

#define _DRVUSB_SET_EVENT_FLAG(u32Data)                     outp32((&USBD->INTSTS), (u32Data))  

#define _DRVUSB_GET_EVENT_FLAG()                            inp32((&USBD->INTSTS))

/* Clear EPx (x = 0 ~ 5) Force device to response STALL */
#define _DRVUSB_CLEAR_EP_STALL(u32EpId)                     outp32(USBD_CFGP0+(0x10 * (u32EpId)), inp32(USBD_CFGP0+(0x10 * (u32EpId))) & ~CFGP_SSTALL)
    
/* Trigger EPx (x = 0 ~ 5) Force device to response STALL */
#define _DRVUSB_TRIG_EP_STALL(u32EpId)                      outp32(USBD_CFGP0+(0x10*(u32EpId)), inp32(USBD_CFGP0+(0x10*(u32EpId))) | CFGP_SSTALL)

/* Clear EPx (x = 0 ~ 5) Specify Data 0 or 1 after IN token toggle automatically after host ACK */
#define _DRVUSB_CLEAR_EP_DSQ_SYNC(u32EpId)                  outp32(USBD_CFG0+(0x10*(u32EpId)), inp32(USBD_CFG0+(0x10*(u32EpId))) & ~CFG_DSQ_SYNC)

/* Configure Set CFGx (x = 0 ~ 5) */
#define _DRVUSB_SET_CFG(u32CFGNum,u32Data)                  outp32(USBD_CFG0+(0x10*(u32CFGNum)), (u32Data) & 0x03FF)

/* Configure Get CFGx (x = 0 ~ 5) */    
#define _DRVUSB_GET_CFG(u32CFGNum)                          (inp32(USBD_CFG0 + (u32CFGNum) * 0x10) & 0x03FF)

/* Set Function Address */
#define _DRVUSB_SET_FADDR(u32Addr)                          (USBD->FADDR.FADDR = u32Addr)

/* Get Function Address */  
#define _DRVUSB_GET_FADDR()                                 (USBD->FADDR.FADDR)

/* Get System states */
#define _DRVUSB_GET_EPSTS()                                 inp32((&USBD->EPSTS))

/* Configure Set CFGPx (x = 0 ~ 5) */
#define _DRVUSB_SET_CFGP(u8CFGPNum, u32Data)                outp32(USBD_CFGP0+(0x10 * (u8CFGPNum)), (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP0(u32Data)                        outp32(USBD_CFGP0, (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP1(u32Data)                        outp32(USBD_CFGP1, (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP2(u32Data)                        outp32(USBD_CFGP2, (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP3(u32Data)                        outp32(USBD_CFGP3, (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP4(u32Data)                        outp32(USBD_CFGP4, (u32Data) & 0x03)

#define _DRVUSB_SET_CFG_EP5(u32Data)                        outp32(USBD_CFGP5, (u32Data) & 0x03)

/* Configure Get CFGPx (x = 0 ~ 5) */
#define _DRVUSB_GET_CFGP(u32CFGPNum)                        (inp32(USBD_CFGP0+(0x10 * (u32CFGPNum))) & 0x03)
 
/* Enable USB , PHY and use remote wake-up */
#define _DRVUSB_ENABLE_USB()                                outp32((&USBD->ATTR), 0x7D0)

/* Disable USB, PHY and use remote wake-up */
#define _DRVUSB_DISABLE_USB()                               outp32((&USBD->ATTR), 0x760)

/* Disable PHY and don't use remote wake-up */
#define _DRVUSB_DISABLE_PHY()                               outp32((&USBD->ATTR), 0x7C0)

#define _DRVUSB_ENABLE_SE0()                                (USBD->DRVSE0.DRVSE0 = 1)

#define _DRVUSB_DISABLE_SE0()                               (USBD->DRVSE0.DRVSE0 = 0)


/*---------------------------------------------------------------------------------------------------------*/
/* USB Request                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
/* Request Type */
#define REQ_STANDARD        0x00
#define REQ_CLASS           0x20
#define REQ_VENDOR          0x40

/* Request */
#define GET_STATUS          0x00
#define CLEAR_FEATURE       0x01

#define SET_FEATURE         0x03

#define SET_ADDRESS         0x05
#define GET_DESCRIPTOR      0x06
#define SET_DESCRIPTOR      0x07
#define GET_CONFIGURATION   0x08
#define SET_CONFIGURATION   0x09
#define GET_INTERFACE       0x0A
#define SET_INTERFACE       0x0B
#define SYNC_FRAME          0x0C


/*---------------------------------------------------------------------------------------------------------*/
/* USB Descriptor Type                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define DESC_DEVICE         0x01
#define DESC_CONFIG         0x02
#define DESC_STRING         0x03
#define DESC_INTERFACE      0x04
#define DESC_ENDPOINT       0x05
#define DESC_QUALIFIER      0x06
#define DESC_OTHERSPEED     0x07
/* For HID */
#define DESC_HID            0x21
#define DESC_HID_RPT        0x22

/*---------------------------------------------------------------------------------------------------------*/
/* USB Descriptor Length                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define LEN_DEVICE          18
#define LEN_CONFIG          9
#define LEN_INTERFACE       9
#define LEN_ENDPOINT        7
/* For HID */
#define LEN_HID             0x09

/* USB Endpoint Type */
#define EP_ISO              0x01
#define EP_BULK             0x02
#define EP_INT              0x03

/*---------------------------------------------------------------------------------------------------------*/
/* USB Feature Selector                                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define FEATURE_DEVICE_REMOTE_WAKEUP    0x01
#define FEATURE_ENDPOINT_HALT           0x00

#define EP_INPUT                        0x80
#define EP_OUTPUT                       0x00
#define MAX_EP_ID                       6
#define USB_DATA_QUEUE_SIZE             16
#define USB_SRAM_ADDR g_UsbSramBase
#define USB_BUF_SETUP                   DrvUSB_GetSetupBuffer()

#define DRVUSB_EVENT_FLAG_ATTACH        0x00000001
#define DRVUSB_EVENT_FLAG_DETACH        0x00000002
#define DRVUSB_EVENT_FLAG_BUS_RESET     0x00000004
#define DRVUSB_EVENT_FLAG_BUS_SUSPEND   0x00000008
#define DRVUSB_EVENT_FLAG_BUS_RESUME    0x00000010
#define DRVUSB_EVENT_FLAG_SETUP         0x00000020
    
/*---------------------------------------------------------------------------------------------------------*/
/* USB Device state                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum
{
    eDRVUSB_DETACHED    = 0,
    eDRVUSB_ATTACHED    = 0x00000001,
    eDRVUSB_POWERED     = eDRVUSB_ATTACHED + 0x00000002,
    eDRVUSB_DEFAULT     = eDRVUSB_POWERED  + 0x00000004,
    eDRVUSB_ADDRESS     = eDRVUSB_DEFAULT  + 0x00000008,
    eDRVUSB_CONFIGURED  = eDRVUSB_ADDRESS  + 0x00000010,
    eDRVUSB_SUSPENDED   = 0x00000020

} E_DRVUSB_STATE;


/*---------------------------------------------------------------------------------------------------------*/
/* USB Vender_info descriptor structure                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint8_t byLength;
    uint8_t byDescType;
    uint16_t au16UnicodeString[100];
    
} S_DRVUSB_STRING_DESC;

typedef S_DRVUSB_STRING_DESC    S_STRING_DESC;

typedef struct
{
    uint16_t u16VendorId;
    uint16_t u16ProductId;
    const S_DRVUSB_STRING_DESC *psVendorStringDesc;
    const S_DRVUSB_STRING_DESC *psProductStringDesc;

} S_DRVUSB_VENDOR_INFO;

typedef S_DRVUSB_VENDOR_INFO    S_VENDOR_INFO;


/*---------------------------------------------------------------------------------------------------------*/
/* Typedef USB Callback function                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVUSB_ATTACH_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_DETACH_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_BUS_RESET_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_BUS_SUSPEND_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_BUS_RESUME_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_SETUP_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_EP_IN_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_EP_OUT_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_CTRL_SETUP_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_CTRL_DATA_IN_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_CTRL_DATA_OUT_CALLBACK)(void *);


/*---------------------------------------------------------------------------------------------------------*/
/* Typedef ISR USB Callback function                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef void (*PFN_DRVUSB_ISR_ATTACH_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_ISR_DETACH_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_ISR_BUS_RESET_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_ISR_BUS_SUSPEND_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_ISR_BUS_RESUME_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_ISR_SETUP_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_ISR_EP_IN_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_ISR_EP_OUT_CALLBACK)(void *);

typedef void (*PFN_DRVUSB_CALLBACK)(void *);
typedef void (*PFN_DRVUSB_INTCALLBACK)(void *);
typedef int32_t (*PFN_DRVUSB_COMPARE)(uint8_t);


/*---------------------------------------------------------------------------------------------------------*/
/* USB Control callback function structure                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint8_t u8RequestType;
    uint8_t u8Request;
    PFN_DRVUSB_CTRL_SETUP_CALLBACK      pfnCtrlSetupCallback;
    PFN_DRVUSB_CTRL_DATA_IN_CALLBACK    pfnCtrlDataInCallback;
    PFN_DRVUSB_CTRL_DATA_OUT_CALLBACK   pfnCtrlDataOutCallback;
    void *                              pVoid;
} S_DRVUSB_CTRL_CALLBACK_ENTRY;

typedef struct
{
    int32_t                             bReg;
    S_DRVUSB_CTRL_CALLBACK_ENTRY        sCtrlCallbackEntry;
} S_DRVUSB_CTRL_CALLBACK_ENTRY_REG;


/*---------------------------------------------------------------------------------------------------------*/
/* EP ctrl structure. USB class must implement this structure                                              */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint32_t u32EpNum;
    uint32_t u32MaxPacketSize; 
    uint8_t *u8SramBuffer;
}S_DRVUSB_EP_CTRL;


/*---------------------------------------------------------------------------------------------------------*/
/* Misc and USB event call back function                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    PFN_DRVUSB_CALLBACK apfnCallback;
    void *              apCallbackArgu;
}S_DRVUSB_EVENT_PROCESS;


/*---------------------------------------------------------------------------------------------------------*/
/* The interface between USB class and USB driver                                                          */
/* USB class implement this structure and register to USB driver                                           */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    void * device;
    PFN_DRVUSB_CALLBACK pfnStart;       /* The callback function when set configuration or set interface */
    PFN_DRVUSB_COMPARE  pfnCompare;
    void * pVoid;
}S_DRVUSB_CLASS;


/*---------------------------------------------------------------------------------------------------------*/
/* USB driver device                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
    uint32_t u32ATTR;
    uint32_t u32EPSTS;
    uint32_t u32FLDET;
    uint32_t u32INTSTS;

    int32_t  abData0[MAX_EP_ID];  /* Index is EP identity   */
    S_DRVUSB_EP_CTRL sEpCrl[MAX_EP_ID+1];
    int32_t  eUsbState;           /* E_DRVUSB_STATE */

    uint16_t u16MiscEventFlags;

    uint16_t u16EPEventFlags;

    S_DRVUSB_CTRL_CALLBACK_ENTRY    *pCtrlCallback;
    uint32_t CtrlCallbackSize;
    uint8_t  au8Setup[8];
    uint8_t  u8UsbAddress;
    uint8_t  u8UsbConfiguration;
        
    int32_t  bSelfPowered;  /* TRUE : Self-Powereded, FALSE : BUS-Powereded */
    int32_t  bRemoteWakeup; /* TRUE : Support RemoteWakeup, FALSE : no support */
    uint8_t  u8MaxPower;        /* Maximum MaxPower 0xFA (500mA), default is 0x32 (100mA) */
        
    S_DRVUSB_CLASS *psUsbClass;
} S_DRVUSB_DEVICE;


/* EP ctrl structure;descript EP */
extern S_DRVUSB_EP_CTRL sEpDescription[];
/* BUS event call back function; USB class must implement */
extern S_DRVUSB_EVENT_PROCESS g_sBusOps[];
/* USB event call back function; USB class must implement */
extern S_DRVUSB_EVENT_PROCESS g_sUsbOps[];
extern uint32_t CFG_EP_SETTING[6];  /* CFGx register setting value */
extern PFN_DRVUSB_INTCALLBACK g_FnIntCallBack;  /* Interrupt call back  */
extern S_DRVUSB_DEVICE gsUsbDevice;


/*---------------------------------------------------------------------------------------------------------*/
/* Get/Set USB device state                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
/* DATA0 or DATA1 Out transaction when Data Out ACK happens */
int32_t DrvUSB_IsData0Out(uint32_t u32EpId);

/* Get USB device state. */
E_DRVUSB_STATE DrvUSB_GetUsbState(void);

/* Set USB device state. */
void DrvUSB_SetUsbState(E_DRVUSB_STATE eUsbState);


/*---------------------------------------------------------------------------------------------------------*/
/* Define DrvUSB READ/WRITE USB BUFFER                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
#define _DRVUSB_READ_USB_BUF(UsbBuf, Mem, Len) \
    memcpy ((void *) (Mem), (void *) (UsbBuf), (Len))

#define _DRVUSB_WRITE_USB_BUF(UsbBuf, Mem, Len) \
    memcpy ((void *) (UsbBuf), (void *) (Mem), (Len))


/*---------------------------------------------------------------------------------------------------------*/
/* APIs declare                                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvUSB_GetVersion(void);
int32_t  DrvUSB_Open(void * pVoid);
void     DrvUSB_PreDispatchEvent(void);
void     DrvUSB_DispatchEvent(void);
void     DrvUSB_Close(void);
void     DrvUSB_BusResetCallback(void * pVoid);
uint32_t DrvUSB_GetSetupBuffer(void);
void *   DrvUSB_InstallClassDevice(S_DRVUSB_CLASS *sUsbClass);
int32_t  DrvUSB_InstallCtrlHandler(void * *device, S_DRVUSB_CTRL_CALLBACK_ENTRY *psCtrlCallbackEntry, uint32_t u32RegCnt);
int32_t  DrvUSB_DataOutTrigger(uint32_t u32EpNum, uint32_t u32Size);
uint8_t* DrvUSB_GetOutData(uint32_t u32EpNum, uint32_t *u32Size);
int32_t  DrvUSB_DataIn(uint32_t u32EpNum, const uint8_t * u8Buffer, uint32_t u32Size);
void     DrvUSB_CtrlSetupAck(void * pArgu);
void     DrvUSB_CtrlDataInAck(void * pArgu);
void     DrvUSB_CtrlDataOutAck(void * pArgu);
void     DrvUSB_ClrCtrlReady(void);
void     DrvUSB_ClrCtrlReadyAndTrigStall(void);
void     DrvUSB_CtrlDataInDefault(void * pVoid);
void     DrvUSB_CtrlDataOutDefault(void * pVoid);
void     DrvUSB_Reset(uint32_t u32EpNum);
int32_t DrvUSB_GetEpId(uint32_t u32EpNum);
int32_t DrvUSB_GetEpIdentity(uint32_t u32EpNum, uint32_t u32EPAttr);
void     DrvUSB_EnableRemoteWakeup(void);
void     DrvUSB_DisableRemoteWakeup(void);
void     DrvUSB_EnableSelfPower (void);
void     DrvUSB_DisableSelfPower (void);
int32_t  DrvUSB_IsSelfPowerEnabled (void);          /* TRUE : Self-Powereded, FALSE : BUS-Powereded */
void     DrvUSB_EnableRemoteWakeup(void);
void     DrvUSB_DisableRemoteWakeup(void);
int32_t  DrvUSB_IsRemoteWakeupEnabled (void);       /* TRUE : Support RemoteWakeup, FALSE : not support */
int32_t  DrvUSB_SetMaxPower(uint32_t u32MaxPower);  /* Maximum MaxPower 0xFA (500mA), default is 0x32 (100mA) */
int32_t  DrvUSB_GetMaxPower(void);                  /* Maximum MaxPower 0xFA (500mA), default is 0x32 (100mA)   */

void DrvUSB_CtrlSetupSetAddress(void * pVoid);
void DrvUSB_CtrlSetupClearSetFeature(void * pVoid);
void DrvUSB_CtrlSetupGetConfiguration(void * pVoid);
void DrvUSB_CtrlSetupGetStatus(void * pVoid);
void DrvUSB_CtrlSetupGetInterface(void * pVoid);
void DrvUSB_CtrlSetupSetInterface(void * pVoid);
void DrvUSB_CtrlSetupSetConfiguration(void * pVoid);
void DrvUSB_CtrlDataInSetAddress(void * pVoid);

uint32_t DrvUSB_GetVersion(void);
int32_t DrvUSB_IsData0(uint32_t u32EpId);

void DrvUSB_EnableUsb(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_DisableUsb(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_PreDispatchWakeupEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_PreDispatchFDTEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_PreDispatchEPEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_PreDispatchBusEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_DispatchWakeupEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_DispatchMiscEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_DispatchEPEvent(S_DRVUSB_DEVICE *psDevice);
void DrvUSB_memcpy(uint8_t *pi8Dest, uint8_t *pi8Src, uint32_t u32Size);
uint32_t DrvUSB_GetFreeSRAM(void);
    
#ifdef __cplusplus
}
#endif

#endif 



