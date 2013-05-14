/******************************************************************************
 * @file     main.c
 * @brief    Nano1xx SPI Driver Sample code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_spi.h"
#include "nano1xx_pdma.h"
#include "spiFlash.h"

#define TEST_COUNT 32

void SpiLoopbackTest(void);
void SpiLoopbackTest_WithPDMA(void);
void SpiLoopbackTest_WithFIFO(void);
void SpiFlashTest(void);
void PDMA_Callback(uint32_t u32UserData);

uint32_t volatile g_Spi0IntFlag;
uint32_t volatile g_Spi1IntFlag;
uint32_t volatile g_PdmaIntFlag;

void showLoopBackTestNote(void)
{
	printf("Configure SPI0 (master) <==> SPI1 (slave)\n");
	printf("SPI clock rate: [%d Hz]\n\n", SPI_GetClock1Freq(SPI0));
	printf("The I/O connection for SPI0/SPI1 loopback:\n    SPI0_SS0(GPC0)   <--> SPI1_SS0(GPC8)\n    SPI0_CLK(GPC1)   <--> SPI1_CLK(GPC9)\n");
	printf("    SPI0_MISO0(GPC2) <--> SPI1_MISO0(GPC10)\n    SPI0_MOSI0(GPC3) <--> SPI1_MOSI0(GPC11)\n\n");
	printf("Please connect SPI0 with SPI1, and press any key to start transmission.\n");
	getchar();
}

int main(void)
{
	uint8_t u8Option;

	SPI_Init(SPI0);
	SPI_Init(SPI1);

	printf("\n\n");
	printf("+-------------------------------------------------+\n");
	printf("|         Nano100 SPI Driver Sample Code          |\n");	
	printf("+-------------------------------------------------+\n");

	/* Enable SPI0 multi function pins */
	/*	CLK = GPC1
		SSO = GPC0
		MISO = GPC2
		MOSI = GPC3
	*/	
	GCR->PC_L_MFP = GCR->PC_L_MFP & ~(PC0_MFP_MASK|PC1_MFP_MASK|PC2_MFP_MASK|PC3_MFP_MASK) | 
					PC0_MFP_SPI0_SS0 | PC1_MFP_SPI0_SCLK | PC2_MFP_SPI0_MISO0 | PC3_MFP_SPI0_MOSI0;
	
	/* Enable SPI1 multi function pins */
	/*	CLK = GPC9
		SSO = GPC8
		MISO = GPC10
		MOSI = GPC11
	*/
	GCR->PC_H_MFP = GCR->PC_H_MFP & ~(PC8_MFP_MASK|PC9_MFP_MASK|PC10_MFP_MASK|PC11_MFP_MASK) | 
					PC8_MFP_SPI1_SS0 | PC9_MFP_SPI1_SCLK | PC10_MFP_SPI1_MISO0 | PC11_MFP_SPI1_MOSI0;
	
	//select SPI0, SPI1 clock source are both from PLL
	CLK->CLKSEL2 = (CLK->CLKSEL2 & CLK_CLKSEL2_SPI0_MASK) | CLK_CLKSEL2_SPI0_PLL;
	CLK->CLKSEL2 = (CLK->CLKSEL2 & CLK_CLKSEL2_SPI1_MASK) | CLK_CLKSEL2_SPI1_PLL;

	while(1)
	{
		printf("+-------------------------------------------------+\n");
		printf("| 1. SPI flash read/write ................... [1] |\n");
		printf("| 2. General SPI data transfer ...............[2] |\n");
		printf("| 3. SPI data transfer by PDMA ...............[3] |\n");
		printf("| 4. SPI data transfer by FIFO ...............[4] |\n");		
		printf("| 5. Quit ...........................[Other keys] |\n");
		printf("+-------------------------------------------------+\n");
		u8Option = getchar();

		if( u8Option == '1' )
			SpiFlashTest();			
		else if( u8Option == '2' )
			SpiLoopbackTest();			
		else if( u8Option == '3' )
			SpiLoopbackTest_WithPDMA();
		else if( u8Option == '4' )
			SpiLoopbackTest_WithFIFO();
		else
			break;
	}
	
	printf("\n\nSPI driver sample code is completed.\n");
	
	return 0;
}


