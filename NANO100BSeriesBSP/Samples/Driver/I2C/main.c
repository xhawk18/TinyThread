/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx I2C Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_i2c.h"
#include "nano1xx_gpio.h"
#include "nano1xx_uart.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t Device_Addr0;
uint8_t Tx_Data0[3];
uint8_t Rx_Data0;
uint8_t DataLen0;
uint8_t volatile EndFlag0 = 0;
uint8_t volatile TimeOutFlag0 = 0; 
uint8_t volatile TimeOutFlag1 = 0;
uint8_t volatile MasterTxGo = 0;
uint8_t volatile MasterRxGo = 0;

uint8_t  Addr1[3] = {0};
uint8_t  DataLen1;
uint8_t  Slave_Buff1[32] = {0};
uint16_t Slave_Buff_Addr1;
/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	uint32_t i;
 
    printf("+---------------------------------------------------------------------+\n");
    printf("|                       I2C Driver Sample Code                        |\n");
    printf("+---------------------------------------------------------------------+\n");
    printf("  I/O Configuration:\n");
    printf("    GPA8 <--> GPA10\n");
    printf("    GPA9 <--> GPA11\n");                
    printf("\n");

    /* Set multi function pin for I2C0 */
    GCR->PA_H_MFP = GCR->PA_H_MFP & ~(PA8_MFP_MASK|PA9_MFP_MASK) | 
					PA8_MFP_I2C0_SDA | PA9_MFP_I2C0_SCL;
	
	/* Set multi function pin for I2C1 */				
	GCR->PA_H_MFP = GCR->PA_H_MFP & ~(PA10_MFP_MASK|PA11_MFP_MASK) | 
					PA10_MFP_I2C1_SDA | PA11_MFP_I2C1_SCL;				
	
	I2C_Init(I2C0);
	I2C_Init(I2C1);
        
	/* Open I2C0 and I2C1, and set clock = 100Kbps */
	I2C_Open(I2C0, 100000);
	I2C_Open(I2C1, 100000);

	/* Get I2C0 clock */
	printf("I2C0 clock %d Hz\n", I2C_GetClockFreq(I2C0));
	/* Get I2C1 clock */
	printf("I2C1 clock %d Hz\n", I2C_GetClockFreq(I2C1));	

	/* Set I2C0 slave addresses */
	I2C_SetAddress(I2C0, 0, 0x15, 0);
	I2C_SetAddress(I2C0, 1, 0x35, 0);
	
	/* Set I2C1 slave addresses */
	I2C_SetAddress(I2C1, 0, 0x16, 0);
	I2C_SetAddress(I2C1, 1, 0x36, 0);
		
    for (i = 0; i < 32; i++)
	{
		Slave_Buff1[i] = 0;
	}	
	
	/* Set AA bit, I2C1 as slave (To simulate a 24LXX EEPROM) */
	I2C_Ctrl(I2C1, 0, 0, 1);
		
	/* Enable I2C0 interrupt and set corresponding NVIC bit */
	I2C_EnableInt(I2C0);
		
	/* Enable I2C1 interrupt and set corresponding NVIC bit */
	I2C_EnableInt(I2C1);
		
	I2C_SetTimeoutCounter(I2C0, 1, 0);
	I2C_SetTimeoutCounter(I2C1, 1, 0);
		
	Device_Addr0 = 0x16;
	Tx_Data0[0] = 0x00;
	Tx_Data0[1] = 0x00;
		
	for (i = 0; i < 1; i++)
	{
		Tx_Data0[2] = 0x10 + i;
		DataLen0 = 0;
		EndFlag0 = 0;
		
		MasterTxGo = 1;				
		/* I2C0 as master sends START signal */
		I2C_Ctrl(I2C0, 1, 0, 0);
				
		/* Wait I2C0 Tx Finish */
		while (EndFlag0 == 0 && TimeOutFlag0 != 1);
		if(TimeOutFlag0)
			goto fail;
			
		EndFlag0 = 0;
		MasterTxGo = 0;
		
		MasterRxGo = 1;
		DataLen0 = 0;
		Device_Addr0 = 0x16;
		I2C_Ctrl(I2C0, 1, 0, 0);
		
		/* Wait I2C0 Rx Finish */
		while (EndFlag0 == 0&& TimeOutFlag0 != 1);
		if(TimeOutFlag0)
			goto fail;
		MasterRxGo = 0;
		
		/* Compare data */
		if (Rx_Data0 != Tx_Data0[2])
		{
			printf("I2C0 Byte Write/Read Failed, Data 0x%x\n", Rx_Data0);
			return -1;
		}			
	}
	printf("I2C0(Master) <==> I2C1(Slave) Test OK\n");

fail:
	/* Disable I2C0 interrupt and clear corresponding NVIC bit */
	I2C_DisableInt(I2C0);
	
	/* Disable I2C1 interrupt and clear corresponding NVIC bit */
	I2C_DisableInt(I2C1);

	/* Close I2C0 and I2C1 */
	I2C_Close(I2C0);
	I2C_Close(I2C1);
	
	I2C_DeInit(I2C0);
	I2C_DeInit(I2C1);
	
	return 0;
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

