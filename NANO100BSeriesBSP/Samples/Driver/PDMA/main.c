/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx PDMA Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx_pdma.h"
#include "nano1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

int32_t PDMA_TEST_LENGTH = 64;
uint8_t SrcArray[64];
uint8_t DestArray[64];
int32_t IsTestOver = 0;
extern char GetChar(void);
/*---------------------------------------------------------------------------------------------------------*/
/* Clear buffer funcion                                                                              	   */
/*---------------------------------------------------------------------------------------------------------*/

void ClearBuf(uint32_t u32Addr, uint32_t u32Length,uint8_t u8Pattern)
{
	uint8_t* pu8Ptr;
	uint32_t i;
	
	pu8Ptr = (uint8_t *)u32Addr;
	
	for (i=0; i<u32Length; i++)
	{
		*pu8Ptr++ = u8Pattern;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Bulid Src Pattern function                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/

void BuildSrcPattern(uint32_t u32Addr, uint32_t u32Length)
{
    uint32_t i=0,j,loop;
    uint8_t* pAddr;
    
    pAddr = (uint8_t *)u32Addr;
    
    do {
        if (u32Length > 256)
	    	loop = 256;
	    else
	    	loop = u32Length;
	    	
	   	u32Length = u32Length - loop;    	

        for(j=0;j<loop;j++)
            *pAddr++ = (uint8_t)(j+i);
            
	    i++;        
	} while ((loop !=0) || (u32Length !=0));         
}

/*---------------------------------------------------------------------------------------------------------*/
/* PDMA Sample Code:                                                                                  	   */
/*---------------------------------------------------------------------------------------------------------*/
/**   
  * @brief  PDMA_Test, Memory to memory transfer test.
  * @param  None.
  * @retval None.
  */
void PDMA_Test(void)
{
	STR_PDMA_T sPDMA;

	BuildSrcPattern((uint32_t)SrcArray, PDMA_TEST_LENGTH);
    ClearBuf((uint32_t)DestArray, PDMA_TEST_LENGTH, 0xFF);
    
	/* PDMA Init */
    PDMA_Init();

 	printf("  This sample code will do Memory to Memory Test. \n");

	/* PDMA Setting - disable channel 1 service selection */
	PDMA_DISABLE_CH1();

 	/* CH1 Setting */
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK1);/* Enable Channel 1 Clock */

	/* memoey to memory test, transfer width-> 32-bit */
	/* source address-> incremented, destination address -> incremented */
	sPDMA.SrcAddr  = (uint32_t)SrcArray;
    sPDMA.DestAddr = (uint32_t)DestArray;   
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_INC; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_INC;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_32BIT;
	sPDMA.Mode = PDMA_CSR_MEM2MEM;
	sPDMA.ByteCnt  = PDMA_TEST_LENGTH;
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA1, &sPDMA);

	/* Enable Channel 1 INT - transfer block done */
    PDMA_EnableInt(PDMA1, PDMA_IER_TD);

	/* Trigger PDMA Channel 1 */
 	PDMA_CHEnableTransfer(PDMA1);

	/* Wait DMA Interrupt */
	while(IsTestOver == FALSE);

	/* Close PDMA Channel */
	PDMA_Close();

	/* PDMA DeInit */
    PDMA_DeInit();

	return;
}



/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                           	   			   */
/*---------------------------------------------------------------------------------------------------------*/

/**   
  * @brief  main, PDMA test.
  * @param  None.
  * @retval None.
  */
int32_t main()
{
	printf("\n\n");
	printf("+------------------------------------------------------------------------+\n");
    printf("|                         PDMA Driver Sample Code                        |\n");
    printf("+------------------------------------------------------------------------+\n");                    

	PDMA_Test();

    printf("\n\n  PDMA sample code is complete.\n");
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

