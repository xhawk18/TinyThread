/******************************************************************************
 * @file     nano1xx_ebi.c
 * @brief    NANO1xx EBI driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_gpio.h"
#include "nano1xx_ebi.h"

/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_EBI_Driver EBI Driver
  @{
*/



/** @addtogroup NANO1xx_EBI_EXPORTED_FUNCTIONS EBI Exported Functions
  @{
*/

/**
  * @brief  Initialize External Bus Interface (EBI)
  * @param  data_width_is_8_bit: 1: 8-bit data width; 0: 16-bit data wdith.
  * @retval None.
  */
void EBI_Init(int data_width_is_8_bit)
{
	/* Enable EBI clock	 */
    CLK->AHBCLK |= CLK_AHBCLK_EBI_EN;

	/* EBI function enable */
	EBI->EBICON |= EBI_EBICON_ExtEN;

	// Enable EBI_EN and EBI_MCLK_EN
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~0xf) | 0x2;

	// Enable nRD/nWR/ALE/nCS for EBI
	GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0xff00) | 0x2200;
	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0xff000000) | 0x22000000;

	// Enable EBI AD Low-byte, bit 7~0
	GCR->PA_L_MFP = (GCR->PA_L_MFP & ~0xff000000) | 0x22000000;
	GCR->PB_H_MFP = (GCR->PB_H_MFP & ~0xff0000) | 0x220000;
	GCR->PC_H_MFP = (GCR->PC_H_MFP & ~0xff000000) | 0x22000000;
	GCR->PC_L_MFP = (GCR->PC_L_MFP & ~0xff000000) | 0x22000000;

	if (data_width_is_8_bit)
	{	
		EBI->EBICON &= ~EBI_EBICON_ExtBW16;
	}
	else // (sEBIConfig.eBusWidth == E_DRVEBI_BUS_16BIT)
	{
		EBI->EBICON |= EBI_EBICON_ExtBW16;

		// Enable nWRH & nWRL for support Byte-Write in 16bit Data Width Device(SARM)
		GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0xff00) | 0x2200;

		// Enable EBI AD High-byte, bit 15~8
		GCR->PA_L_MFP = (GCR->PA_L_MFP & ~0xfffff0) | 0x222220;
		GCR->PA_H_MFP = (GCR->PA_H_MFP & ~0xfff0000) | 0x2220000;
	}
}



/**
  * @brief  Disable External Bus Interface (EBI)
  * @param  None.
  * @retval None.
  */
void EBI_DeInit(void)
{
	/* EBI disable */
	EBI->EBICON &= ~EBI_EBICON_ExtEN;
	
	/* EBI clock disable */
    CLK->AHBCLK &= ~CLK_AHBCLK_EBI_EN;
}


/**
  * @brief  Set EBI bus timings
  * @param  sEBITiming: The new EBI timing settings.
  * @retval None.
  */
void EBI_SetBusTiming(EBI_TIMING_T sEBITiming)
{	
	uint32_t   tmp;
	
	tmp = ((sEBITiming.eMCLKDIV & 0x7) << 8) | ((sEBITiming.u8ExttALE & 0x7) << 16);
	EBI->EBICON = (EBI->EBICON & 0xfff8f8ff) | tmp;
	
	EBI->EXTIME = (sEBITiming.u8ExttACC & 0x1F) |
				  ((sEBITiming.u8ExttAHD & 0x7) << 8) |
				  ((sEBITiming.u8ExtIW2X & 0xf) << 12) |
				  ((sEBITiming.u8ExtIR2W & 0xf) << 16) |
				  ((sEBITiming.u8ExtIR2R & 0xf) << 24);
}


/**
  * @brief  Get current EBI bus timing settings
  * @param  psEBITiming: Pointer of current EBI timing settings.
  * @retval None.
  */
void EBI_GetBusTiming(EBI_TIMING_T *psEBITiming)
{
	psEBITiming->eMCLKDIV  = (EBI->EBICON & EBI_EBICON_MCLKDIV_MASK) >> 8;
	psEBITiming->u8ExttALE = (EBI->EBICON & EBI_EBICON_ExtALE_MASK) >> 16;
	
	psEBITiming->u8ExttACC = (EBI->EXTIME & EBI_EXTIME_ExttAAC_MASK);
	psEBITiming->u8ExttAHD = (EBI->EXTIME & EBI_EXTIME_ExttAHD_MASK) >> 8;
	psEBITiming->u8ExtIW2X = (EBI->EXTIME & EBI_EXTIME_ExtlW2X_MASK) >> 12;
	psEBITiming->u8ExtIR2W = (EBI->EXTIME & EBI_EXTIME_ExtlR2W_MASK) >> 16;
	psEBITiming->u8ExtIR2R = (EBI->EXTIME & EBI_EXTIME_ExtlR2R_MASK) >> 24;
}


/*@}*/ /* end of group NANO1xx_EBI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_EBI_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */																								

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
