/******************************************************************************
 * @file     Smpl_Main.c
 * @brief    Nano1xx NUTINY-EVB_NANO130 LCD Demo code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_lcd.h"
#include "nano1xx_sys.h"
#include "nano1xx_rtc.h"

#ifdef __DEBUG_MSG
#define DEBUG_MSG 	printf 
#else
#define DEBUG_MSG(...)
#endif


/* External functions */
void textticker(char *string, uint32_t delayus);
void showTime(uint32_t hour, uint32_t minute);
void planNextRTCInterrupt(S_DRVRTC_TIME_DATA_T *sCurTime);


/**
  * @brief  Main routine. 
  * @param  None.
  * @return None.
  */
int32_t main(void)							   
{
	S_LCD_INIT lcdinit;
	S_DRVRTC_TIME_DATA_T sCurTime;

	DEBUG_MSG("\nNANO130 NuTiny EVB Demo Program V1.0.0\n");
	DEBUG_MSG("[LCD Demo]\n"); 

	/* Unlock protected registers */
	UNLOCKREG();
	
	/* Init RTC */
	RTC_Init();	
	DEBUG_MSG("RTC Init. complete!\n");

	/* Init LCD */
	SYS_SelectIPClockSource_1(CLK_CLKSEL1_LCD_MASK, CLK_CLKSEL1_LCD_LXT); /* Select LCD Clock Source From 32KHz */
	
	/* Select LCD COMs, SEGs, V1 ~ V3, DH1, DH2 */
	MFP_LCD_TYPEA();		
	
	/* Do LCD Initializaton */
	lcdinit.cpump_enable = true;
	lcdinit.internal_bias = false;
	lcdinit.cpump_freqdiv = LCD_CPUMP_DIV1;
	lcdinit.cpump_voltage = LCD_CPVOl_3V;
	lcdinit.bias = LCD_BIAS_THIRD;
	lcdinit.mux = LCD_MUX_ONE_FOURTH;
	lcdinit.freqdiv = LCD_FREQ_DIV64;
	LCD_Init(&lcdinit);

	DEBUG_MSG("LCD Init. complete!\n");

	/* Start displaying on LCD */
	LCD_EnableSegment(3, 22);
	SYS_Delay(335000);

	textticker("*** NUVOTON NANO130 ***", 335000);
	SYS_Delay(335000);

	LCD_Write("NUVOTON");
	LCD_BlinkFrequency(250);
	LCD_EnableBlink();
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	SYS_Delay(335000);
	LCD_DisableBlink();
	
	/* Read curent RTC time */		
	RTC_Read(&sCurTime);
	DEBUG_MSG("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
	
	/* Display RTC time */			
	showTime(sCurTime.u32cHour, sCurTime.u32cMinute);
	
	/* Enable RTC alarm for 1 minute to update RTC time */
	planNextRTCInterrupt(&sCurTime);


	/* Enable RTC timer tick for 1 second to display ":" */
	RTC_SetTickMode(DRVRTC_TICK_1_SEC);
	RTC_EnableInt(RTC_RIER_TIER);		

	/* forever loop */
	while (1);

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
 	MFP_UART0_TO_PORTA();				   /* UART0 TX/RX to PA14/PA15*/
 	CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
	CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
	CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

 
    /* Set UART to 115200,n,8,1,none */
    UART0->BAUD = 0x67;             /* Baud Rate:115200 for 12MHz */
    UART0->TLCTL = 0x03;            /* Word len is 8 bits */          
	
	printf("[%s] line %d : wrong parameters.\r\n", file, line);

	/* Infinite loop */
	while(1) ;

}
#endif


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/


	 
