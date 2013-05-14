/******************************************************************************
 * @file     EBI_NOR.c
 * @brief    Nano1xx EBI Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_ebi.h"


void NOR_W39L010(void);
void NOR_Reset_W39L010(void);
uint8_t NOR_Device_ID_W39L010(void);
uint8_t NOR_Erase_W39L010(uint8_t u8IsNeedCompare);
uint8_t NOR_CheckCMDComplete(uint32_t u32DestAddr, uint8_t u8Data);
uint8_t NOR_ProgramByte_W39L010(uint32_t u32DestAddr, uint8_t u8Data);
uint8_t ProgramDataTest(void);
uint8_t ContinueDataTest(void);


/**
  * @brief  Do NOP loop delay
  * @param  u32Cnt: NOP Loop count
  * @retval None
  */
void DelayNOP(uint32_t u32Cnt)
{
	volatile uint32_t u32LoopCnt = u32Cnt;
	while (u32LoopCnt--)
	{
		__NOP();
	}
}


/**
  * @brief  External W39L010 NOR flash on EBI bus access test
  *         Data Width : 8bits, Data Size : 128K
  * @param  None
  * @retval None
  */
void NOR_W39L010(void)
{
	// Enable EBI function with 8-bit data width
 	EBI_Init(1);

	// Reset NOR Flash
	NOR_Reset_W39L010();
	
	// Get Device ID
	if (NOR_Device_ID_W39L010() == TRUE)
	{
		printf("NOR W39L010 initial OK !\n");
	}else
	{
		printf("NOR W39L010 initial fail !\n\n");
		return ;
	}
	
	printf("Erase Test ... \n");
	NOR_Erase_W39L010(TRUE);
	printf("\n");

	printf("Program Data Test ... \n");
	ProgramDataTest();
}


/**
  * @brief  Reset W39L010 NOR flash
  * @param  None
  * @retval None
  */
void NOR_Reset_W39L010(void)
{
	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0, 0xF0);
	DelayNOP(0x1000);
}


/**
  * @brief  Check W39L010 NOR flash ID
  * @param  None
  * @retval TRUE: ID match
  *         FALSE: ID mismatch
  */
uint8_t NOR_Device_ID_W39L010()
{
	uint8_t u8ManuFactureID, u8DeviceID;

	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0x5555, 0x90);
	DelayNOP(0x1000);
	
	u8ManuFactureID = EBI_READ_DATA8(0x0);
	u8DeviceID 		= EBI_READ_DATA8(0x1);
	printf("   >> W39L010 ManufactureID = 0x%X, DeviceID = 0x%X\n", u8ManuFactureID, u8DeviceID);
	DelayNOP(0x1000);
	if ((u8ManuFactureID != 0xDA) ||(u8DeviceID != 0x31))
		return FALSE;
		
	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0x5555, 0xF0);
	DelayNOP(0x1000);
	
	return TRUE;
}


/**
  * @brief  Erase whole W39L010 NOR flash
  * @param  u8IsNeedCompare: 1: After erase, verify flash content is erased or not
  *                          0: Erase onlt, do not verify.
  * @retval TRUE: Success
  *         FALSE: Erase or verify failed.
  */
uint8_t NOR_Erase_W39L010(uint8_t u8IsNeedCompare)
{
	uint8_t u8Status = TRUE;
	
	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0x5555, 0x80);

	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0x5555, 0x10);
	
	u8Status = NOR_CheckCMDComplete(0x0, 0x0);
	if (u8Status == FALSE)
	{
		printf("   >> Chip Erase ... TIME OUT !!!\n");
		return u8Status;
	}
	DelayNOP(0x10000);
	
	if ( u8IsNeedCompare )
	{
		/* Compare data ...... */
		uint8_t u8DataIn, u8DataOut;
		uint32_t u32NORAddr;
		u8DataIn = 0xFF;
		for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
		{
			u8DataOut = EBI_READ_DATA8(u32NORAddr);
			if (u8DataOut != u8DataIn)
			{
				printf("Read [0x%05X]:[0x%02X] FAIL !!! (Got [0x%02X])\n", u32NORAddr, u8DataIn, u8DataOut);
				printf("   >> Chip Erase FAIL !!!\n\n");
				return FALSE;
			}
/*
			else
			{
				// for test only 
				if ((u32NORAddr%32) == 0)
					printf("Read [0x%05X]:[0x%02X] !!!\r", u32NORAddr, u8DataOut);
			}
*/			
		}
		printf("   >> Chip Erase OK !!!          \n\n");
	}
		
	return u8Status;
}


/**
  * @brief  Check previous command finished or not
  * @param  u32DestAddr: Check address
  *         u8Data: Check data
  * @retval TRUE: Success
  *         FALSE: Last command may fail.
  */
uint8_t NOR_CheckCMDComplete(uint32_t u32DestAddr, uint8_t u8Data)
{
#if 0
	/* Check Toggle Ready */
    uint8_t u8PreData;
    uint8_t u8CurData;
    uint32_t u32TimeOut = 0;

    u8PreData = EBI_READ_DATA8(u32DestAddr);
    u8PreData = u8PreData & (1<<6);   		// read DQ6
    while (u32TimeOut < EBI_TIMEOUT_COUNT)	 
    {
        u8CurData = EBI_READ_DATA8(u32DestAddr);
        u8CurData = u8CurData & (1<<6); 	// read DQ6 again
        if (u8PreData == u8CurData)
        {
			return TRUE;
        }
		u8PreData = u8CurData;
        u32TimeOut++;
    }
    return FALSE;
#else
	/* Check Data Polling */
	uint8_t u8CurData;
    uint32_t u32TimeOut = 0;

    u8Data = u8Data & (1<<7); 	// read D7
    while (u32TimeOut < EBI_TIMEOUT_COUNT)	
    {
		u8CurData = EBI_READ_DATA8(u32DestAddr);
		u8CurData = u8CurData & (1<<7);	// read DQ7
		if (u8Data == u8CurData)
		{
			return TRUE;
		}
		u32TimeOut++;
    }
    return FALSE;
#endif
}


