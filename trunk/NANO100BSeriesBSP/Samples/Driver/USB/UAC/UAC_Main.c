/******************************************************************************
 * @file     UAC_Main.c
 * @brief    Nano1xx USB Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_usb.h"

extern int32_t UAC_MainProcess(void);

/*---------------------------------------------------------------------------------------------------------*/
/* Main function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	SYS_SetClockDivider_0(CLK_CLKDIV0_USB_MASK, USB_CLK_DIVIDER(1));

#ifdef _NO_USE_HXT_
	/* if not use HXT, PLL source from HIRC, HIRC need to enable auto-trim */
	/* 1. enable LXT to do auto-trim */
	UNLOCKREG();
	CLK->PWRCTL |= CLK_PWRCTL_LXT_EN;	   
	while (!(CLK->CLKSTATUS & CLK_CLKSTATUS_LXT_STB));
	LOCKREG();

	/* 2. trim HIRC to 12MHz */
	GCR->IRCTRIMCTL = (GCR_IRCTRIMCTL_TRIM_12M | GCR_IRCTRIMCTL_LOOP_32CLK | GCR_IRCTRIMCTL_RETRY_512);

	/* enable HIRC-trim interrupt */
	NVIC_EnableIRQ(HIRC_IRQn);
	GCR->IRCTRIMIER = (GCR_IRCTRIMIEN_FAIL_EN | GCR_IRCTRIMIEN_32KERR_EN);
#endif

    printf("NuMicro USB UAC\n");

    UAC_MainProcess();
}

#ifdef USE_ASSERT
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     assert_error                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */   
/*               file           -[in] source file name                                                     */
/*               line           -[in] line number                                                          */
/* Returns:                                                                                                */
/*               None.                                                                                     */
/* Description:                                                                                            */
/*               The function prints the source file name and line number where the assert_param() error   */
/*               occurs, and then stops in an infinite loop.                                               */
/*               User can add his own codes here if necessary.                                                  */
/*---------------------------------------------------------------------------------------------------------*/

void assert_error(uint8_t * file, uint32_t line)
{
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

