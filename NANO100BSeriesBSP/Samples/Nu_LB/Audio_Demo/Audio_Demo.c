/******************************************************************************
 * @file     Audio_Demo.c
 * @brief    This is a demonstration for playing/recording audio with WAU8822 audio codec.
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
 
#include <stdio.h>
#include <String.h>

#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_i2s.h"
#include "nano1xx_i2c.h"
#include "nano1xx_pdma.h"

#include "config.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

#ifdef I2S_USE_FIFO
uint32_t PcmBuff[BUFF_LEN] = {0};
uint32_t volatile u32BuffPos = 0;
#endif
#ifdef I2S_USE_PDMA
uint32_t volatile PdmaTxBuff[BUFF_LEN] = {0};
uint32_t volatile PdmaRxBuff[BUFF_LEN] = {0};
uint32_t volatile Buff_Tx_Idx = 0;
#endif

uint32_t volatile EndFlag0 = 0;
uint8_t Tx_Data0[2];
uint8_t DataCnt0;

void Delay(int count)
{
	volatile uint32_t i;
	for (i = 0; i < count ; i++);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Write 9-bit data to 7-bit address register of WAU8822 with I2C0  			   						   */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_WriteWAU8822(uint8_t u8addr, uint16_t u16data)
{		
	DataCnt0 = 0;
	EndFlag0 = 0;
	
	Tx_Data0[0] = (uint8_t)((u8addr << 1) | (u16data >> 8));
	Tx_Data0[1] = (uint8_t)(u16data & 0x00FF);

	/* I2C0 as master sends START signal */
	I2C_Ctrl(I2C0, 1, 0, 0);
		
	/* Wait I2C0 Tx Finish */
	while (EndFlag0 == 0);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  WAU8822 Settings with I2C interface									       						       */
/*---------------------------------------------------------------------------------------------------------*/
void WAU8822_Setup()
{
	printf("WAU8822 Setup\n");
	
	I2C_WriteWAU8822(0,  0x000);   /* Reset all registers */ 
	Delay(0x200);

#ifdef INPUT_IS_LIN	//input source is LIN
	I2C_WriteWAU8822(1,  0x02F);        
	I2C_WriteWAU8822(2,  0x1B3);   /* Enable L/R Headphone, ADC Mix/Boost, ADC */
	I2C_WriteWAU8822(3,  0x07F);   /* Enable L/R main mixer, DAC */
	I2C_WriteWAU8822(4,  0x010);   /* 16-bit word length, I2S format, Stereo */
	//I2C_WriteWAU8822(4,  0x013);   /* 16-bit word length, I2S format, Mono, Swap LR */
	I2C_WriteWAU8822(5,  0x000);   /* Companding control and loop back mode (all disable) */
	I2C_WriteWAU8822(6, 0x1AD); 	 /* Divide by 6, 16K */
	I2C_WriteWAU8822(7, 0x006);    /* 16K for internal filter cofficients */
	I2C_WriteWAU8822(10, 0x008);   /* DAC softmute is disabled, DAC oversampling rate is 128x */
	I2C_WriteWAU8822(14, 0x108);   /* ADC HP filter is disabled, ADC oversampling rate is 128x */
	I2C_WriteWAU8822(15, 0x1EF);   /* ADC left digital volume control */
	I2C_WriteWAU8822(16, 0x1EF);   /* ADC right digital volume control */
	
	I2C_WriteWAU8822(44, 0x000);   /* LLIN/RLIN is not connected to PGA */
	I2C_WriteWAU8822(47, 0x050);   /* LLIN connected, and its Gain value */
	I2C_WriteWAU8822(48, 0x050);   /* RLIN connected, and its Gain value */
	I2C_WriteWAU8822(50, 0x001);   /* Left DAC connected to LMIX */
	I2C_WriteWAU8822(51, 0x001);   /* Right DAC connected to RMIX */
#else	//input source is MIC
	I2C_WriteWAU8822(1,  0x03F);        
	I2C_WriteWAU8822(2,  0x1BF);   /* Enable L/R Headphone, ADC Mix/Boost, ADC */
	I2C_WriteWAU8822(3,  0x07F);   /* Enable L/R main mixer, DAC */
	
	//8822 outputs only LEFT data, but I2S engine gets RIGHT data when enable MONO mode//
	//I2C_WriteWAU8822(4,  0x013);   /* 16-bit word length, I2S format, Mono, Swap LR */
	
	I2C_WriteWAU8822(5,  0x000);   /* Companding control and loop back mode (all disable) */
	I2C_WriteWAU8822(6, 0x1AD); 	 /* Divide by 6, 16K */
	I2C_WriteWAU8822(7, 0x006);    /* 16K for internal filter cofficients */
	I2C_WriteWAU8822(10, 0x008);   /* DAC softmute is disabled, DAC oversampling rate is 128x */
	I2C_WriteWAU8822(14, 0x108);   /* ADC HP filter is disabled, ADC oversampling rate is 128x */
	I2C_WriteWAU8822(15, 0x1EF);   /* ADC left digital volume control */
	I2C_WriteWAU8822(16, 0x1EF);   /* ADC right digital volume control */
	I2C_WriteWAU8822(44, 0x033);   /* LMICN/LMICP is connected to PGA */
	I2C_WriteWAU8822(50, 0x001);   /* Left DAC connected to LMIX */
	I2C_WriteWAU8822(51, 0x001);   /* Right DAC connected to RMIX */
#endif
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
#ifdef I2S_USE_FIFO
	uint32_t u32startFlag = 1;
#endif	

#ifdef	I2S_USE_PDMA
	STR_PDMA_T sPDMA;
#endif
	I2S_DATA_T st;	
	

	printf("+------------------------------------------------------------------------+\n");
    printf("|                   I2S Learning Board Audio Sample Code                 |\n");
    printf("+------------------------------------------------------------------------+\n");                    
    printf("  NOTE: This sample code needs to work with WAU8822.\n");                    
	
	MFP_I2S_TO_PORTD();
	
	/* Set multi function pin for I2C0 */
    GCR->PA_H_MFP = GCR->PA_H_MFP & ~(PA8_MFP_MASK|PA9_MFP_MASK) | 
					PA8_MFP_I2C0_SDA | PA9_MFP_I2C0_SCL;
					
	I2C_Init(I2C0);
						
    /* Open I2C0 and set clock = 100Kbps */
    I2C_Open(I2C0, 100000);
    
	/* Enable I2C0 interrupt and set corresponding NVIC bit */
	I2C_EnableInt(I2C0);
	
	I2S_Init();
	st.u32SampleRate 	 = 16000;
    st.u32WordWidth 	 = I2S_DATABIT_16;
#ifdef INPUT_IS_LIN
    st.u32AudioFormat 	 = I2S_STEREO;    
#else
	st.u32AudioFormat 	 = I2S_MONO;
#endif
	st.u32DataFormat  	 = I2S_FORMAT_I2S;
    st.u32Mode 		 	 = I2S_MODE_SLAVE;
    st.u32TxFIFOThreshold = I2S_FIFO_TX_LEVEL_WORD_4;
    st.u32RxFIFOThreshold = I2S_FIFO_RX_LEVEL_WORD_4;
	I2S_Open(&st);
	
	// select source from HXT(12MHz)
	SYS_SelectIPClockSource_2(CLK_CLKSEL2_I2S_MASK, CLK_CLKSEL2_I2S_HXT);
	// div = 0
	SYS_SetClockDivider_0(CLK_CLKDIV0_I2S_MASK, I2S_CLK_DIVIDER(0));
		
	/* Initialize WAU8822 codec */
	WAU8822_Setup();

	/* Set MCLK and enable MCLK */
	I2S_SetMCLKFreq(12000000);
	I2S_EnableMCLK();

#ifndef INPUT_IS_LIN
	I2S_SetMonoSrcChannel(I2S_MONO_LEFT);		//wau8822 will store data in left channel
#endif

#ifdef I2S_USE_FIFO
	/* Enable Rx threshold level interrupt */
	I2S_EnableInt_RxThreshold();
	
	/* Enable I2S Rx function to receive data */
	I2S_EnableRx();

	while(1)
	{
		if (u32startFlag)
		{
			/* Enable I2S Tx function to send data when data in the buffer is more than half of buffer size */
			if (u32BuffPos >= BUFF_LEN/2)
			{
				I2S_EnableInt_TxThreshold();	
				I2S_EnableTx();
				u32startFlag = 0;
			}
		}
	}
#endif

#ifdef I2S_USE_PDMA
	/* PDMA Init */
    PDMA_Init();

	/* CH2: I2S RX */
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK2);/* Enable Channel 2 Clock */
	sPDMA.SrcAddr  = (uint32_t)&(I2S->RXFIFO);
    sPDMA.DestAddr = (uint32_t)&PdmaRxBuff[0];   
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_FIX; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_WRAP;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_32BIT;
	sPDMA.Mode = PDMA_CSR_IP2MEM;
	sPDMA.ByteCnt  = BUFF_LEN*sizeof(uint32_t);
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA2, &sPDMA);
	
	PDMA_I2S_RX_TO_CH2();
	PDMA_EnableInt(PDMA2, PDMA_IER_WRA_BCR_DONE | PDMA_IER_WRA_BCR_HALF);
	
	/* CH1: I2S TX */
	PDMA_EnableChannelClock(PDMAGCR_CSR_CLK1);/* Enable Channel 2 Clock */
	sPDMA.SrcAddr  = (uint32_t)&PdmaTxBuff[0];
    sPDMA.DestAddr = (uint32_t)&(I2S->TXFIFO);   
	sPDMA.SrcCtrl  = PDMA_CSR_SAD_WRAP; 
	sPDMA.DestCtrl = PDMA_CSR_DAD_FIX;   
    sPDMA.TransWidth = PDMA_CSR_APB_TWS_32BIT;
	sPDMA.Mode = PDMA_CSR_MEM2IP;
	sPDMA.ByteCnt  = BUFF_LEN*sizeof(uint32_t);
	sPDMA.IsTimeOutEnable = 0;
	PDMA_Open(PDMA1, &sPDMA);
	
	PDMA_I2S_TX_TO_CH1();
	PDMA_EnableInt(PDMA1, PDMA_IER_WRA_BCR_DONE | PDMA_IER_WRA_BCR_HALF);
	
	/* Start to plar/record */
	I2S_EnableRxDMA();
	I2S_EnableRx();
	PDMA_CHEnableTransfer(PDMA2);

	
	I2S_EnableTxDMA();
	I2S_EnableTx();
	PDMA_CHEnableTransfer(PDMA1);

	while(1);
#endif

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
