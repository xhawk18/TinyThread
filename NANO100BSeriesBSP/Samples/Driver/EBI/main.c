/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx EBI Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_ebi.h"
#include "nano1xx_sys.h"

extern void SARM_BS616LV4017(void);	
extern void NOR_W39L010(void);

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/    
int main (void)
{
	EBI_TIMING_T	sEBITiming;
	uint32_t		u32SysHCLKFreq;
	uint8_t			u8Item = 0x0;

	UNLOCKREG();

	printf("\n\n");
    printf("+------------------------------------------------------------------+\n");
    printf("|                      EBI Driver Sample Code                      |\n");
    printf("+------------------------------------------------------------------+\n");
	printf("\n");
	
	// Enable EBI function with 16-bit data width
 	EBI_Init(0);
							  
	// Configure EBI timing
    u32SysHCLKFreq = 32000000;
    printf("HCLK is %d\n", u32SysHCLKFreq);
    printf("MCLK of EBI must be less than 18 MHz for Read/Write test.\n");
 	sEBITiming.eMCLKDIV = E_EBI_MCLKDIV_2;
    printf("MCLK of EBI is %d KHz.\n", u32SysHCLKFreq/2);

 	sEBITiming.u8ExttALE = 2;
	sEBITiming.u8ExtIR2R = 2;
	sEBITiming.u8ExtIR2W = 2;
	sEBITiming.u8ExtIW2X = 2;
	sEBITiming.u8ExttAHD = 2;
	sEBITiming.u8ExttACC = 2;
	EBI_SetBusTiming(sEBITiming);

	// Get EBI timing
 	EBI_GetBusTiming(&sEBITiming);
	printf("u8MCLKDIV=%d \nu8ExttALE=%d \nu8ExtIR2R=%d \nu8ExtIR2W=%d \nu8ExtIW2X=%d \nu8ExttAHD=%d \nu8ExttACC=%d\n\n",
			sEBITiming.eMCLKDIV,  sEBITiming.u8ExttALE, sEBITiming.u8ExtIR2R,  
			sEBITiming.u8ExtIR2W, sEBITiming.u8ExtIW2X, sEBITiming.u8ExttAHD, sEBITiming.u8ExttACC);  

	do {
		printf("*******************************************************************\n");
		printf("* Please connect BS616LV4017 or W39L010 to NANO100 Series EBI bus *\n");
		printf("* before EBI testing!!                                            *\n");
		printf("*                                                                 *\n");								
		printf("* The testing result will be FAILED, if there is no BS616LV4017   *\n");
		printf("* or W39L010 connecting to NANO100 sereis EBI bus!                *\n");
		printf("*******************************************************************\n");
		printf("\n");
		printf("|   >>> Selese item to test <<<    |\n");
		printf("| [0] SARM  --- BS616LV4017, 16bit |\n");
		printf("| [1] NOR   --- W39L010, 8bit      |\n");
		printf("| [ESC] Exit                       |\n");
		u8Item = getchar();

		printf("\n");
		switch (u8Item)
		{
			case '0':
				SARM_BS616LV4017();
				break;
				
			case '1':		
				NOR_W39L010();		
				break;
		}
	} while (u8Item != 27);

	// Disable EBI function
	EBI_DeInit();
	
	printf("Exit EBI Test Function \n\n");
	
	return 1;
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
