/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx SYS Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"

#define SIGNATURE       0x125ab234
#define FLAG_ADDR       0x20003FFC

extern __IO uint32_t u32PWDU_WakeFlag;
extern __IO uint32_t u32WDT_Ticks;



int32_t main (void)
{
    __IO uint32_t u32data;
	S_SYS_CHIP_CLKCFG sChipClkCfg;

    printf("+------------------------------------------------------------------------+\n");
    printf("|                       SYS Driver Sample Code V1.00                     |\n");
    printf("+------------------------------------------------------------------------+\n");                    

    /* Unlock protected registers */
    UNLOCKREG();

	/* Check the signature word */
    if (M32(FLAG_ADDR) == SIGNATURE)
    {
        printf("CPU Reset Successfully.\n");
        M32(FLAG_ADDR) = 0;
    }

	/* Check and Clear reset sources */
	u32data = SYS_GetResetSource();
	printf("Reset Source : [%08X]\n", u32data);
	SYS_ClearResetSource(u32data);

    /* Read Product ID */
	u32data = SYS_ReadProductID();
	printf("Product ID : [%08X]\n", u32data);

    /* Init Chip clock source and IP clocks */
	sChipClkCfg.u32ChipClkEn = CLK_PWRCTL_HXT_EN | CLK_PWRCTL_LXT_EN;
	sChipClkCfg.u32PLLClkSrc = CLK_PLLCTL_PLLSRC_HXT;
    sChipClkCfg.u8PLLEnable  = 1;   /* enable PLL */
	sChipClkCfg.ePLLInFreq   = E_SYS_PLLIN_12M;    
    sChipClkCfg.ePLLOutFreq  = E_SYS_PLLOUT_120M;  
	sChipClkCfg.u32HClkSrc   = CLK_CLKSEL0_HCLK_PLL; 	
    sChipClkCfg.u32HClkDiv   = HCLK_CLK_DIVIDER(2);    /* HCLK = 120/(2+1) = 40 MHz */
 	/* IPs clock setting */
	sChipClkCfg.u32AHBClkEn  = CLK_AHBCLK_TICK_EN | CLK_AHBCLK_GPIO_EN;
    sChipClkCfg.u32APBClkEn  = CLK_APBCLK_UART0_EN | CLK_APBCLK_TMR0_EN | CLK_APBCLK_I2S_EN;

    SYS_InitChipClock(&sChipClkCfg);

	/* Set IP clock source and divider */
	/* I2S */
	SYS_SetClockDivider_0(CLK_CLKDIV0_I2S_MASK, I2S_CLK_DIVIDER(5));
	SYS_SelectIPClockSource_2(CLK_CLKSEL2_I2S_MASK, CLK_CLKSEL2_I2S_PLL);

	/* Timer 0 */
	SYS_SelectIPClockSource_2(CLK_CLKSEL1_TMR0_MASK, CLK_CLKSEL1_TMR0_HXT);	

#if 1
	/* Dump register setting */
	printf("\n============================================\n");
	printf("Clock Control Registers : \n\n");
	printf("PWRCTL  = [%08X]\n", CLK->PWRCTL);
	printf("PLLCTL  = [%08X]\n", CLK->PLLCTL);
	printf("AHBCLK  = [%08X]\n", CLK->AHBCLK);
	printf("APBCLK  = [%08X]\n", CLK->APBCLK);
	printf("CLKSEL0 = [%08X]\n", CLK->CLKSEL0);
	printf("CLKSEL1 = [%08X]\n", CLK->CLKSEL1);
	printf("CLKSEL2 = [%08X]\n", CLK->CLKSEL2);
	printf("CLKDIV0 = [%08X]\n", CLK->CLKDIV0);
	printf("CLKDIV1 = [%08X]\n", CLK->CLKDIV1);
	printf("============================================\n");
#endif

	/* Read CPU clock frequency */
	//SYS_SelectHCLKSource(CLK_CLKSEL0_HCLK_HXT);	 /* Select HCLK from HXT, usually 12MHz */
	//SYS_SetHCLKDivider(0);						 /* Set HCLK divider to 0, HCLK = HXT / (divider +1) */
	printf("SystemCoreClock = [%08d]\n", SystemCoreClock);
	u32data = SYS_GetHCLKFreq();
	printf("CPU Clock Speed : [%d MHz]\n\n", u32data/1000000);
	
    /* Enable WDT clock */
	SYS_SetWDTClock(1);  

	/* Enable WDT and interrupt */
	WDT->CTL = WDT_CTL_WTIS_2POW14 | WDT_CTL_WTWKE | WDT_CTL_WTE;
	WDT->IER |=  WDT_IER_WDTIE;
	NVIC_EnableIRQ(WDT_IRQn);
  
	/* Power Down Test */
	SYS_SetUpPowerDown(1) ; /* Power down setting and reable PDWU interrupt */

	printf("u32PWDU_WakeFlag = %x\n",u32PWDU_WakeFlag);
	printf("Enter Power Down Mode >>>>>>>>>>>\n");
	u32PWDU_WakeFlag = 0;					/* clear software semaphore */
    while(!(UART0->FSR & UART_FSR_TE_F)) ;  /* waits for message send out */

	__WFI(); /* really enter power down here !!! */

	printf("<<<<<<<<<< Program resumes execution.\n");
	printf("u32PWDU_WakeFlag = %x\n",u32PWDU_WakeFlag);

	/* CPU Reset test */
	
	printf("Waits for 5 times WDT interrupts.....\n");
    while (u32WDT_Ticks <= 5);
	  
    M32(FLAG_ADDR) = SIGNATURE;	  /* write the signature word for reset check */ 
    printf("Going to reset CPU.....\n\n\n");

    while(!(UART0->FSR & UART_FSR_TE_F)) ;  /* waits for message send out */

	/* Reset CPU */
	SYS_ResetProtectIP(GCR_IPRSTCTL1_CPU);

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
 	GCR->PB_L_MFP = (GCR->PB_L_MFP & ~0x77) | (PB1_MFP_UART0_TX | PB0_MFP_UART0_RX);  /* Select multi-function pin for UART0 */
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
