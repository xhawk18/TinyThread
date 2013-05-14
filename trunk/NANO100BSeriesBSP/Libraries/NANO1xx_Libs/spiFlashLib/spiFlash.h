/******************************************************************************
 * @file     spiFlash
 * @brief    This file provides all the SPI Flash firmware functions
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/ 
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include "NANO1xx.h"

#define SPI_FLASH_DONE	1
#define SPI_FLASH_ERROR	0

#define SPI_FLASH_CMD_WRITE_STATUS		0x01
#define SPI_FLASH_CMD_PAGE_PROGRAM		0x02
#define SPI_FLASH_CMD_READ_DATA			0x03
#define SPI_FLASH_CMD_WRITE_DISABLE		0x04
#define SPI_FLASH_CMD_READ_STATUS		0x05
#define SPI_FLASH_CMD_WRITE_ENABLE		0x06
#define SPI_FLASH_CMD_FAST_READ			0x0B
#define SPI_FLASH_CMD_SECTOR_ERASE		0x20
#define SPI_FLASH_CMD_BLOCK_ERASE		0xD8
#define SPI_FLASH_CMD_CHIP_ERASE		0xC7
#define SPI_FLASH_CMD_READ_ID			0x90
#define SPI_FLASH_CMD_SECTOR_ERASE		0x20

void spiFlashInit(SPI_TypeDef *SpiPort);
void SpiFalshClose(void);
uint8_t spiFlashCheckBusy(void);
uint8_t spiFlashReadData(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf);
uint8_t spiFlashFastRead(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf);
uint8_t spiFlashWriteEnable(void);
uint8_t spiFlashWriteDisable(void);
uint8_t spiFlashPageProgram(uint32_t spiAddr, uint32_t DataLen, uint8_t *pDataBuf);
uint8_t spiFlashSectorErase(uint32_t spiAddr, uint32_t SectorCnt);
uint8_t spiFlashPageErase(uint32_t spiAddr, uint32_t PageCnt);
uint8_t spiFlashChipErase(void);
uint16_t spiFlashReadID(void);
uint8_t spiFlashReadStatus(void);
uint8_t spiFlashWriteStatus(uint8_t RegData);

#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
