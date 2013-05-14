/*-----------------------------------------------------------------------*/
/* Low level disk control module for Win32              (C)ChaN, 2007    */
/*-----------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>

#include "diskio.h"
#include "sdcard.h"
#include "spiFlash.h"

#define STORAGE_BUFFER_SIZE 1024        /* Data transfer buffer size in 512 bytes alignment */
uint32_t Storage_Block[STORAGE_BUFFER_SIZE / 4];
#define STORAGE_DATA_BUF   ((uint32_t)&Storage_Block[0])
//extern void SpiRead(uint32_t addr, uint32_t size, uint32_t buffer);
//extern void SpiRead(uint32_t addr, uint32_t size, uint8_t buffer);
extern void SpiRead(uint32_t addr, uint32_t size, uint8_t *buffer);
//extern void SpiWrite(uint32_t addr, uint32_t size, uint32_t buffer);
extern void SpiWrite(uint32_t addr, uint32_t size,  uint8_t *buffer);
extern int8_t Is_Initialized ;


static uint8_t bUseSDCard;

void RoughDelay(uint32_t t)
{
    volatile int32_t delay;

    delay = t;

    while(delay-- >= 0);
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber */
)
{
	DSTATUS sta;
	uint32_t timeout = 0;
	uint32_t id;

	//card detect = GPB14
	GCR->PB_H_MFP = (GCR->PB_H_MFP & ~PB14_MFP_MASK) | PB14_MFP_GPB14;
	GPIOB->PMD = (GPIOB->PMD & ~GPIO_PMD_PMD14_MASK) | GPIO_PMD_PMD14_INPUT;
	
	printf("Check SD Card insert ?\n");
	while(GPIOBITB->GP_BIT14 == 1)
	{	
		if(timeout++ > 0x200)
			break;
	}
	
	bUseSDCard = 0;
	if(GPIOBITB->GP_BIT14 != 1)
	{
		printf("Card Detected!!\n");
		
		//power pin(GPE6)
		GCR->PE_L_MFP = (GCR->PE_L_MFP & ~PE6_MFP_MASK) | PE6_MFP_GPE6;
		GPIOE->PMD = (GPIOE->PMD & ~GPIO_PMD_PMD6_MASK) | GPIO_PMD_PMD6_OUTPUT;
		GPIOBITE->GP_BIT6 = 0;
		
		RoughDelay(100000);
		if(SDCARD_Open() == E_SUCCESS)
		{	
			sta = 	RES_OK;
			printf("SDCard Open success\n");
			bUseSDCard = 1;
		}
		else
		{
			sta = STA_NOINIT;
			printf("SDCard Open failed\n");
		}
	}
	else
	{	
		printf("Can't detect card !!\n");
		
		/* Set SPI0 pin, GPE1->SS0; GPE2->CLK, GPE3->MISO0, GPE4->MOSI0 */
                GCR->PE_L_MFP = GCR->PE_L_MFP & ~0x000ffff0|0x00066660;
		CLK->CLKSEL2 = (CLK->CLKSEL2 & CLK_CLKSEL2_SPI0_MASK) | CLK_CLKSEL2_SPI0_PLL;
	
		//can't find SD card, use SPI flash instead.
		spiFlashInit(SPI0);
	
		/* Check Flash ID */
		if((id = spiFlashReadID()) == 0)
		{
			printf("Sorry!! The ID (0x%x)is wrong!!\n", id);
			sta = STA_NOINIT;
		}
		else
			sta = RES_OK;
	}
	
	return sta;

}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{   
	DSTATUS sta1=STA_OK;
	if (drv) 
		sta1 =   STA_NOINIT;
	return sta1;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
#define DRVSDCARD_BLOCK_SIZE 512
extern int8_t SDtype;

DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	DRESULT res;
	uint32_t size;

	if (drv) {
		res = (DRESULT)STA_NOINIT;	
		return res;
	}

	if(count==0||count>=2)
	{	 
		res = (DRESULT)STA_NOINIT;
		return res;
	}
	
	size = count*512;
	if(bUseSDCard)
		SpiRead(sector, size, buff);
	else
		spiFlashReadData(sector*512, size, buff);
		
	res = RES_OK;	/* Clear STA_NOINIT */;
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{

	DRESULT  res;	
	uint32_t size;
	uint32_t scount;
	
	if (drv) {
		res = (DRESULT)STA_NOINIT;	
		return res;
	}	
	 
	if(count==0||count>=2)
	{	 
		res = (DRESULT)  STA_NOINIT;
		return res;
	}
	
	size=count*512;
	
	if(bUseSDCard)
		SpiWrite(sector, size,(uint8_t *)buff);
	else
	{
		scount = size / (4*1024);
		if ((size % (4*1024)) != 0)
			scount++;
		
		spiFlashSectorErase(sector*512, scount);
		spiFlashCheckBusy();
		spiFlashPageProgram(sector*512, size,(uint8_t *)buff);
	}
	res = RES_OK;

	return res;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	DRESULT res;

//	BYTE n;

	if (drv) return RES_PARERR;

	switch (ctrl) {
	case CTRL_SYNC :		/* Make sure that no pending write process */
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
		if(bUseSDCard)
			SDCARD_GetCardSize(buff);
		else
			*(DWORD*)buff = 8192;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
		if(bUseSDCard)
			*(DWORD*)buff = 512;	//512;
		else
			*(DWORD*)buff = 256;	//512;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		*(DWORD*)buff = 1;
		res = RES_OK;
		break;


	default:
		res = RES_PARERR;
	}


	res = RES_OK;


	return res;
}




