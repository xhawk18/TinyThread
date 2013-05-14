/******************************************************************************
 * @file     EBI_LCM.c
 * @brief    LCM demo program Main function
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_sys.h"
#include "EBI_LCM_OTM3225A.h"

extern void EBI_LCM_Initial(void);
  
int main (void)
{
	uint16_t regcontent;
	volatile uint32_t loop, xaxis, yaxis;
      
	printf("\n\n");
    printf("+-------------------------------------------------------------+\n");
    printf("|                   EBI LCD Sample Code                    |\n");
    printf("+-------------------------------------------------------------+\n");
	printf("\n");
	
	/* initailize EBI and pins */
	EBI_LCM_Initial();
	
	// read something from LCD driver
	OTM3225A_IDRead(ID_READ_ADDR, &regcontent);
	printf("ID = 0x%x \n", regcontent);	

	while(1) 
	{
		/* panel initialization */
		OTM3225_init(RES_PORTRAIT_240X320);
		ClearScreen(0x0000);
		Show_String32x32(1, 1, "Nuvoton", 0xf800);
		Show_String32x32(1, 33, "Tech.", 0xf800);
		Show_String32x32(1, 65, "Company", 0xf800);
		Show_String32x32(1, 97, "Nano 100", 0xf800);
		Show_String32x32(1, 129, "Protrait", 0xf800);
		ClearScreen(0x0000);
		
		/* panel initialization */
		OTM3225_init(RES_LANDSCAPE_320X240);
		ClearScreen(0x0000);
		Show_String32x32(1, 1, "Nuvoton", 0xf800);
		Show_String32x32(1, 33, "Tech.", 0xf800);
		Show_String32x32(1, 65, "Company", 0xf800);
		Show_String32x32(1, 97, "Nano 100", 0xf800);
		Show_String32x32(1, 129, "Landscape", 0xf800);
		ClearScreen(0x0000);
	}	

	
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


	 
