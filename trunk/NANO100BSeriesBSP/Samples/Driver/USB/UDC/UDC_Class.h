/******************************************************************************
 * @file     UDC_Class.h
 * @brief    Nano1xx USB Driver Sample header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/***************************************************/
/*    Mass Storage Class-Specific Request Codes	   */
/***************************************************/
/*!<Define Mass Storage Class Specific Request */
#define	BULK_ONLY_MASS_STORAGE_RESET	0xFF
#define	GET_MAX_LUN						0xFE


/***************************************************/
/*    Mass Storage Class Information			   */
/***************************************************/
/*!<Define Mass Storage Class Information */
#define DATA_FLASH_STORAGE_SIZE		(30*1024)	/* Configure the DATA FLASH storage size */
#define MASS_BUFFER_SIZE			48			/* Mass Storage command buffer size */
#define STORAGE_BUFFER_SIZE			512			/* Data transfer buffer size in 512 bytes alignment */
#define	UDC_SECTOR_SIZE				512			/* logic sector size */


/*!<Define Mass Storage State */
#define	BULK_CBW			0x00
#define	BULK_IN				0x01
#define	BULK_OUT			0x02
#define	BULK_CSW			0x04
#define BULK_NORMAL			0xFF


/*!<Define Mass Storage Signature */
#define	CBW_SIGNATURE		0x43425355
#define	CSW_SIGNATURE		0x53425355


/*!<Define Mass Storage UFI Command */
#define	UFI_TEST_UNIT_READY						0x00
#define	UFI_REQUEST_SENSE						0x03
#define	UFI_INQUIRY								0x12
#define	UFI_MODE_SELECT_6						0x15
#define	UFI_MODE_SENSE_6						0x1A
#define	UFI_START_STOP							0x1B
#define	UFI_PREVENT_ALLOW_MEDIUM_REMOVAL		0x1E
#define	UFI_READ_FORMAT_CAPACITY				0x23
#define	UFI_READ_CAPACITY						0x25
#define	UFI_READ_10								0x28
#define	UFI_WRITE_10							0x2A
#define	UFI_VERIFY_10							0x2F
#define	UFI_MODE_SELECT_10						0x55
#define	UFI_MODE_SENSE_10						0x5A



/******************************************************************************/
/*                USBD Mass Storage Structure                                 */
/******************************************************************************/
/** @addtogroup NANO1XX_USBD_Mass_Exported_Struct NANO1XX USBD Mass Exported Struct
  NANO1XX USBD Mass Specific Struct
  @{
*/

/*!<USB Mass Storage Class - Command Block Wrapper Structure */
struct CBW
{
	uint32_t	dCBWSignature;
	uint32_t	dCBWTag;
	uint32_t	dCBWDataTransferLength;
	uint8_t		bmCBWFlags;
	uint8_t		bCBWLUN;
	uint8_t		bCBWCBLength;
	uint8_t		u8OPCode;
	uint8_t		u8LUN;
	uint8_t		au8Data[14];
};

/*!<USB Mass Storage Class - Command Status Wrapper Structure */
struct CSW
{
	uint32_t	dCSWSignature;
	uint32_t	dCSWTag;
	uint32_t	dCSWDataResidue;
	uint8_t		bCSWStatus;
};



/*!<USB Mass Storage Class Information Structure */
typedef struct DRVUSBD_MASS_STRUCT
{
	uint8_t BulkState;
	uint8_t SenseKey[4];
	uint8_t preventflag;
	uint8_t Size;
	uint8_t F_DATA_FLASH_LUN;

	uint32_t DataFlashStartAddr;
	uint32_t Address;
	uint32_t Length;
	uint32_t LbaAddress;
	uint32_t BytesInStorageBuf;
	uint32_t BulkBuf0;
	uint32_t BulkBuf1;

	int32_t dataFlashTotalSectors;


	/* CBW/CSW variables */
	struct CBW sCBW;
	struct CSW sCSW;

}STR_USBD_MASS_T;

/*@}*/ /* end of group NANO1XX_USBD_Mass_Exported_Struct */


/* DATA Flash Programming API */
extern void DataFlashRead(uint32_t addr, uint32_t size, uint32_t buffer);
extern void DataFlashWrite(uint32_t addr, uint32_t size, uint32_t buffer);

void ModeSenseCommand(void);
void ReqSenCommand(void);
void RdFmtCapCommand(void);
void RdCurCapCommand(void);
void UsbRead(void);
void UsbReadTrig(void);
void UsbWrite(void);

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

