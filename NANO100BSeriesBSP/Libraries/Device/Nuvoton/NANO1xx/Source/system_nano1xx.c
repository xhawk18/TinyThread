/******************************************************************************
 * @file     system_nano1xx.c
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Source File for
 *           NANO1XX devices
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdint.h>
#include "nano1xx.h"


/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __SYSTEM_CLOCK    (1*__IRC12M)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/**
  * @brief  Initial UART0 as debug console. 
  * @param  None
  * @return None
  */
void SysInit_DebugConsole()
{

  /* Enable UART0 for debug console */
  GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
  //MFP_UART0_TO_PORTA();
  CLK->APBCLK |= CLK_APBCLK_UART0_EN;    /* Enable UART0 clock */
  CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
  CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */

  UART0->BAUD = 0x67;              /* Baud Rate:115200  OSC:12MHz */
  UART0->TLCTL = 0x03;             /* Character len is 8 bits */


}

/**
  * @brief  Set up multi-function pin selection. 
  * @param  None
  * @return None
  */
void SysInit_PinFunc(void)
{
	/* TODO : Set up pin selection */
}


/**
  * @brief  Calculate current PLL clock frquency. 
  * @param  None.
  * @return PLL clock frquency. The clock UNIT is in Hz.
  */
uint32_t SysGet_PLLClockFreq(void)
{
	uint32_t u32Freq =0, u32PLLSrc;
    uint32_t u32NO, u32NR, u32IN_DV, u32PllReg;

	u32PllReg = CLK->PLLCTL;

	if (u32PllReg & CLK_PLLCTL_PD)  
		return 0;	 /* PLL is in power down mode */
	
	if (u32PllReg & CLK_PLLCTL_PLLSRC_HIRC)
		u32PLLSrc = __IRC12M;
	else
		u32PLLSrc = __XTAL;

    u32NO = (u32PllReg & CLK_PLLCTL_OUT_DV) ? 2: 1;

	u32IN_DV = (u32PllReg & CLK_PLLCTL_IN_DIVIDER_MASK) >> 8;
	if (u32IN_DV == 0)
		u32NR = 2;
	else if (u32IN_DV == 1)
		u32NR = 4;
	else if (u32IN_DV == 2)
		u32NR = 8;
	else
		u32NR = 16;

	u32Freq = u32PLLSrc * ((u32PllReg & CLK_PLLCTL_FB_DIVIDER_MASK) +32) / u32NR / u32NO;

	return u32Freq;
}


/**
  * @brief  Get current HCLK clock frquency. 
  * @param  None.
  * @return HCLK clock frquency. The clock UNIT is in Hz.
  */
uint32_t SysGet_HCLKFreq(void)
{
	
	uint32_t u32Freqout, u32AHBDivider, u32ClkSel;

	u32ClkSel = CLK->CLKSEL0 & CLK_CLKSEL0_HCLK_MASK;

	if (u32ClkSel == CLK_CLKSEL0_HCLK_HXT)	/* external HXT crystal clock */
	{
		u32Freqout = __XTAL;	
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_LXT)		/* external LXT crystal clock */ 
	{
		u32Freqout = __RTC_XTAL;
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_PLL)		/* PLL clock */
	{
		u32Freqout = SysGet_PLLClockFreq();
	}
	else if(u32ClkSel == CLK_CLKSEL0_HCLK_LIRC)	/* internal LIRC oscillator clock */
	{
	 	u32Freqout = __IRC10K;
	}
	else									/* internal HIRC oscillator clock */
	{
	 	u32Freqout = __IRC12M;
	
	}
	u32AHBDivider = (CLK->CLKDIV0 & CLK_CLKDIV0_HCLK_MASK) + 1 ;
	
	return (u32Freqout/u32AHBDivider);	
}


/**
  * @brief  Enable chip clock source and select CPU clock. 
  * @param  None
  * @return None
  */
void SysInit_Clock(void)
{
  __IO uint32_t delayCnt;
 
  /* Enable system clock source, HIRC and LIRC are default enabled */	
  UNLOCKREG();
  CLK->PWRCTL |= (CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN);	   
  LOCKREG();


  /* Select HCLK from HIRC */
  CLK->CLKDIV0 &= ~CLK_CLKDIV0_HCLK_MASK; /* divider is 0 */
  UNLOCKREG();
  CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLK_MASK) | CLK_CLKSEL0_HCLK_HIRC;	 /* HCLK = 12MHz */
  LOCKREG();

  /* Update CPU Clock Frequency */
  SystemCoreClockUpdate();
}


/**
  * @brief  Get Core Clock Frequency 
  * @param  None
  * @return None
  */
void SystemCoreClockUpdate (void)           
{
  SystemCoreClock = SysGet_HCLKFreq();
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
  /* Init CPU clock */
  SysInit_Clock();

  /* Select pin functions */
  SysInit_PinFunc();

  /* Enable UART0 as debug console */
  SysInit_DebugConsole();
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
