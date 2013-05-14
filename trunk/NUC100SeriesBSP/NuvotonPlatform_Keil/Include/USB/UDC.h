/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef _NUC100_UDC_H_
#define _NUC100_UDC_H_

#include "NUC1xx.h"
#include "Driver\DrvUSB.h"
//======================================================

#define IEF_wakeupEn    0x00000100    // R/W  1/0 Enable/Disable USB wakeup function
#define IEF_INNAKen     0x00008000    // W  1/0 Enable/Disable IN NAK Interrupt

#define EVF_INT_BUS     0x00000001    // R/WC
#define EVF_INT_USB     0x00000002    // R/WC
#define EVF_INT_FLDET   0x00000004    // R/WC
#define EVF_INT_WAKEUP  0x00000008    // R/WC
#define EVF_INT_EPTF0   0x00010000    // R/WC
#define EVF_INT_EPTF1   0x00020000    // R/WC
#define EVF_INT_EPTF2   0x00040000    // R/WC
#define EVF_INT_EPTF3   0x00080000    // R/WC
#define EVF_INT_EPTF4   0x00100000    // R/WC
#define EVF_INT_EPTF5   0x00200000    // R/WC
#define EVF_INT_SETUP   0x80000000    // R/WC

#define STS_EPT       0x0000000F    // R&C  USB Event Endpoint Number
//#define STS_STS       0x00000070    // R&C  USB Event (IN ACK/NAK, OUT 0/1 ACK, Setup ACK)

#define USB_SETUP_ACK   0x30
#define USB_IN_ACK      0x00
#define USB_OUT_0_ACK   0x20
#define USB_OUT_1_ACK   0x60
#define USB_IN_NAK      0x10
#define USB_ISO         0x70

#define STS_Overrun   0x00000080    // R&C  Out Data more than Max Payload or Setup Data more than 8 Bytes
#define STS0          0x00000700
#define STS1          0x00003800
#define STS2          0x0001C000
#define STS3          0x000E0000
#define STS4          0x00700000
#define STS5          0x03800000

#define ATTR_enUSB    0x00000080      // R/W  1/0 Enable/Disable USB
#define ATTR_SPEED    0x00000040      // R/W  1/0 Full/Low Speed
#define ATTR_RWakeUp  0x00000020      // R/W  1: Force USB Bus to K State, Used for Remote Wake-up
#define ATTR_enPHY    0x00000010      // R/W  1/0 Enable/Disable PHY
#define ATTR_Timeout  0x00000008      // R&C  USB Timeout, No Response more than 2^18 clock time
#define ATTR_Resume   0x00000004      // R&C  USB Resume
#define ATTR_Suspend  0x00000002      // R  USB Suspend
#define ATTR_usbRST   0x00000001      // R&C  USB Bus Reset

#define RCVmode     0x00000001      // R/W  1/0 RCV signal come from RXD/RXDP

#define EPT_stall_ctl 0x00000200      // R/W  1/0 Enable/Disable Auto Clear Stall in Setup Stage
#define EPT_DSQ       0x00000080      // R/W  Specify Data 0 or 1 after IN Token toggle Automatically after Host ACK
#define EPT_state     0x00000060
#define EPT_DISABLE   0
#define EPT_OUT       0x00000020
#define EPT_IN        0x00000040
#define EPT_ISOCH     0x00000010      // R/W  Isochronous, No Handshake
#define EPT_ISO       0x00000010
#define EPT_EPT       0x0000000F
#define EPT_stall     0x00000002      // R/W  Force Device to Response Stall
#define EPT_CFGP      0x00000001      // W&C  Write 1 to Clear In/Out Ready set by MXPLD

#define CBW_SIGNATURE 0x43425355
#define CSW_SIGNATURE 0x53425355

//======================================================
#define DATA0(a)  (a&0x7F)
#define DATA1(a)  (a|0x80)

//======================================================
// USB Request
//======================================================
// Request Type
#define REQ_STANDARD    0x00
#define REQ_CLASS       0x20
#define REQ_VENDOR      0x40

// Request
#define GET_STATUS      0x00
#define CLEAR_FEATURE   0x01
//#define         0x02
#define SET_FEATURE     0x03
//#define         0x04
#define SET_ADDRESS       0x05
#define GET_DESCRIPTOR    0x06
#define SET_DESCRIPTOR    0x07
#define GET_CONFIGURATION 0x08
#define SET_CONFIGURATION 0x09
#define GET_INTERFACE   0x0A
#define SET_INTERFACE   0x0B
#define SYNC_FRAME      0x0C

// USB Descriptor Type
#define DESC_DEVICE     0x01
#define DESC_CONFIG     0x02
#define DESC_STRING     0x03
#define DESC_INTERFACE  0x04
#define DESC_ENDPOINT   0x05
#define DESC_QUALIFIER  0x06
#define DESC_OTHERSPEED 0x07

// USB Descriptor Length
#define LEN_DEVICE      18
#define LEN_CONFIG      9
#define LEN_INTERFACE   9
#define LEN_ENDPOINT    7

// USB Endpoint Type
#define EP_ISO        0x01
#define EP_BULK       0x02
#define EP_INT        0x03

