/******************************************************************************
 * @file     nano1xx_fmc.h
 * @brief    NANO1xx FMC driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef _NANO1XX_FMC_H_
#define _NANO1XX_FMC_H_

#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_FMC_Driver FMC Driver
  @{
*/
	
/** @addtogroup NANO1xx_FMC_EXPORTED_CONSTANTS FMC Exported Constants
  @{
*/	
							   
/*---------------------------------------------------------------------------------------------------------*/
/*  Define Error Code									                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_FMC_OK  					0
#define E_FMC_ISP_FAIL   			(-1)
#define E_FMC_INVALID_ADDR			(-2)
#define E_FMC_INVALID_PARAM			(-3)

/*---------------------------------------------------------------------------------------------------------*/
/*  Flash Boot Selector 								                                                   */
/*---------------------------------------------------------------------------------------------------------*/
#define E_FMC_APROM					0
#define E_FMC_LDROM					1


/*---------------------------------------------------------------------------------------------------------*/
/* Define Flash address                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define APROM_BASE			0x00000000
#define LDROM_BASE			0x00100000
#define LDROM_SIZE			0x10000
#define CONFIG_BASE			0x00300000
#define CONFIG0         	0x00300000
#define CONFIG1         	0x00300004 
#define PAGE_SIZE   		512

/*@}*/ /* end of group NANO1xx_FMC_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_FMC_EXPORTED_FUNCTIONS FMC Exported Functions
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/* Inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
  * @brief  Enable ISP function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_Init(void)
{
	CLK->AHBCLK |= CLK_AHBCLK_ISP_EN;
	FMC->ISPCON |= FMC_ISPCON_ISPEN;
}


/**
  * @brief  Disable ISP function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_DeInit(void)
{
	FMC->ISPCON &= ~FMC_ISPCON_ISPEN;
}


/**
  * @brief  Set "Boot Select" for next software reset.
  * @param  boot: E_FMC_APROM or E_FMC_LDROM
  * @retval None.
  */
static __INLINE void FMC_BootSelect(int boot)
{
	if (boot == E_FMC_APROM)
		FMC->ISPCON &= ~FMC_ISPCON_BS;
	else
		FMC->ISPCON |= FMC_ISPCON_BS;
}


/**
  * @brief  Get the current boot select setting.
  * @param  None.
  * @retval E_FMC_APROM or E_FMC_LDROM
  */
static __INLINE int FMC_GetBootSelect(void)
{
	if (FMC->ISPCON & FMC_ISPCON_BS)
		return E_FMC_LDROM;
	else
		return E_FMC_APROM;
}


/**
  * @brief  Enable APROM update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_EnableAPUpdate(void)
{	
	FMC->ISPCON |= FMC_ISPCON_APUEN;
}


/**
  * @brief  Disable APROM update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_DisableAPUpdate(void)
{	
	FMC->ISPCON &= ~FMC_ISPCON_APUEN;
}


/**
  * @brief  Enable LDROM update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_EnableLDUpdate(void)
{	
	FMC->ISPCON |= FMC_ISPCON_LDUEN;
}


/**
  * @brief  Disable LDROM update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_DisableLDUpdate(void)
{	
	FMC->ISPCON &= ~FMC_ISPCON_LDUEN;
}


/**
  * @brief  Enable User Config update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_EnableConfigUpdate()
{	
	FMC->ISPCON |= FMC_ISPCON_CFGUEN;
}


/**
  * @brief  Disable User Config update function.
  * @param  None.
  * @retval None.
  */
static __INLINE void FMC_DisableConfigUpdate()
{	
	FMC->ISPCON &= ~FMC_ISPCON_CFGUEN;
}


/**
  * @brief  Read Data Flash base address.
  * @param  None.
  * @retval The base address of Data Flash.
  *         
  */
static __INLINE uint32_t FMC_ReadDataFlashBaseAddr(void)
{	
	return FMC->DFBADR;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Define FMC functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t  FMC_Write(uint32_t u32addr, uint32_t u32data);
int32_t  FMC_Read(uint32_t u32addr, uint32_t * u32data);
int32_t  FMC_Erase(uint32_t u32addr);
int32_t  FMC_SetVectorPage(uint32_t u32addr);
int32_t  FMC_ReadCID(uint32_t * u32data);
int32_t  FMC_ReadDID(uint32_t * u32data);
int32_t  FMC_ReadPID(uint32_t * u32data);
int32_t  FMC_ReadUID(int index, uint32_t * u32data);
int32_t  FMC_WriteConfig(uint32_t u32data0, uint32_t u32data1);

/*@}*/ /* end of group NANO1xx_FMC_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_FMC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
