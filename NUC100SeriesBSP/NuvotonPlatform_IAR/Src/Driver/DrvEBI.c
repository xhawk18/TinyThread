/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvGPIO.h"
#include "DrvEBI.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvEBI_Open                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  sEBIConfig - [in]																	   */
/*						Input the general EBI Control Register settings, the DRVEBI_CONFIG_T structure.    */
/*						It's included eDataWidth, eAddrWidth, u32BaseAddress and u32Size.				   */   
/* Return:                                                                                                 */
/*					E_SUCCESS								Operation successful   			               */
/*                  E_DRVEBI_ERR_ARGUMENT					Invalid argument                               */
/* Description:                                                                                            */
/*                  Enable EBI function and configure the relative EBI Control Registers.                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvEBI_Open(DRVEBI_CONFIG_T sEBIConfig)
{
	/* EBI clock enable */
	SYSCLK->AHBCLK.EBI_EN = 1;

	/* EBI function enable */
	EBI->EBICON.ExtEN = 1;
	
	if (sEBIConfig.eAddrWidth == E_DRVEBI_ADDR_8BIT)
	{	
		DrvGPIO_InitFunction(E_FUNC_EBI_8B);
	}else
	if (sEBIConfig.eAddrWidth == E_DRVEBI_ADDR_16BIT)
	{
		DrvGPIO_InitFunction(E_FUNC_EBI_16B);
	}else
	{
        return E_DRVEBI_ERR_ARGUMENT;
	}
    EBI->EBICON.ExtBW16 = sEBIConfig.eDataWidth;
    		
    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvEBI_Close                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Return:                                                                                                 */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  Disable EBI function and release the relative pins for GPIO used.                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvEBI_Close(void)
{
	/* EBI disable */
	EBI->EBICON.ExtEN = 0;
	
	/* EBI clock disable */
	SYSCLK->AHBCLK.EBI_EN = 0;
	
	/* Configure EBI pins to GPIO pins here ...... */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvEBI_SetBusTiming                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  sEBITiming - [in]				   													   */
/*						It is meaning the DRVEBI_TIMING_T structure and included eMCLKDIV, 	               */
/*						u8ExttALE, u8ExtIR2R, u8ExtIW2X, u8ExttAHD and u8ExttACC.	                       */
/* Return:                                                                                                 */
/*                  None                                                                                   */
/* Description:                                                                                            */
/*                  Configure the relative EBI bus timing.                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvEBI_SetBusTiming(DRVEBI_TIMING_T sEBITiming)
{	
	EBI->EBICON.MCLKDIV = sEBITiming.eMCLKDIV;
	EBI->EBICON.ExttALE = sEBITiming.u8ExttALE;
	
	EBI->EXTIME.ExttACC = sEBITiming.u8ExttACC;
	EBI->EXTIME.ExttAHD = sEBITiming.u8ExttAHD;
	EBI->EXTIME.ExtIW2X = sEBITiming.u8ExtIW2X;
	EBI->EXTIME.ExtIR2R = sEBITiming.u8ExtIR2R;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvEBI_GetBusTiming                                                                    */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*             		psEBITiming - [out] 																   */
/*						It is meaning the DRVEBI_TIMING_T structure and included eMCLKDIV, 	               */
/*						u8ExttALE, u8ExtIR2R, u8ExtIW2X, u8ExttAHD and u8ExttACC.	                       */
/* Return:                                                                                                 */
/*                  Data buffer pointer that stored the EBI bus timing settings		                       */
/* Description:                                                                                            */
/*                  Get the current bus timing of the EBI.	                                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvEBI_GetBusTiming(DRVEBI_TIMING_T *psEBITiming)
{
	psEBITiming->eMCLKDIV  = (E_DRVEBI_MCLKDIV)EBI->EBICON.MCLKDIV;
	psEBITiming->u8ExttALE = EBI->EBICON.ExttALE;
	
	psEBITiming->u8ExttACC = EBI->EXTIME.ExttACC;
	psEBITiming->u8ExttAHD = EBI->EXTIME.ExttAHD;
	psEBITiming->u8ExtIW2X = EBI->EXTIME.ExtIW2X;
	psEBITiming->u8ExtIR2R = EBI->EXTIME.ExtIR2R;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:        DrvEBI_GetVersion                                                                      */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*                  None                                                                                   */
/* Returns:                                                                                                */
/*                  DRVEBI_VERSION_NU						Version number of EBI driver                   */
/* Description:                                                                                            */
/*                  Get the version number of EBI driver.                                                  */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvEBI_GetVersion(void)
{
	return DRVEBI_VERSION_NUM;
}
