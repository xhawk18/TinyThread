/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx PDMA_CRC Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "nano1xx_pdma.h"
#include "nano1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

volatile uint32_t g_uTotalByteCnt = 0;

/*---------------------------------------------------------------------------------------------------------*/
/* CPU Mode Sample Code:                                                                                  	   */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  CRC_CPU_Mode_Test, CRC CPU mode test sample code.
  * @param  None.
  * @retval None.
  */
void CRC_CPU_Mode_Test(void)
{
    volatile uint32_t i;
    uint8_t *p8BufAddr, u8WriteLength;  
	uint32_t SrcData;
    uint32_t uInteger, uRemainder;
    volatile uint32_t uFinalCRC;

	/* PDMA Init */
    PDMA_Init();

 	printf("  This sample code will do CRC CPU Mode Test. \n");

    p8BufAddr = (uint8_t *)"123456789"; 
    g_uTotalByteCnt = strlen("123456789");

 	/* CRC Setting */
	PDMAGCR->CSR |= PDMAGCR_CSR_CRCCLK;			/* Enable CRC Clock */
	/* Reset CRC, CPU Write data length-> 32-bit */
	PDMACRC->CTL = (uint32_t)(PDMACRC_CTL_CEN|PDMACRC_CTL_RST|PDMACRC_CTL_CPU_WDLEN_32BIT);

	/*Set SEED-> 0xFFFF */
	PDMACRC->SEED = 0xFFFF;

    uInteger = g_uTotalByteCnt / 4;
    uRemainder = g_uTotalByteCnt % 4;
    u8WriteLength = 4;

    for (i=0; i<uInteger; i++)
    {
        memcpy(&SrcData, p8BufAddr, u8WriteLength);
		PDMACRC->WDATA = SrcData;
		p8BufAddr += u8WriteLength;
	}

    if (uRemainder != 0)
    {
        if (uRemainder == 3)
        {
			PDMACRC->CTL = (PDMACRC->CTL & ~PDMACRC_CTL_CPU_WDLEN_MASK|PDMACRC_CTL_CPU_WDLEN_16BIT);
            memcpy(&SrcData, p8BufAddr, 2);
            SrcData &= 0xFFFF;
			PDMACRC->WDATA = SrcData;
    
            p8BufAddr = p8BufAddr + 2;
			PDMACRC->CTL = (PDMACRC->CTL & ~PDMACRC_CTL_CPU_WDLEN_MASK|PDMACRC_CTL_CPU_WDLEN_8BIT);
            SrcData &= 0xFF;
            memcpy(&SrcData, p8BufAddr, 1);
			PDMACRC->WDATA = SrcData;
        }
		else if (uRemainder == 2)
        {
			PDMACRC->CTL = (PDMACRC->CTL & ~PDMACRC_CTL_CPU_WDLEN_MASK|PDMACRC_CTL_CPU_WDLEN_16BIT);
            memcpy(&SrcData, p8BufAddr, 2);
            SrcData &= 0xFFFF;
			PDMACRC->WDATA = SrcData;
        }
		else if (uRemainder == 1)
        {
			PDMACRC->CTL = (PDMACRC->CTL & ~PDMACRC_CTL_CPU_WDLEN_MASK|PDMACRC_CTL_CPU_WDLEN_8BIT);
            SrcData &= 0xFF;
            memcpy(&SrcData, p8BufAddr, 1);
			PDMACRC->WDATA = SrcData;
        }
    }
    uFinalCRC = PDMACRC->CHECKSUM & PDMACRC->SEED;    

	if (uFinalCRC == 0x29B1)
		printf("CRC CPU mode test pass\n");
	else
		printf("CRC CPU mode test fail\n");

	/* Close PDMA CRC Channel */
	PDMAGCR->CSR = 0;

	/* PDMA DeInit */
    PDMA_DeInit();

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* DMA Mode Sample Code:                                                                                  	   */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  CRC_DMA_Mode_Test, CRC DMA mode test sample code.
  * @param  None.
  * @retval None.
  */
void CRC_DMA_Mode_Test(void)
{
    volatile uint32_t i;
    uint8_t *p8BufAddr;  
    volatile uint32_t uFinalCRC;

	/* PDMA Init */
    PDMA_Init();

 	printf("  This sample code will do CRC DMA Mode Test. \n");

    p8BufAddr = (uint8_t *)"123456789"; 
    g_uTotalByteCnt = strlen("123456789");

 	/* CRC Setting */
	PDMAGCR->CSR |= PDMAGCR_CSR_CRCCLK;			/* Enable CRC Clock */
	/* Reset CRC, CPU Write data length-> none */
	PDMACRC->CTL = (uint32_t)(PDMACRC_CTL_CEN|PDMACRC_CTL_RST|PDMACRC_CTL_CPU_WDLEN_MASK);

	PDMACRC->DMASAR = (uint32_t)p8BufAddr;		/* Set Source Address */
	PDMACRC->DMABCR = g_uTotalByteCnt;			/* Set Byte Count Register */

	/*Set SEED-> 0xFFFF */
	PDMACRC->SEED = 0xFFFF;

	/* Trigger CRC DMA */
	PDMACRC->CTL |= PDMACRC_CTL_TRIG_EN;

    while (1)
    {
		if (PDMACRC->DMAISR & PDMACRC_DMAISR_TABORT)
        {
            PDMACRC->DMAISR |= PDMACRC_DMAISR_TABORT;
            printf("    CRC target abort !!! \n");   
            return ;
        }
        
        if (PDMACRC->DMAISR & PDMACRC_DMAISR_TD)
        {
            PDMACRC->DMAISR |= PDMACRC_DMAISR_TD;

            // Read the CRC checksum result ......
		    uFinalCRC = PDMACRC->CHECKSUM & PDMACRC->SEED;
			break;    
        }
	}

	if (uFinalCRC == 0x29B1)
		printf("CRC DMA mode test pass\n");
	else
		printf("CRC DMA mode test fail\n");

	/* Close PDMA CRC Channel */
	PDMAGCR->CSR = 0;

	/* PDMA DeInit */
    PDMA_DeInit();

	return;
}

/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                           	   			   */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  main.
  * @param  None.
  * @retval None.
  */
int32_t main()
{
	printf("\n\n");
	printf("+------------------------------------------------------------------------+\n");
    printf("|                     PDMA Driver Sample Code for CRC                    |\n");
    printf("+------------------------------------------------------------------------+\n");                    

	CRC_CPU_Mode_Test();

	CRC_DMA_Mode_Test();

    printf("\n\n  PDMA CRC sample code is complete.\n");
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


