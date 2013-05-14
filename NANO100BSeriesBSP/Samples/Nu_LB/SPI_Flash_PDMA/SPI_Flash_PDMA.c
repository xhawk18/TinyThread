/******************************************************************************
 * @file     SPI_Flash_PDMA.c
 * @brief    This sample shows the usage of SPI APIs to access flash.
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

#define TEST_NUMBER 1   /* page numbers */
#define	TEST_LENGTH	256	/* length */
uint8_t SrcArray[TEST_LENGTH];
uint8_t DestArray[TEST_LENGTH];

uint32_t volatile PDMA1_INT_Flag;
uint32_t volatile PDMA2_INT_Flag;

void PDMA0_Callback(void);
void PDMA1_Callback(void);

uint8_t SpiFlash_ReadMidDid(void);
void SpiFlash_ChipErase(void);
uint32_t SpiFlash_ReadStatusReg1(void);
uint32_t SpiFlash_ReadStatusReg2(void);
void SpiFlash_WaitReady(void);
void SpiFlash_PageProgram(uint32_t, uint32_t);
void SpiFlash_ReadData(uint32_t, uint32_t);

/* Main */
int main(void)
{	
	STR_PDMA_T sPDMA;  
    uint32_t u32ByteCount, u32FlashAddress, u32PageNumber;
	uint32_t nError = 0;
	SPI_DATA_T conf;
		
	printf("\n+------------------------------------------------------------------------+\n");
    printf("|               Learning Board Sample for SPI Flash with PDMA            |\n");
    printf("+------------------------------------------------------------------------+\n");
    
	MFP_SPI0_TO_PORTE();
	
	SPI_Init(SPI0);
	
	/* Configure SPI0 as a master, 32-bit transaction*/
	conf.u32Mode = SPI_MODE_MASTER;
	conf.u32Type = SPI_TYPE0;
	conf.i32BitLength = 32;	
	SPI_Open(SPI0, &conf);	
	
	/* Diable AutoSS */
	SPI_DisableAutoSS(SPI0);
	
	/* Set the active level of slave select. */
	SPI_SetSlaveSelectActiveLow(SPI0);
	
	/* SPI clock rate 2MHz */
	SPI_SetClockFreq(SPI0, 1000000, 0);
								  
	/* PDMA Init */
    PDMA_Init();
    
	/* -------------------------------------------- */
    /* Configure PDMA Channel 1 to receive SPI0 Rx0 */
	/* -------------------------------------------- */
        
    /* CH1 Setting */
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK1);/* Enable Channel 1 Clock */
	
	sPDMA.SrcAddr  = (uint32_t)&(SPI0->RX0);
    sPDMA.DestAddr = (uint32_t)&DestArray[0]; 
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_FIX; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_INC;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_8BIT;
	sPDMA.Mode = PDMA_CSR_IP2MEM;
	sPDMA.ByteCnt  = TEST_LENGTH;
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA1, &sPDMA);
	
	PDMA_SPI0_RX_TO_CH1();
	  
	/* Enable INT */
    PDMA_EnableInt(PDMA1, PDMA_IER_TD);
	
	/* -------------------------------------------- */
    /* Configure PDMA Channel 2 to receive SPI0 Tx0 */
	/* -------------------------------------------- */
	/* Tx PDMA Setting */
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK2);/* Enable Channel 2 Clock */
	
	sPDMA.SrcAddr  = (uint32_t)&SrcArray[0];
    sPDMA.DestAddr = (uint32_t)&(SPI0->TX0);
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_INC; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_FIX;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_8BIT;
	sPDMA.Mode = PDMA_CSR_MEM2IP;
	sPDMA.ByteCnt  = TEST_LENGTH;
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA2, &sPDMA);
	
	PDMA_SPI0_TX_TO_CH2();
	  
	/* Enable INT */
    PDMA_EnableInt(PDMA2, PDMA_IER_TD);
    
		        
    /* read MID & DID */
	if(SpiFlash_ReadMidDid() == 0)
	{
		printf("Sorry!! The ID is wrong!!\n");
		return 0;
	}
	
	SYS_Delay(1000000);
	
	printf("Earse chip ...\n");
	/* Erase SPI flash */
	SpiFlash_ChipErase();
	
	/* Wait ready */
	SpiFlash_WaitReady();
	
	/* Get the status of SPI flash */
	SYS_Delay(1000000);
	
    /* init source data buffer */
	for(u32ByteCount=0; u32ByteCount<256; u32ByteCount++)
	{
		SrcArray[u32ByteCount] = u32ByteCount;
	} 
	
	printf("Start to write data to Flash ...\n");
	/* Program SPI flash */	
	u32FlashAddress = 0;
	for(u32PageNumber=0; u32PageNumber<TEST_NUMBER; u32PageNumber++)
	{
		/* Trigger PDMA specified Channel */
		PDMA_CHEnableTransfer(PDMA2);
		 
		/* page program */
		SpiFlash_PageProgram(u32FlashAddress, 256);
		SpiFlash_WaitReady();
		u32FlashAddress += 0x100;
	} 
    
	SYS_Delay(1000000);	

	/* clear destination data buffer */
	for(u32ByteCount=0; u32ByteCount<256; u32ByteCount++)
	{
		DestArray[u32ByteCount] = 0;
	} 
	
	printf("Now, we read them back to compare ...\n");
		
	/* Read SPI flash */
	u32FlashAddress = 0;
	for(u32PageNumber=0; u32PageNumber<TEST_NUMBER; u32PageNumber++)
	{
		/* Trigger PDMA specified Channel */
		PDMA_CHEnableTransfer(PDMA1);
		/* page read */
		SpiFlash_ReadData(u32FlashAddress, 256);
		u32FlashAddress += 0x100;
		
		for(u32ByteCount=0; u32ByteCount<256; u32ByteCount++)
		{
		    if(DestArray[u32ByteCount] != SrcArray[u32ByteCount])
		    {
				printf("Data error! [%d] R:0x%x - W:0x%x\n", u32PageNumber, DestArray[u32ByteCount], SrcArray[u32ByteCount]);
				nError ++;
			}
		}	
	}

	if(nError == 0)
		printf("Compare Data OK!!\n");
				
	SPI_Close(SPI0);
	PDMA_Close();
	
	return 1;
}


