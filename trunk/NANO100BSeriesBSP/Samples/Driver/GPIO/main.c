
/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx GPIO Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_gpio.h"

extern char GetChar(void);

/**   
  * @brief  main, GPIO sample code.
  * @param  None.
  * @retval None.
  */
int main (void)
{
    int32_t i32Err;

	printf("\n\n");
    printf("+-------------------------------------------------------------------+\n");
    printf("|                      GPIO Driver Sample Code                      |\n");
    printf("+-------------------------------------------------------------------+\n");

/*---------------------------------------------------------------------------------------------------------*/
/* Basic Setting                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/    
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure Bit0 in GPIOA to Output pin and Bit1 in GPIOA to Input pin then close it                  */
    /*-----------------------------------------------------------------------------------------------------*/
	printf("  >> Please connect GPA0 and GPA1 first <<\n");
	printf("  Press any key to continue ...\n");
	GetChar();
	
	GPIO_Open(GPIOA, GPIO_PMD_PMD0_OUTPUT, GPIO_PMD_PMD0_MASK);
	GPIO_Open(GPIOA, GPIO_PMD_PMD1_INPUT, GPIO_PMD_PMD1_MASK);

	GPIO_ClrBit(GPIOA, 0);
	
    i32Err = 0;
    printf("  GPIO Input/Output test ................................ ");

	if (GPIO_GetBit(GPIOA, 1) != 0)
	{
		i32Err = 1;
	}

	GPIO_SetBit(GPIOA, 0);
	
	if (GPIO_GetBit(GPIOA, 1) != 1)
	{
		i32Err = 1;
	}

    GPIO_Close(GPIOA, 0);
    GPIO_Close(GPIOA, 1);
 
    if(i32Err)
    {
	    printf("[FAIL]\n");
        printf("\n  (Please make sure GPA0 and GPA1 are connected)\n");
    }
    else
    {
        printf("[OK]\n");
    }

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO Interrupt Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/    
    
    printf("\n  GPB13, GPC0, GPD1, GPE2, GPF5, GPB14 and GPB15 are used to test key interrupt\n  and control LED (GPA7)\n");    

	printf("  Press any key to continue ...\n");
	GetChar();

    /* Configure general GPIO interrupt */
    GPIO_Open(GPIOB, GPIO_PMD_PMD13_INPUT, GPIO_PMD_PMD13_MASK);

    /* The input mode with pull up enable */
    GPIO_Open(GPIOC, GPIO_PMD_PMD0_INPUT, GPIO_PMD_PMD0_MASK);
	GPIO_EnablePullup(GPIOC, 0);

    GPIO_Open(GPIOD, GPIO_PMD_PMD1_INPUT, GPIO_PMD_PMD1_MASK);
	GPIO_EnablePullup(GPIOD, 1);

    GPIO_Open(GPIOE, GPIO_PMD_PMD2_INPUT, GPIO_PMD_PMD2_MASK);
	GPIO_EnablePullup(GPIOE, 2);

    GPIO_Open(GPIOF, GPIO_PMD_PMD5_INPUT, GPIO_PMD_PMD5_MASK);
	GPIO_EnablePullup(GPIOF, 5);

	GPIO_EnableInt(GPIOB, 13, GPIO_IER_IR_EN_13, GPIO_IMD_EDGE_13);

    /* low level trigger */
	GPIO_EnableInt(GPIOC, 0, GPIO_IER_IF_EN_0, GPIO_IMD_LEVEL_0);
	GPIO_EnableInt(GPIOD, 1, GPIO_IER_IF_EN_1, GPIO_IMD_LEVEL_1);
	GPIO_EnableInt(GPIOE, 2, GPIO_IER_IF_EN_2, GPIO_IMD_LEVEL_2);
	GPIO_EnableInt(GPIOF, 5, GPIO_IER_IF_EN_5, GPIO_IMD_LEVEL_5);

	GPIO_SetDebounceTime(GPIO_DBNCECON_DBCLKSEL_3, GPIO_DBNCECON_DBCLKSRC_HCLK);
	GPIO_EnableDebounce(GPIOB, 13);
	GPIO_EnableDebounce(GPIOC, 0);
	GPIO_EnableDebounce(GPIOD, 1);
	GPIO_EnableDebounce(GPIOE, 2);
	GPIO_EnableDebounce(GPIOF, 5);

    /* Configure external interrupt */
	MFP_EXT_INT0_TO_PB14();
	MFP_EXT_INT1_TO_PB15();
	GPIO_EnableEINT0(GPIOB, 14, GPIO_IER_IF_EN_14, GPIO_IMD_EDGE_14);
	GPIO_EnableEINT1(GPIOB, 15, GPIO_IER_IF_EN_15|GPIO_IER_IR_EN_15, GPIO_IMD_EDGE_15);

    /* Waiting for interrupts */
    while(1);
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