void SpiLoopbackTest(void)
{    
	uint32_t au32SourceData[TEST_COUNT];
	uint32_t au32DestinationData[TEST_COUNT];
	uint32_t u32DataCount, u32TestCycle;
	int32_t i32Err;
	uint32_t u32Tmp;
	SPI_DATA_T conf;

	/* SPI-0 */

	/* Configure SPI0 as a master, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_MASTER;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI0, &conf);	
	
	/* Enable the automatic slave select function of SS0. */
	SPI_EnableAutoSS(SPI0, SPI_SS0);
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI0);
	
	/* SPI clock rate 2MHz */
	SPI_SetClockFreq(SPI0, 2000000, 0);
	
	/* SPI-1 */
	
	/* Configure SPI1 as a slave, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_SLAVE;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI1, &conf);	
	
	/* Set the trigger mode of slave select pin. */
	SPI_SetLevelTriggerMode(SPI1);   /* level trigger */
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI1);

	showLoopBackTestNote();
		
	/* ========= */
	printf("\nSPI0 <-> SPI1 Normal Loop Back Test");
	
	/* Enable the SPI1 interrupt */
	SPI_EnableInt(SPI1);
	
	/* Clear Tx register of SPI1 to avoid send non-zero data to Master. Just for safe. */
	u32Tmp = 0;
	SPI_SetTxRegister(SPI1, &u32Tmp, 1);
	
	/* set the soure data and clear the destination buffer */
	for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
	{
		au32SourceData[u32DataCount] = u32DataCount;
		au32DestinationData[u32DataCount] = 0;
	}
	
	i32Err = 0;
	for(u32TestCycle=0; u32TestCycle<10000; u32TestCycle++)
	{
		u32DataCount=0;
		g_Spi1IntFlag = 0;

		if((u32TestCycle&0x1FF) == 0)
		{
			putchar('.');
		}
		/* set the GO_BUSY bit of SPI1 */
		SPI_SetGo(SPI1);
		/* write the first data of source buffer to Tx register of SPI0. And start transmission. */
		SPI_SingleWrite(SPI0, &au32SourceData[0]);
    
		while(1)
		{
			if(g_Spi1IntFlag==1)
			{
				g_Spi1IntFlag = 0;
				
				if(u32DataCount<(TEST_COUNT-1))
				{
					/* Read the previous retrieved data and trigger next transfer. */
					SPI_SingleRead(SPI1, &au32DestinationData[u32DataCount]);
					u32DataCount++;
					SPI_SingleWrite(SPI0, &au32SourceData[u32DataCount]);
				}
				else
				{
					/* Just read the previous retrived data but trigger next transfer, because this is the last transfer. */
					SPI_DumpRxRegister(SPI1, &au32DestinationData[u32DataCount], 1);
					break;
				}
			}
		}
		
		for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
		{
			if(au32DestinationData[u32DataCount]!=au32SourceData[u32DataCount])
			{
				i32Err = 1;
			}
		}

		if(i32Err)
			break;
	}

	if(i32Err)
	{	
		printf(" [FAIL]\n\n");
	}
	else
	{
		printf(" [OK]\n\n");
	}
	
	/* Disable the SPI1 interrupt */
	SPI_DisableInt(SPI1);
	
	SPI_Close(SPI0);
	SPI_Close(SPI1);
	
	return ;
}

