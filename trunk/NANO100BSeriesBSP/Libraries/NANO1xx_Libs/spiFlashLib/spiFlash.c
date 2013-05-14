/******************************************************************************
 * @file     spiFlash
 * @brief    This file provides all the SPI Flash firmware functions
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "nano1xx_spi.h"
#include "spiFlash.h"

SPI_TypeDef *TempSpiPort;
uint32_t spiTxData[2], spiRxData[2];

void spiFlashInit(SPI_TypeDef *SpiPort)
{
	SPI_DATA_T conf;
	
	SPI_Init(SpiPort);
	
	conf.u32Mode = SPI_MODE_MASTER;
	conf.u32Type = SPI_TYPE0;
	conf.i32BitLength = 32;
	
	/* Configure SPI as a master, 32-bit transaction */
	SPI_Open(SpiPort, &conf);
	
	/* Diable AutoSS */
	SPI_DisableAutoSS(SpiPort);
	
	/* SPI clock rate 10MHz */
	SPI_SetClockFreq(SpiPort, 10000000, 0);
	
	TempSpiPort = SpiPort;
	
	memset(&spiTxData, 0, sizeof(spiTxData));
	memset(&spiRxData, 0, sizeof(spiRxData));
	
	SPI_DisableInt(SpiPort);
}

void SpiFalshClose(void)
{
	SPI_Close(TempSpiPort);
	TempSpiPort = NULL;
}

uint8_t spiFlashCheckBusy(void)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_READ_STATUS;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	spiTxData[0] = 0xFF;	
	
	//loop until device is not busy
	while(1)
	{
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
		
		SPI_DumpRxRegister(TempSpiPort, &spiRxData[0], 1);
		if((spiRxData[0] & 0x01) != 0x01)
			break;
	}
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashReadData(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf)
{
	uint32_t i;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_READ_DATA;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
		
	// address (24bit)
	SPI_SetBitLength(TempSpiPort, 24);
	spiTxData[0] = spiAddr;
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// data (8bit*n)
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = 0xFF;
	for (i=0; i<DataLen; i++)
	{
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
		SPI_DumpRxRegister(TempSpiPort, &spiRxData[0], 1);
		*(pDataBuf++) = spiRxData[0] & 0xff;
	}
		
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashFastRead(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf)
{
	uint32_t i;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
			
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_FAST_READ;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// address (24bit)
	SPI_SetBitLength(TempSpiPort, 24);
	spiTxData[0] = spiAddr;
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// dummy (8bit)
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = 0xFF;
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// data (8bit*n)
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = 0xFF;
	for (i=0; i<DataLen; i++)
	{
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
		SPI_DumpRxRegister(TempSpiPort, &spiRxData[0], 1);
		*(pDataBuf++) = spiRxData[0] & 0xff;
	}
		
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashWriteEnable(void)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_WRITE_ENABLE;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashWriteDisable(void)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_WRITE_DISABLE;
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashPageProgram(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf)
{
	uint32_t PageCount, ByteCount, i;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	PageCount = DataLen / 256;
	
	if ((DataLen % 256) != 0)
		PageCount++;
	
	for (i=0; i<PageCount; i++)
	{		
		spiFlashWriteEnable();
		
		if(DataLen > 256)
		{	
			ByteCount = 256;
			DataLen -= 256;
		}
		else
			ByteCount = DataLen;
			
		// command (8bit)	
		SPI_SetBitLength(TempSpiPort, 8);
		spiTxData[0] = SPI_FLASH_CMD_PAGE_PROGRAM;	
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		
		SPI_SetSS(TempSpiPort, SPI_SS0);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
	
		// address (24bit)
		SPI_SetBitLength(TempSpiPort, 24);
		spiTxData[0] = spiAddr+i*256;
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
		
		while(ByteCount --)
		{
			// data (8bit*n)
			SPI_SetBitLength(TempSpiPort, 8);
			spiTxData[0] = *(pDataBuf++);
			SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
			SPI_SetGo(TempSpiPort);
			while(SPI_IsBusy(TempSpiPort));
		}
		SPI_ClrSS(TempSpiPort, SPI_SS0);
		
		spiFlashCheckBusy();		
	}	
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashSectorErase(uint32_t spiAddr, uint32_t SectorCnt)
{
	uint32_t i;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(spiAddr % (4*1024) != 0)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	spiFlashWriteEnable();	
	
	for (i=0; i<SectorCnt; i++)
	{			
		SPI_SetSS(TempSpiPort, SPI_SS0);
		
		// command (8bit)	
		SPI_SetBitLength(TempSpiPort, 8);
		spiTxData[0] = SPI_FLASH_CMD_SECTOR_ERASE;	
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
	
		SPI_SetBitLength(TempSpiPort, 24);
		spiTxData[0] = spiAddr+i*(4*1024);	
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);		
		while(SPI_IsBusy(TempSpiPort));
	
		SPI_ClrSS(TempSpiPort, SPI_SS0);
		
		spiFlashCheckBusy();
	}
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashBlockErase(uint32_t spiAddr, uint32_t BlockCnt)
{
	uint32_t i;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(spiAddr % (64*1024) != 0)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	spiFlashWriteEnable();
		
	for (i=0; i<BlockCnt; i++)
	{			
		SPI_SetSS(TempSpiPort, SPI_SS0);
		
		// command (8bit)	
		SPI_SetBitLength(TempSpiPort, 8);
		spiTxData[0] = SPI_FLASH_CMD_BLOCK_ERASE;	
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);
		while(SPI_IsBusy(TempSpiPort));
	
		SPI_SetBitLength(TempSpiPort, 24);
		spiTxData[0] = spiAddr+i*(64*1024);	
		SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
		SPI_SetGo(TempSpiPort);		
		while(SPI_IsBusy(TempSpiPort));
	
		SPI_ClrSS(TempSpiPort, SPI_SS0);
		
		spiFlashCheckBusy();
	}
	
	return SPI_FLASH_DONE;
}

uint8_t spiFlashChipErase(void)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	
	spiFlashWriteEnable();
		
	SPI_SetSS(TempSpiPort, SPI_SS0);
	
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_CHIP_ERASE;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);		
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	spiFlashCheckBusy();
	
	return SPI_FLASH_DONE;
}