// **************************************
// For W25Q16BV, Manufacturer ID: 0xEF; Device ID: 0x14
// For W26X16, Manufacturer ID: 0xEF; Device ID: 0x14
uint8_t SpiFlash_ReadMidDid(void)
{
	uint32_t au32SourceData;
    uint32_t au32DestinationData; 	

    // configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8);

	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);
	
	// send Command: 0x90, Read Manufacturer/Device ID
	au32SourceData = 0x90;
	SPI_SingleWrite(SPI0, &au32SourceData); 
    	
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// configure transaction length as 24 bits
	SPI_SetBitLength(SPI0, 24);	

	// send 24-bit '0', dummy
	au32SourceData = 0x0;
    SPI_SingleWrite(SPI0, &au32SourceData); 	

	// wait
	while (SPI_IsBusy(SPI0)) {}
    
	// configure transaction length as 16 bits
	SPI_SetBitLength(SPI0, 16);	

	// receive
	au32SourceData = 0x0;
	SPI_SingleWrite(SPI0, &au32SourceData);
	
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0); 	

	SPI_DumpRxRegister(SPI0, &au32DestinationData, 1);

    if ((au32DestinationData & 0xffff) == 0xEF14) 
	{	
		printf("MID & DID=0xEF14\n");		
		return 1;
	}
	else 
	{
		printf("MID & DID Error(id=0x%x)!\n", au32DestinationData);
		return 0;
	}
  
}

// **************************************
void SpiFlash_ChipErase(void)
{
	
	uint32_t au32SourceData;

	// configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8); 	

	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0); 	

	// send Command: 0x06, Write enable
	au32SourceData = 0x06;
	SPI_SingleWrite(SPI0, &au32SourceData); 	

	// wait
	while (SPI_IsBusy(SPI0)) {}

	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0); 

	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);	

	// send Command: 0xC7, Chip Erase
	au32SourceData = 0xc7;
	SPI_SingleWrite(SPI0, &au32SourceData); 	

	// wait
	while (SPI_IsBusy(SPI0)) {}

	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0); 
}