void SpiLoopbackTest_WithPDMA(void)
{
 	uint8_t u8Option;
	uint32_t au32SourceData[TEST_COUNT];
	uint32_t au32DestinationData[TEST_COUNT];
	uint32_t u32DataCount, u32TestCycle;
	int32_t i32Err;
	uint32_t u32Tmp;
	STR_PDMA_T sPDMA;
 	SPI_DATA_T conf;
	
	/* SPI-0 */
	
	/* Configure SPI0 as a master, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_MASTER;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI0, &conf);	
	
	/* Enable the automatic slave select function of SS0. */
	SPI_EnableAutoSS(SPI0, SPI_SS0);
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI0);
	
	/* SPI clock rate 2MHz */
	SPI_SetClockFreq(SPI0, 2000000, 0);
	
	/* SPI-1 */
	
	/* Configure SPI1 as a slave, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_SLAVE;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI1, &conf);	
	
	/* Set the trigger mode of slave select pin. */
	SPI_SetLevelTriggerMode(SPI1);   /* level trigger */
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI1);
	
	showLoopBackTestNote();
	printf("+------------------------------------------------+\n");
	printf("| 1. Use one PDMA channel .................. [1] |\n");
	printf("| 2. Use two PDMA channels ................. [2] |\n");
	printf("| 3. Return ........................[Other keys] |\n");
	printf("+------------------------------------------------+\n");	
	u8Option = getchar();
   	
	if( u8Option == '1' )
	{
		printf("\nSPI0/1 Loopback test with one PDMA channel");
	}
	else if( u8Option == '2' )
	{
		printf("\nSPI0/1 Loopback test with two PDMA channels");
	}
	else
		return;
	
	/* Clear Tx register of SPI1 to avoid send non-zero data to Master. Just for safe. */
	u32Tmp = 0;
	SPI_SetTxRegister(SPI1, &u32Tmp, 1);
	
	/* set the soure data and clear the destination buffer */
	for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
	{
		au32SourceData[u32DataCount] = u32DataCount+1;
		au32DestinationData[u32DataCount] = 0;
	}
	
	PDMA_Init();
	
	if( u8Option == '2' )
	{
		PDMA_EnableChannelClock(PDMAGCR_CSR_CLK2);/* Enable Channel 2 Clock */
		sPDMA.SrcAddr  = (uint32_t)au32SourceData;
	    sPDMA.DestAddr = (uint32_t)&(SPI0->TX0);   
		sPDMA.SrcCtrl  = PDMA_CSR_SAD_INC; 
		sPDMA.DestCtrl = PDMA_CSR_DAD_FIX;   
	    sPDMA.TransWidth = PDMA_CSR_APB_TWS_32BIT;
		sPDMA.Mode = PDMA_CSR_MEM2IP;
		sPDMA.ByteCnt  = TEST_COUNT*4;
		sPDMA.IsTimeOutEnable = 0;
		PDMA_Open(PDMA2, &sPDMA);
		
		PDMA_SPI0_TX_TO_CH2();
	}
	
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK1);/* Enable Channel 1 Clock */
	sPDMA.SrcAddr  = (uint32_t)&(SPI1->RX0);
    sPDMA.DestAddr = (uint32_t)au32DestinationData;   
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_FIX; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_INC;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_32BIT;
	sPDMA.Mode = PDMA_CSR_IP2MEM;
	sPDMA.ByteCnt  = TEST_COUNT*4;
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA1, &sPDMA);
	
	PDMA_SPI1_RX_TO_CH1();
	PDMA_EnableInt(PDMA1, PDMA_IER_TD);
		
	i32Err = 0;
	for(u32TestCycle=0; u32TestCycle<10000; u32TestCycle++)
	{
		if((u32TestCycle&0x1FF) == 0)
			putchar('.');
				
		if( u8Option == '1' )
		{
			/* Enable SPI PDMA and Trigger PDMA specified Channel */
			PDMA_CHEnableTransfer(PDMA1);
		 	SPI_SetRxPDMA(SPI1, TRUE);
									
			/* Enable the SPI0 interrupt and install the callback function. */
			SPI_EnableInt(SPI0);
			
			/* set the GO_BUSY bit of SPI1 */
			SPI_SetGo(SPI1);
			
			g_PdmaIntFlag = 0;
			g_Spi0IntFlag = 0;
			u32DataCount = 0;
			
			/* write the first data of source buffer to Tx register of SPI0. And start transmission. */
			SPI_SingleWrite(SPI0, &au32SourceData[0]);
			while(1)
			{
				if(g_Spi0IntFlag==1)
				{
					g_Spi0IntFlag = 0;
				
					if(u32DataCount<(TEST_COUNT-1))
					{
						/* Trigger next transfer. */
						u32DataCount++;
						SPI_SingleWrite(SPI0, &au32SourceData[u32DataCount]);
					}
					else
						break;
				}
			}
			
			/* Disable the SPI0 interrupt */
			SPI_DisableInt(SPI0);
			
			/* Wait PDMA transfer done */
			while(g_PdmaIntFlag==0);
		}
		else
		{
			g_PdmaIntFlag = 0;
			/* Enable SPI PDMA and Trigger PDMA specified Channel */
			PDMA_CHEnableTransfer(PDMA1);
			SPI_SetRxPDMA(SPI1, TRUE);
			
			/* set the GO_BUSY bit of SPI1 */
			SPI_SetGo(SPI1);
			
			PDMA_CHEnableTransfer(PDMA2);
			SPI_SetTxPDMA(SPI0, TRUE);
			
			/* set the GO_BUSY bit of SPI0 */
			SPI_SetGo(SPI0);
			
			/* Wait PDMA transfer done */
			while(g_PdmaIntFlag==0);
			
		}
		
		/* Compare the transfer data */
		for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
		{
			if(au32DestinationData[u32DataCount]!=au32SourceData[u32DataCount])
				i32Err = 1;
		}

		if(i32Err)
			break;
	}
	
	/* Disable SPI PDMA */
	SPI_SetTxPDMA(SPI0, FALSE);
	SPI_SetRxPDMA(SPI1, FALSE);
	
	/* Close PDMA */
	PDMA_Close();
	PDMA_DeInit();
	
	if(i32Err)
	{	
		printf(" [FAIL]\n\n");
	}
	else
	{
		printf(" [OK]\n\n");
	}
	
	SPI_Close(SPI0);
	SPI_Close(SPI1);

	return;
}

