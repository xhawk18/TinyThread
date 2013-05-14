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
  DEFINES
 *----------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define __SYS_OSC_CLK     (    ___HSI)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (1*__XTAL)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SetSystemClock(void)
{
  __IO uint32_t delayCnt;
  
  /* Enable system clock source */	
  UNLOCKREG();
  CLK->PWRCTL |= CLK_PWRCTL_HXT_EN;
  LOCKREG();

  /* Enable PLL out to 96MHz */
#ifdef _NO_USE_HXT_
  CLK->PLLCTL = (CLK_PLLCTL_PLLSRC_HIRC | PLL_IN_12M_OUT_96M);
#else
  CLK->PLLCTL = (CLK_PLLCTL_PLLSRC_HXT | PLL_IN_12M_OUT_96M);
#endif

  /* Waits for PLL clock stable */
  for (delayCnt=0; delayCnt<100000; delayCnt++)	
		if (CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB)	break;

  /* Change HCLK to PLL output */		  
  if (delayCnt < 100000) {
	  CLK->CLKDIV0 = (CLK->CLKDIV0 & ~CLK_CLKDIV0_HCLK_MASK) | 2; /* divider is 3 */
	  UNLOCKREG();
      CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLK_MASK) | CLK_CLKSEL0_HCLK_PLL;	 /* HCLK = 32MHz */
      LOCKREG();
  }

  /* Set HCLK back to HIRC if error happens */
  if (CLK->CLKSTATUS  & CLK_CLKSTATUS_CLK_SW_FAIL) 	{
	  CLK->CLKDIV0 &= ~CLK_CLKDIV0_HCLK_MASK; /* divider is 1 */
	  UNLOCKREG();
      CLK->CLKSEL0 = (CLK->CLKSEL0 & ~CLK_CLKSEL0_HCLK_MASK) | CLK_CLKSEL0_HCLK_HIRC;	 /* HCLK = 12MHz */
      LOCKREG();
  }
  
  /* Enable UART0 for debug console */
  MFP_UART0_TO_PORTA();
  //GCR->PB_L_MFP = ((GCR->PB_L_MFP & ~0x7777) | 0x1111);  /* Select multi-function pin for UART0 */
  CLK->APBCLK |= CLK_APBCLK_UART0_EN;                         /* Enable UART0 clock */
  CLK->CLKDIV0 &= ~CLK_CLKDIV0_UART_MASK;
#ifdef _NO_USE_HXT_
  CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HIRC;  /* Select 12 Mhz HIRC */
#else
  CLK->CLKSEL1 = (CLK->CLKSEL1 & ~CLK_CLKSEL1_UART_MASK) | CLK_CLKSEL1_UART_HXT;  /* Select 12 Mhz XTAL */
#endif

  UART0->BAUD = 0x67;              /* Baud Rate:115200  OSC:12MHz */
  UART0->TLCTL = 0x03;             /* Character len is 8 bits */

}

void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
  SystemCoreClock = __SYSTEM_CLOCK;
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
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */


  //SystemCoreClock = __SYSTEM_CLOCK;
  SetSystemClock();
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
