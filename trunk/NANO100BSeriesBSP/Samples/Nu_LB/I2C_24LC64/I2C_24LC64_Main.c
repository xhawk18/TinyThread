/******************************************************************************
 * @file     I2C_24CL64_Main.c
 * @brief    This sample show the usage of I2C APIs to access 24LC64 EEPROM.
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "EEPROM_24LC64.h"

void delay_loop(void)
{
 	uint32_t i,j;
	for(i=0;i<3;i++)	
	{
		for(j=0;j<60000;j++);
    } 
}

void delay(void)
{
	int j;
   	for(j=0; j<1000; j++);
}


int main(void)
{
	uint32_t i2cdata=0;		
			
	printf("+---------------------------------------------------------------------+\n");
    printf("|                       I2C Driver Sample Code                        |\n");
    printf("|                                                                     |\n");
    printf("+---------------------------------------------------------------------+\n");
    
 	/* Set multi function pin for I2C0 */
    GCR->PA_H_MFP = GCR->PA_H_MFP & ~(PA8_MFP_MASK|PA9_MFP_MASK) | 
					PA8_MFP_I2C0_SDA | PA9_MFP_I2C0_SCL;
    
	Write_24LC64(0x00000000, 0x55);
	SYS_Delay(10000);		  								//delay for finish writting
	i2cdata= Read_24LC64(0x00000000);

	if(i2cdata != 0x55)
		printf("EEPROM Read/Write Test fail!\n");
	else
		printf("EEPROM Read/Write Test pass!\n");		
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
