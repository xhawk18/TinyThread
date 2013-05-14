/******************************************************************************
 * @file     VCOM_IO_Main.c
 * @brief    Nano1xx USB Sample code
 * @version  1.0.1
 * @date     08, October, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "nano1xx_usb.h"
#include "nano1xx_lcd.h"

extern void VCOM_MainProcess(void);


/**
  * @brief  main function
  * @param  none
  * @return None
  */
int32_t main (void)
{
	S_LCD_INIT lcdinit;
	char start_str[5];

	SYS_SetChipClockSrc(CLK_PWRCTL_HXT_EN, 1);
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_HXT_STB));
	SYS_SetChipClockSrc(CLK_PWRCTL_LXT_EN, 1);
	while(SYS_CheckChipClockSrc(CLK_CLKSTATUS_LXT_STB));

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

	/* Select LCD Clock Source From 32KHz */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT);
	MFP_LCD_TYPEA();

	/* LCD Initialize */
	lcdinit.cpump_enable = true;
	lcdinit.internal_bias = false;
	lcdinit.cpump_freqdiv = LCD_CPUMP_DIV1;
	lcdinit.cpump_voltage = LCD_CPVOl_3V;
	lcdinit.bias = LCD_BIAS_THIRD;
	lcdinit.mux = LCD_MUX_ONE_FOURTH;
	lcdinit.freqdiv = LCD_FREQ_DIV64;
	LCD_Init(&lcdinit);

	/* disable the segment pin */
	LCD_SegmentPinEnable(4, false);
	LCD_SegmentPinEnable(5, false);
	LCD_SegmentPinEnable(6, false);
	LCD_SegmentPinEnable(7, false);
	LCD_SegmentPinEnable(12, false);
	LCD_SegmentPinEnable(13, false);
	LCD_SegmentPinEnable(20, false);
	LCD_SegmentPinEnable(21, false);
	LCD_SegmentPinEnable(26, false);
	LCD_SegmentPinEnable(28, false);
	LCD_SegmentPinEnable(29, false);
	LCD_SegmentPinEnable(30, false);
	LCD_SegmentPinEnable(31, false);
	LCD_SegmentPinEnable(34, false);
	LCD_SegmentPinEnable(35, false);
	LCD_SegmentPinEnable(39, false);

	start_str[0] = 'N';
	start_str[1] = 'A';
	start_str[2] = 'N';
	start_str[3] = 'O';
	LCD_Write(&start_str[0]);

    /* Execute VCOM process */
	VCOM_MainProcess();
}

#ifdef USE_ASSERT
/**
  * @brief  The function prints the source file name and line number where the assert_param() error
  *         occurs, and then stops in an infinite loop. User can add his own codes here if necessary.
  * @param[in] file Source file name
  * @param[in] line Line number
  * @return None
  */

void assert_error(uint8_t * file, uint32_t line)
{
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