// USB Feature Selector
#define FEATURE_DEVICE_REMOTE_WAKEUP  0x01
#define FEATURE_ENDPOINT_HALT     0x00

//======================================================
// USB Control
//======================================================
// USB STATE
#define USB_STATE_DETACHED      0x00
#define USB_STATE_ATTACHED      0x01
#define USB_STATE_DEFAULT     0x11
#define USB_STATE_ADDRESS     0x31
#define USB_STATE_CONFIGURED    0x71

#define USB_STATE_FLAG_DEFAULT    0x10
#define USB_STATE_FLAG_ADDRESS    0x20
#define USB_STATE_FLAG_CONFIGURED 0x40
#define USB_STATE_FLAG_SUSPENDED  0x80

// FLAG
#define FLAG_OUT_ACK    0x01
#define FLAG_SET_ADDRESS  0x02
#define FLAG_SET_CONFIG   0x04
#define FLAG_SET_INTERFACE  0x08
#define FLAG_GET_MAX_LUN  0x10

// Max Packet Size
#define MAX_PACKET_SIZE0  64
#define MAX_PACKET_SIZE   64

// SRAM
#define BUF_SETUP   0xF8
#define BUF_CTRL    0x00
#define BUF_BULK0   (BUF_CTRL+MAX_PACKET_SIZE0)
#define BUF_BULK1   (BUF_BULK0+MAX_PACKET_SIZE)

// CFG
#define CFG0_SETTING  (CFG_CSTALL | CFG_EPT_IN  | 0) /* EP address 0, control IN  */
#define CFG1_SETTING  (CFG_CSTALL | CFG_EPT_OUT | 0) /* EP address 0, control OUT */
#define CFG2_SETTING  (CFG_EPT_IN  | 2)                  /* EP address 2 */
#define CFG3_SETTING  (CFG_EPT_OUT | 3)                  /* EP address 3 */

// BUF
#define BUF0_SETTING  BUF_CTRL
#define BUF1_SETTING  BUF_CTRL
#define BUF2_SETTING  BUF_BULK1
#define BUF3_SETTING  BUF_BULK0
            
#define g_au8UsbSetup ((uint8_t *)(USBD_BASE+0x100+BUF_SETUP))
#define g_au8UsbCtrl  ((uint8_t *)(USBD_BASE+0x100+BUF_CTRL))
#define g_au8UsbBulk1 ((uint8_t *)(USBD_BASE+0x100+BUF_BULK1))
#define g_au8UsbBulk0 ((uint8_t *)(USBD_BASE+0x100+BUF_BULK0))

//======================================================
#define BULK_ONLY_MASS_STORAGE_RESET  0xFF
#define GET_MAX_LUN           0xFE

//======================================================
#define BULK_CBW  0x00
#define BULK_IN   0x01
#define BULK_OUT  0x02
#define BULK_CSW  0x04
#define BULK_NORMAL 0xFF

//======================================================
#define UFI_TEST_UNIT_READY                 0x00
#define UFI_REQUEST_SENSE                   0x03
#define UFI_INQUIRY                         0x12
#define UFI_MODE_SELECT_6                   0x15
#define UFI_MODE_SENSE_6                    0x1A
#define UFI_START_STOP                      0x1B
#define UFI_PREVENT_ALLOW_MEDIUM_REMOVAL    0x1E
#define UFI_READ_FORMAT_CAPACITY            0x23
#define UFI_READ_CAPACITY                   0x25
#define UFI_READ_10                         0x28
#define UFI_WRITE_10                        0x2A
#define UFI_VERIFY_10                       0x2F
#define UFI_MODE_SELECT_10                  0x55
#define UFI_MODE_SENSE_10                   0x5A

//======================================================
/* for mass storage */
struct CBW
{
  uint32_t  dCBWSignature;
  uint32_t  dCBWTag;
  uint32_t  dCBWDataTransferLength;
  uint8_t   bmCBWFlags;
  uint8_t   bCBWLUN;
  uint8_t   bCBWCBLength;
  uint8_t   u8OPCode;
  uint8_t   u8LUN;
  uint8_t   au8Data[14];
};

struct CSW
{
  uint32_t  dCSWSignature;
  uint32_t  dCSWTag;
  uint32_t  dCSWDataResidue;
  uint8_t   bCSWStatus;
};

typedef struct Disk_Par_Inf 
{
  uint32_t  partition_size,
        data_location,
        bpb_location,
        fat_location,
        rootdir_location,
        free_size;
  uint16_t  rootdirentryno,
        totalcluster,
        NumCyl;
  uint8_t   NumHead,
        NumSector,
        capacity,
        fatcopyno,
        fatsectorno,
        fat16flg;
} Disk_Par_Info;

/* extern flags */
extern uint8_t g_u8BulkState;
extern int32_t udcOnLine;
extern int32_t g_bIsFirstRead;
extern uint32_t usbDataReady;
extern uint8_t g_u8UsbState;

/* extern functions */
void ReqSenCommand(void);
void RdFmtCapCommand(void);
void RdCurCapCommand(void);
uint8_t udcFlashInit(void);

void udcInit(void);  
void udcMassBulk(void);

#endif