// **************************************
uint32_t SpiFlash_ReadStatusReg1(void)
{
    uint32_t au32SourceData;
    uint32_t au32DestinationData; 	

	// configure transaction length as 16 bits
	SPI_SetBitLength(SPI0, 16);
		
	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);
	
	// send Command: 0x05, Read status register 1
	au32SourceData = 0x0500;
	SPI_SingleWrite(SPI0, &au32SourceData); 
	
	// wait
	while (SPI_IsBusy(SPI0)) {}
		
	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0);

	// dump Rx register
	SPI_DumpRxRegister(SPI0, &au32DestinationData, 1);

	return (au32DestinationData & 0xFF);
}

// **************************************
uint32_t SpiFlash_ReadStatusReg2(void)
{
	uint32_t au32SourceData;
    uint32_t au32DestinationData; 	
	
	// configure transaction length as 16 bits
	SPI_SetBitLength(SPI0, 16);
	
	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);
	
	// send Command: 0x35, Read status register 2
	au32SourceData = 0x3500;
	SPI_SingleWrite(SPI0, &au32SourceData); 
	
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0);

    // dump Rx register
	SPI_DumpRxRegister(SPI0, &au32DestinationData, 1);

	return (au32DestinationData & 0xFF);
}

// **************************************
void SpiFlash_WaitReady(void)
{
	uint32_t ReturnValue;
	
	do{
		ReturnValue = SpiFlash_ReadStatusReg1();
		ReturnValue = ReturnValue & 1;
	}while(ReturnValue!=0); // check the BUSY bit
	
}

// **************************************
void SpiFlash_PageProgram(uint32_t StartAddress, uint32_t ByteCount)
{
	uint32_t au32SourceData;
	
    // configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8);
	
	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);
	
	// send Command: 0x06, Write enable
	au32SourceData = 0x06;
	SPI_SingleWrite(SPI0, &au32SourceData); 
	
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0);
	
	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);
	
	// send Command: 0x02, Page program
	au32SourceData = 0x02;
	SPI_SingleWrite(SPI0, &au32SourceData); 
	
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// configure transaction length as 24 bits
	SPI_SetBitLength(SPI0, 24);	
	
	// send 24-bit start address
	au32SourceData = StartAddress;
	SPI_SingleWrite(SPI0, &au32SourceData);
		
	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8); 
	
	// enable SPI PDMA 
	SPI_SetTxPDMA(SPI0, TRUE);

	// SPI go
	PDMA2_INT_Flag = 0;
	SPI_SetGo(SPI0);
	
	// wait PDMA1 done
	while (1)
	{
		if (PDMA2_INT_Flag==1)
		{
			PDMA2_INT_Flag = 0;
			break;
		}
	}
	
	while(SPI_IsBusy(SPI0));

	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0);	
}

// **************************************
void SpiFlash_ReadData(uint32_t StartAddress, uint32_t ByteCount)
{
	uint32_t au32SourceData;
    
	// configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8);	

	// /CS: active
	SPI_SetSS(SPI0, SPI_SS0);

	// send Command: 0x03, Read data
	au32SourceData = 0x03;
	SPI_SingleWrite(SPI0, &au32SourceData); 	

	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// configure transaction length as 24 bits
	SPI_SetBitLength(SPI0, 24);	

	// send 24-bit start address
	au32SourceData = StartAddress;
	SPI_SingleWrite(SPI0, &au32SourceData); 	

	// wait
	while (SPI_IsBusy(SPI0)) {}
	
	// configure transaction length as 8 bits
	SPI_SetBitLength(SPI0, 8); 	

  	// enable SPI PDMA 
	SPI_SetRxPDMA(SPI0, TRUE);

	// SPI go
	PDMA1_INT_Flag = 0;
	SPI_SetGo(SPI0);
	
	// wait PDMA0 done	
	while (1)
	{
		if (PDMA1_INT_Flag==1)
		{
			PDMA1_INT_Flag = 0;
			break;
		}
	}
	
	while(SPI_IsBusy(SPI0));
	
	// /CS: de-active
	SPI_ClrSS(SPI0, SPI_SS0); 	

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