/**
  * @brief  Program one byte to W39L010 NOR flash 
  * @param  u32DestAddr: Program target address
  *         u8Data: Program data
  * @retval TRUE: Success
  *         FALSE: Command failed
  */
uint8_t NOR_ProgramByte_W39L010(uint32_t u32DestAddr, uint8_t u8Data)
{
	EBI_WRITE_DATA8(0x5555, 0xAA);
	EBI_WRITE_DATA8(0x2AAA, 0x55);
	EBI_WRITE_DATA8(0x5555, 0xA0);
	
	EBI_WRITE_DATA8(u32DestAddr, u8Data);
	
 	return NOR_CheckCMDComplete(u32DestAddr, u8Data);
}


/**
  * @brief  Execute program and compare test on whole W39L010 NOR flash 
  * @param  None
  * @retval TRUE:  Test passed
  *         FALSE: Test failed
  */
uint8_t ProgramDataTest(void)
{
	uint8_t u8DataIn, u8DataOut, u8WriteOnce = TRUE;
	uint32_t u32NORAddr;
	uint8_t	u8Data = 0x00;	

	u8DataIn = u8Data;
	if (u8DataIn == 0x00)
		u8WriteOnce = FALSE;
		
	while (1)
	{		
		/* Erase flash first */
		NOR_Erase_W39L010(FALSE);
		DelayNOP(0x10000);
		
		/* Program flash and compare data */
		printf("  >> Program Flash Test ... \n");
		for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
		{
			if (NOR_ProgramByte_W39L010(u32NORAddr, u8DataIn) == FALSE)
			{
				printf("Program [0x%05X]:[0x%02X] FAIL !!!\n", u32NORAddr, u8DataIn);
				return FALSE;
			}
/*			else
			{
				// for test only
				if ((u32NORAddr%32) == 0)
					printf("Program [0x%05X]:[0x%02X] !!!\r", u32NORAddr, u8DataIn);
			}
*/			
		}
		for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
		{
			u8DataOut = EBI_READ_DATA8(u32NORAddr);
			if (u8DataOut != u8DataIn)
			{
				printf("Read [0x%05X]:[0x%02X] FAIL !!! (Got [0x%02X])\n", u32NORAddr, u8DataIn, u8DataOut);
				printf("Program flash FAIL !!!          \n");
				return FALSE;
			}
/*			else
			{
				// for test only 
				if ((u32NORAddr%32) == 0)
					printf("Read [0x%05X]:[0x%02X] !!!     \r", u32NORAddr, u8DataOut);
			}
*/			
		}
		printf("  >> Program flash [0x%02X] OK !!!          \n\n", u8DataIn);
		
		if ( u8WriteOnce )
			break;
			
		if (u8DataIn == 0x00)
			u8DataIn = 0xFF;
		else if (u8DataIn == 0xFF)
			u8DataIn = 0x55;
		else if (u8DataIn == 0x55)
			u8DataIn = 0xAA;
		else if (u8DataIn ==  0xAA)
			u8DataIn = 0x5A;
		else if (u8DataIn == 0x5A)
			u8DataIn = 0xA5;
		else if (u8DataIn == 0xA5)
			u8DataIn = 0x96;
		else if (u8DataIn == 0x96)
			u8DataIn = 0x69;
		else if (u8DataIn == 0x69)
		{
			u8DataIn = 0xF0;
			ContinueDataTest();
		}
		else 
			break;	
	}

	return TRUE;
}


/**
  * @brief  Execute program and compare test on whole W39L010 NOR flash 
  * @param  None
  * @retval TRUE:  Test passed
  *         FALSE: Test failed
  */
uint8_t ContinueDataTest(void)
{
	uint8_t u8DataIn, u8DataOut;
	uint32_t u32NORAddr;

	/* Erase flash first */
	NOR_Erase_W39L010(FALSE);
	DelayNOP(0x10000);
	
	/* Program flash and compare data */
	printf("  >> Program Flash Test ... \n");
	for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
	{
		u8DataIn = (uint8_t)(((u32NORAddr&0xFF0000)>>16) + ((u32NORAddr&0xFF00)>>8) + (u32NORAddr&0xFF));
		if (NOR_ProgramByte_W39L010(u32NORAddr, u8DataIn) == FALSE)
		{
			printf("Program [0x%05X]:[0x%02X] FAIL !!!\n", u32NORAddr, u8DataIn);
			return FALSE;
		}

	}
	for (u32NORAddr=0; u32NORAddr<EBI_MAX_SIZE; u32NORAddr++)
	{
		u8DataIn = (uint8_t)(((u32NORAddr&0xFF0000)>>16) + ((u32NORAddr&0xFF00)>>8) + (u32NORAddr&0xFF));
		u8DataOut = EBI_READ_DATA8(u32NORAddr);
		if (u8DataOut != u8DataIn)
		{
			printf("Read [0x%05X]:[0x%02X] FAIL !!! (Got [0x%02X])\n", u32NORAddr, u8DataIn, u8DataOut);
			printf("Program flash FAIL !!!          \n");
			return FALSE;
		}

	}
	printf("  >> Program flash [0x%02X] OK !!!          \n\n", u8DataIn);
		
	return TRUE;
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
