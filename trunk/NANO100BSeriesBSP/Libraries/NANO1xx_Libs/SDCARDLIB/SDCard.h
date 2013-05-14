/******************************************************************************
 * @file     SDCard.h
 * @brief    SD Card library include header file
 * @version  1.0.0
 * @date     21, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

 
#ifndef __DRVSDCARD_H__
#define __DRVSDCARD_H__

// Include header file
#include "nano1xx.h"

// Command Table Index Constants:
// Definitions for each table entry in the command table.
// These allow the MMC_Command_Exec function to be called with a
// meaningful parameter rather than a number.
#define		SDv1	1<<1
#define		SDv2	1<<2
#define		MMCv3	1<<3
#define		SDBlock	1<<4



#define     GO_IDLE_STATE            	0//CMD0
#define     SEND_OP_COND             	1//CMD1
#define     SEND_IF_COND     			2//CMD8
#define     SEND_CSD                 	3//CMD9
#define     SEND_CID                 	4//CMD10
#define     STOP_TRANSMISSION        	5//CMD12
#define     SEND_STATUS              	6//CMD13
#define     SET_BLOCKLEN             	7//CMD16
#define     READ_SINGLE_BLOCK        	8//CMD17
#define     READ_MULTIPLE_BLOCK      	9//CMD18
#define     SET_BLOCK_COUNT       		10//CMD23
#define     WRITE_BLOCK              	11//CMD24
#define     WRITE_MULTIPLE_BLOCK    	12//CMD25
#define     PROGRAM_CSD             	13//CMD27
#define     SET_WRITE_PROT          	14//CMD28
#define     CLR_WRITE_PROT          	15//CMD29
#define     SEND_WRITE_PROT         	16//CMD30
#define     TAG_SECTOR_START        	17//CMD32
#define     TAG_SECTOR_END          	18//CMD33
#define     UNTAG_SECTOR            	19//CMD34
#define     TAG_ERASE_GROUP_START   	20//CMD35
#define     TAG_ERASE_GROUP_END     	21//CMD36
#define     UNTAG_ERASE_GROUP       	22//CMD37
#define     ERASE                   	23//CMD38
#define     LOCK_UNLOCK             	24//CMD42
#define     APP_CMD                 	25//CMD55
#define     READ_OCR                	26//CMD58
#define     CRC_ON_OFF              	27//CMD59
#define     SD_SEND_STATUS          	28//ACMD13
#define     SD_SET_WR_BLK_ERASE_COUNT	29//ACMD23
#define     SD_SEND_OP_COND         	30//ACMD41


// APIs declaration
uint32_t SDCARD_Open(void);
void SDCARD_Close(void);
uint32_t SDCARD_GetVersion(void);
uint32_t MMC_Command_Exec (uint8_t cmd_loc, uint32_t argument,uint8_t *pchar, uint32_t* response);
uint32_t GetLogicSector(void);
uint32_t SDCARD_GetCardSize(uint32_t* pu32TotSecCnt);
void SpiRead(uint32_t addr, uint32_t size, uint8_t* buffer);
void SpiWrite(uint32_t addr, uint32_t size, uint8_t* buffer);
#endif //__DRVSDCARD_H__

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



