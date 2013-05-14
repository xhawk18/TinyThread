/******************************************************************************
 * @file     nano1xx_fmc.c
 * @brief    NANO1xx FMC driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "nano1xx_fmc.h"

/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_FMC_Driver FMC Driver
  @{
*/


/** @addtogroup NANO1xx_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/


/**
  * @brief  Program a word to specified flash address. Note that the target address
  *         should never be programmed since last page erase.
  * @param  u32addr: Flash word address. Must be a word aligned address.
  * @param  u32data: The 32-bit data to be programmed into flash.
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_Write(uint32_t u32addr, uint32_t u32data)
{
	FMC->ISPCMD = FMC_ISPCMD_PROGRAM;
    FMC->ISPADR	= u32addr;
	FMC->ISPDAT	= u32data;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
	
	return E_FMC_OK;
}


/**
  * @brief  Read a word from specified flash address. 
  * @param  u32addr: Flash word address. Must be a word aligned address.
  * @param  u32data: A 32-bit buffer to store the word read from flash. 
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_Read(uint32_t u32addr, uint32_t * u32data)
{ 
	FMC->ISPCMD = FMC_ISPCMD_READ;
    FMC->ISPADR	= u32addr;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
	
	*u32data = FMC->ISPDAT;
	return E_FMC_OK;
}


/**
  * @brief  Erase a page. The page size is 512 bytes. 
  * @param  u32addr: Flash page address. Must be a 512-byte aligned address.
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_Erase(uint32_t u32addr)
{
	FMC->ISPCMD = FMC_ISPCMD_PAGE_ERASE;
    FMC->ISPADR	= u32addr;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
	
	return E_FMC_OK;
}


/**
  * @brief  Set vector page re-mapping. Mapping APROM page 0 or LDROM page 0
  *         to memory address 0x0 ~ 0x1FF. 
  * @param  u32addr: APROM (0x0) or LDROM (0x100000) base address.
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  * @retval E_FMC_INVALID_ADDR: Invalid mapping address.
  */
int32_t FMC_SetVectorPage(uint32_t u32addr)
{
	if ((u32addr != 0) && (u32addr != 0x100000))
		return E_FMC_INVALID_ADDR;
	
	FMC->ISPCMD = FMC_ISPCMD_VECMAP;
    FMC->ISPADR	= u32addr;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}

	return E_FMC_OK;
}


/**
  * @brief  Read company ID. It should always be 0xDA.
  * @param  u32data: A word buffer to store the company ID. 
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_ReadCID(uint32_t * u32data)
{
	FMC->ISPCMD = FMC_ISPCMD_READ_CID;
	FMC->ISPADR	= 0;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
    
	*u32data = FMC->ISPDAT; 
    return E_FMC_OK;
}


/**
  * @brief  Read device ID.
  * @param  u32data: A word buffer to store the device ID. 
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_ReadDID(uint32_t * u32data)
{
	FMC->ISPCMD = FMC_ISPCMD_READ_DID;
	FMC->ISPADR	= 0;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
    
	*u32data = FMC->ISPDAT; 
    return E_FMC_OK;
}


/**
  * @brief  Read product ID.
  * @param  u32data: A word buffer to store the product ID. 
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  */
int32_t FMC_ReadPID(uint32_t * u32data)
{
	FMC->ISPCMD = FMC_ISPCMD_READ_DID;
	FMC->ISPADR	= 0x04;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
    
	*u32data = FMC->ISPDAT; 
    return E_FMC_OK;
}


/**
  * @brief  Read unique ID. There's three unique ID.
  * @param  index: Unique ID index. Index should be 0, 1, or 2.
  * @param  u32data: A word buffer to store the unique ID. 
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  * @retval E_FMC_INVALID_PARAM: Invalid unique ID index.
  */
int32_t FMC_ReadUID(int index, uint32_t * u32data)
{
	if ((index < 0) || (index > 2))
		return E_FMC_INVALID_PARAM;
	
	FMC->ISPCMD = FMC_ISPCMD_READ_UID;
	FMC->ISPADR	= 0x04 * index;
	FMC->ISPTRG = FMC_ISPTRG_ISPGO;	
	
	while (FMC->ISPTRG & FMC_ISPTRG_ISPGO) ;

	if (FMC->ISPCON & FMC_ISPSTA_ISPFF)
	{
		FMC->ISPCON = FMC_ISPSTA_ISPFF;
		return E_FMC_ISP_FAIL;
	}
    
	*u32data = FMC->ISPDAT; 
    return E_FMC_OK;
}


/**
  * @brief  Write Config0 and Config1
  * @param  u32data0: Data word to be written to User Config 0.
  * @param  u32data1: Data word to be written to User Config 1.
  * @retval E_FMC_OK: Success
  * @retval E_FMC_ISP_FAIL: ISP command failed.
  * @retval E_FMC_INVALID_PARAM: Invalid unique ID index.
  */
int32_t FMC_WriteConfig(uint32_t u32data0, uint32_t u32data1)
{		
	if (FMC_Erase(CONFIG0))
		return E_FMC_ISP_FAIL;
	
	if (FMC_Write(CONFIG0, u32data0))
		return E_FMC_ISP_FAIL;

	return FMC_Write(CONFIG1, u32data1);
}


/*@}*/ /* end of group NANO1xx_FMC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_FMC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

