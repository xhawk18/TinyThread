/******************************************************************************
 * @file     EBI_LCM_config.c
 * @brief    LCM init function
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
  
#include <stdlib.h>
#include <stdio.h>
#include "Nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_ebi.h"


void EBI_LCM_Initial(void)
{
	uint32_t u32SysHCLKFreq = SystemCoreClock;
	EBI_TIMING_T	sEBITiming;

	UNLOCKREG();
		
	// Enable EBI function with 8-bit data width
	EBI_Init(1);

	sEBITiming.eMCLKDIV = E_EBI_MCLKDIV_2;
	printf("MCLK of EBI is %d KHz.\n", u32SysHCLKFreq/2);

	sEBITiming.u8ExttALE = 0;
	sEBITiming.u8ExtIR2R = 0;
	sEBITiming.u8ExtIR2W = 0;
	sEBITiming.u8ExtIW2X = 0;
	sEBITiming.u8ExttAHD = 0;
	sEBITiming.u8ExttACC = 0;
	EBI_SetBusTiming(sEBITiming);


	// configure EBI ALE pin to RS for LCD driver control
	GCR->PB_L_MFP &= ~0x0F000000;
	GPIOB->PMD |= 0x00001000;		// output mode
	GPIOB->DOUT |= (1<<6);			// defalut HIGH

	// configure EBI CS pin to CS for manual driver control
	GCR->PB_L_MFP &= ~0xF0000000;
	GPIOB->PMD |= 0x00004000;		// output mode
	GPIOB->DOUT |= (1<<7);			// defalut HIGH

	// configure PD[8] pin to RESET for manual driver control
	GCR->PD_H_MFP &= ~0x0000000F;
	GPIOD->PMD |= 0x00010000;		// output mode
	GPIOD->DOUT |= (1<<8);			// defalut HIGH
	
}



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/