void SpiLoopbackTest_WithFIFO(void)
{    
	uint32_t au32SourceData[TEST_COUNT];
	uint32_t au32DestinationData[TEST_COUNT];
	uint32_t u32DataCount;
	int32_t i32Err;
	uint32_t u32Tmp;
	SPI_DATA_T conf;
	
	/* SPI-0 */
	
	/* Configure SPI0 as a master, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_MASTER;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI0, &conf);	
	
	/* Enable the automatic slave select function of SS0. */
	SPI_EnableAutoSS(SPI0, SPI_SS0);
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI0);
	
	/* SPI clock rate 2MHz */
	SPI_SetClockFreq(SPI0, 2000000, 0);
		
	/* SPI-1 */
	
	/* Configure SPI1 as a slave, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_SLAVE;
	conf.u32Type = SPI_TYPE1;
	conf.i32BitLength = 32;	
	SPI_Open(SPI1, &conf);	
	
	/* Set the trigger mode of slave select pin. */
	SPI_SetLevelTriggerMode(SPI1);   /* level trigger */
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI1);

	showLoopBackTestNote();
	
	printf("\nSPI0 <-> SPI1 Loop Back Test by FIFO ");
		
	/* Clear Tx register of SPI1 to avoid send non-zero data to Master. Just for safe. */
	u32Tmp = 0;
	SPI_SetTxRegister(SPI1, &u32Tmp, 1);
	
	/* set the soure data and clear the destination buffer */
	for(u32DataCount=0; u32DataCount<8; u32DataCount++)
	{
		au32SourceData[u32DataCount] = u32DataCount;
		au32DestinationData[u32DataCount] = 0;
	}	
	
	/* Enable FIFO mode */
	SPI_SetFIFOMode(SPI0,TRUE, 1);
	SPI_SetFIFOMode(SPI1, TRUE, 1);
			
	i32Err = 0;

	/* SPI0 Tx <-> SPI1 Rx */
	SPI_FIFOWrite32(SPI0, au32SourceData, 8);
	SPI_FIFORead32(SPI1, au32DestinationData, 8);
	
	for(u32DataCount=0; u32DataCount<8; u32DataCount++)
	{
		if(au32DestinationData[u32DataCount]!=au32SourceData[u32DataCount])
			i32Err = 1;
	}	

	if(i32Err)
	{	
		printf(" [FAIL]\n\n");
	}
	else
	{
		printf(" [OK]\n\n");
	}
		
	/* Disable FIFO mode */
	SPI_SetFIFOMode(SPI0, FALSE, 3);
	SPI_SetFIFOMode(SPI1, FALSE, 3);
	
	SPI_Close(SPI0);
	SPI_Close(SPI1);
}

void SpiFlashTest(void)
{	
	uint16_t u16Id, i;
	uint8_t u8TxData[8], u8RxData[8];
	
	printf("SPI0 (master) <==> SPI Flash (slave)\n");
	
	/* Init SPI port-0 to communicate with SPI Flash */
	spiFlashInit(SPI0);

	////////////////////////////////
	// Read SPI Flash ID
	// Winbond W25X40: ID=0xef12
	// Winbond W25P16: ID=0xef14
	/////////////////////////////////
	u16Id = spiFlashReadID();
	if(u16Id != 0xEF14)
	{	
		printf("ID error!!(0x%x)\n", u16Id);
		return;
	}
	
	/* Earse whole chip */		
	printf("Earse Chip ..");
	spiFlashChipErase();
	printf("Done ..\n");
		
	/* Prepare data to write to flash */	
	for(i=0;i<8;i++)
		u8TxData[i] = i;
	
	/* Start to write to flash */
	printf("Program Chip ..");
	spiFlashPageProgram(0, 8, &u8TxData[0]);
	printf("Done ..\n");
	
	/* Read it back for comparing */
	printf("Read Data and Compare ..");
	spiFlashReadData(0, 8, &u8RxData[0]);
	for(i=0;i<8;i++)
		if(u8RxData[i] != u8TxData[i])
			printf("\nError at %d, receive 0x%x but should be 0x%x\n", i, u8RxData[i], u8TxData[i]);
	printf("Done ..\n");
	
	/* Close SPI interface */
	SpiFalshClose();
}

void PDMA_Callback(uint32_t u32UserData)
{
	g_PdmaIntFlag = 1;
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