uint16_t spiFlashReadID(void)
{
	uint16_t Id;
	
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
		
	// command (8bit)	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_READ_ID;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);		
	
	SPI_SetSS(TempSpiPort, SPI_SS0);	
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// address (24bit)	
	SPI_SetBitLength(TempSpiPort, 24);
	spiTxData[0] = 0x000000;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);		
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	// id (16bit)	
	SPI_SetBitLength(TempSpiPort, 16);
	spiTxData[0] = 0xffff;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);		
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
		
	SPI_DumpRxRegister(TempSpiPort, &spiRxData[0], 1);
	Id = spiRxData[0] & 0xffff;
		
	return Id;
}

uint8_t spiFlashReadStatus(void)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;
	

	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_READ_STATUS;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	spiTxData[0] = 0xFF;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	SPI_DumpRxRegister(TempSpiPort, &spiRxData[0], 1);
	
	SPI_ClrSS(TempSpiPort, SPI_SS0);
	
	return (spiRxData[0] & 0xff);
}

uint8_t spiFlashWriteStatus(uint8_t RegData)
{
	if(TempSpiPort == NULL)
		return SPI_FLASH_ERROR;
	
	if(SPI_IsBusy(TempSpiPort)== TRUE)
		return SPI_FLASH_ERROR;	
	
	SPI_SetBitLength(TempSpiPort, 8);
	spiTxData[0] = SPI_FLASH_CMD_WRITE_STATUS;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	
	SPI_SetSS(TempSpiPort, SPI_SS0);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
	
	spiTxData[0] = RegData;	
	SPI_SetTxRegister(TempSpiPort, &spiTxData[0], 1);
	SPI_SetGo(TempSpiPort);
	while(SPI_IsBusy(TempSpiPort));
		
	SPI_ClrSS(TempSpiPort, SPI_SS0);	
	
	return SPI_FLASH_DONE;
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
