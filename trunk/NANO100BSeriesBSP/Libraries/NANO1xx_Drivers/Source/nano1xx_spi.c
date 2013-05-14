/******************************************************************************
 * @file     nano1xx_spi.c
 * @brief    NANO1xx SPI driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "nano1xx_sys.h"
#include "nano1xx_spi.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SPI_Driver SPI Driver
  @{
*/


/** @addtogroup NANO1xx_SPI_EXPORTED_FUNCTIONS SPI Exported Functions
  @{
*/

/**
  * @brief  Enable SPI engine clock.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
  
void SPI_Init(SPI_TypeDef *SpiPort)
{	
	if((uint32_t)SpiPort == SPI0_BASE)
		CLK->APBCLK |= CLK_APBCLK_SPI0_EN;		
	else if((uint32_t)SpiPort == SPI1_BASE)
		CLK->APBCLK |= CLK_APBCLK_SPI1_EN;	
	else if((uint32_t)SpiPort == SPI2_BASE)		
		CLK->APBCLK |= CLK_APBCLK_SPI2_EN;	
}

/**
  * @brief  Disable SPI engine clock.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
void SPI_DeInit(SPI_TypeDef *SpiPort)
{	
	if((uint32_t)SpiPort == SPI0_BASE)
		CLK->APBCLK &= ~CLK_APBCLK_SPI0_EN;
	else if((uint32_t)SpiPort == SPI1_BASE)
		CLK->APBCLK &= ~CLK_APBCLK_SPI1_EN;
	else if((uint32_t)SpiPort == SPI2_BASE)
		CLK->APBCLK &= ~CLK_APBCLK_SPI2_EN;	
}

/**
  * @brief  Configure the operation mode, transfer type and bit length of a 
  *         transaction of the specified SPI port.
  * @param  SpiPort: SPI port handler
  *	@param  sParam: Pointer of SPI arguments structure, including
  *    eMode: Specify the operation mode (SPI_MASTER/SPI_SLAVE).
  *	   eType: Specify the transfer type (SPI_TYPE0 ~ SPI_TYPE2).
  *	   i32BitLength: Specify the bit length in a transaction (8~32)
  * @retval SPI_ERR_BUSY: The specified SPI port is in busy status.
  *			E_SUCCESS: Success.
  */
  
/*
SPI_TYPE0:          
             _________________________________
    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |-----


SPI_TYPE1:          
             _________________________________
    CS    --|          Active state           |---
               _   _   _   _   _   _   _   _  
    CLK   ____| |_| |_| |_| |_| |_| |_| |_| |_____
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |---


SPI_TYPE2:
             _________________________________
    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----
          
    Rx    ----| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |----


SPI_TYPE3:
             _________________________________
    CS    --|          Active state           |---
           ___   _   _   _   _   _   _   _   ______ 
    CLK       |_| |_| |_| |_| |_| |_| |_| |_|  
              
    Tx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
          
    Rx    --| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |------
*/
int32_t SPI_Open(SPI_TypeDef *SpiPort, SPI_DATA_T *sParam)
{
	int32_t i32TimeOut;
	            
    /* Bit length 8 ~ 32 */
    assert_param(SPI_CHECK_BIT_LENGTH_0);
    	
	if((uint32_t)SpiPort == SPI0_BASE)
	{		
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_SPI0;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_SPI0;			
	}	
	else if((uint32_t)SpiPort == SPI1_BASE)
	{	
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_SPI1;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_SPI1;		
	}	
	else if((uint32_t)SpiPort == SPI2_BASE)
	{	
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_SPI2;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_SPI2;		
	}
	else	
		return 0;
			
	/* Check busy*/
	i32TimeOut = 0x10000;
	while(SpiPort->CTL & SPI_CTL_GO_BUSY)
	{
		if(i32TimeOut-- <= 0)
			return SPI_ERR_BUSY;
	}
	
	/* "i32BitLength = 0" means 32 bits */
	if(sParam->i32BitLength == 32)
		sParam->i32BitLength = 0;
		
	SpiPort->CTL = (SpiPort->CTL & ~SPI_CTL_TX_BIT_LEN_MASK) | (sParam->i32BitLength << 3);
	
	SpiPort->CTL = (SpiPort->CTL & ~0x40000) | sParam->u32Mode;

	/* Default to automatic slave select and it is low active */
	SpiPort->SSR |= SPI_SSR_AUTOSS;
	
	/* Transition types */	
	SpiPort->CTL = (SpiPort->CTL & ~0x806) | sParam->u32Type;
	
    return E_SUCCESS;
}

/**
  * @brief  Close the specified SPI module and disable the SPI interrupt.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */  
void SPI_Close(SPI_TypeDef *SpiPort)
{
	if((uint32_t)SpiPort == SPI0_BASE)
		NVIC_DisableIRQ(SPI0_IRQn);	
	else if((uint32_t)SpiPort == SPI1_BASE)
		NVIC_DisableIRQ(SPI1_IRQn);		
	else if((uint32_t)SpiPort == SPI2_BASE)
		NVIC_DisableIRQ(SPI2_IRQn);			
}

/**
  * @brief  Set the bit length of SPI transfer.
  * @param  SpiPort: SPI port handler
  *	@param  i32BitLength: Specify the bit length (8~32 bits).
  * @retval None.  
  */
void SPI_SetBitLength(SPI_TypeDef *SpiPort, int32_t i32BitLength)
{
	assert_param(SPI_CHECK_BIT_LENGTH_1);
   
    if(i32BitLength == 32)
        i32BitLength = 0;

	SpiPort->CTL = (SpiPort->CTL & ~SPI_CTL_TX_BIT_LEN_MASK) | (i32BitLength << 3);
}

/**
  * @brief  Set the number of clock cycle of the suspend interval.  
  * @param  SpiPort: SPI port handler
  *	@param  i32Interval: If the Byte Suspend function is enabled, it specified the delay clocks 
  *                      among each byte. It could be 2~17. 
  * @retval None.   
  */
void SPI_SetSuspendCycle(SPI_TypeDef *SpiPort, int32_t i32Interval)
{
    if(SpiPort->CTL & SPI_CTL_FIFOM) 
    	SpiPort->CTL = (SpiPort->CTL & ~SPI_CTL_SP_CYCLE) | (i32Interval << 12);
    else 
    {    
    	assert_param(SPI_CHECK_BYTE_SUS);
		SpiPort->CTL = (SpiPort->CTL & ~SPI_CTL_SP_CYCLE) | ((i32Interval-2) << 12);
	}
}

/**
  * @brief  Configure the SPI clock.
  * @param  SpiPort: SPI port handler  
  *	@param  u32Clock1: Specify the SPI clock rate in Hz. It's the clock rate of SPI base clock or variable clock 1.
  *	@param  u32Clock2: Specify the SPI clock rate in Hz. It's the clock rate of variable clock 2.
  * @retval The actual clock rate of SPI engine clock 1 is returned.
  *			The actual clock may be different to the target SPI clock due to hardware limitation.
  */
uint32_t SPI_SetClockFreq(SPI_TypeDef *SpiPort, uint32_t u32Clock1, uint32_t u32Clock2)
{
    uint32_t u32Div;
    uint32_t u32Pclk, u32CalculatedSpeed=0;
    
    u32Pclk = SPI_GetSourceClockFreq(SpiPort);
    
	if(u32Clock2!=0)
	{
		u32Div = (((u32Pclk / u32Clock2) + 1) >> 1) - 1;
		if(u32Div > 0xFF)
		    u32Div = 0xFF;
		
		u32CalculatedSpeed = u32Pclk / (2*(u32Div+1));
		if(u32CalculatedSpeed > u32Clock2)
			u32Div ++;			
		
		SpiPort->CLKDIV = (SpiPort->CLKDIV & ~SPI_CLKDIV_DIVIDER2_MASK) | (u32Div << 16);
	}
	else
		SpiPort->CLKDIV |= SPI_CLKDIV_DIVIDER2_MASK;
	
	if(u32Clock1!=0)
	{
		u32Div = ((u32Pclk / u32Clock1) >> 1);
		if(u32Div > 0x7F)
		    u32Div = 0x7F;
		
		if(u32Div == 0)
			u32CalculatedSpeed = u32Pclk;
		else
			u32CalculatedSpeed = u32Pclk / (u32Div << 1);
		
		if(u32CalculatedSpeed > u32Clock1)
		{	
			u32Div ++;
			u32CalculatedSpeed = u32Pclk / (u32Div << 1);
		}
		
		SpiPort->CLKDIV = (SpiPort->CLKDIV & ~SPI_CLKDIV_DIVIDER1_MASK) | u32Div;
	}
	else
		SpiPort->CLKDIV |= SPI_CLKDIV_DIVIDER1_MASK;
	
	return u32CalculatedSpeed;
}

/**
  * @brief  Get the SPI engine clock rate in Hz.
  * @param  SpiPort: SPI port handler  
  * @retval The current SPI bus clock frequency in Hz.
  */
uint32_t SPI_GetClock1Freq(SPI_TypeDef *SpiPort)
{
	uint32_t u32Div;
	uint32_t u32ApbClock;

	u32ApbClock = SPI_GetSourceClockFreq(SpiPort);
	u32Div = SpiPort->CLKDIV & SPI_CLKDIV_DIVIDER1_MASK;
	
	if(u32Div == 0)	return u32ApbClock;
	else return ((u32ApbClock >> 1) / u32Div);   /* SPI_CLK = APB_CLK / (Divider * 2) */
}

/**
  * @brief  Get the clock rate of variable clock 2 in Hz.
  * @param  SpiPort: SPI port handler  
  * @retval The frequency of variable clock 2 in Hz.
  */
uint32_t SPI_GetClock2Freq(SPI_TypeDef *SpiPort)
{
	uint32_t u32Div;
	uint32_t u32ApbClock;

	u32ApbClock = SPI_GetSourceClockFreq(SpiPort);
	u32Div = (SpiPort->CLKDIV & SPI_CLKDIV_DIVIDER2_MASK) >> 16;
	return ((u32ApbClock >> 1) / (u32Div + 1));   /* SPI_CLK = APB_CLK / ((Divider + 1) * 2) */
}

/**
  * @brief  Enable the SPI interrupt of the specified SPI port and install the callback function.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
void SPI_EnableInt(SPI_TypeDef *SpiPort)
{	
	SpiPort->CTL |= SPI_CTL_INTEN;
	
	if((uint32_t)SpiPort == SPI0_BASE)	
		NVIC_EnableIRQ(SPI0_IRQn);
	else if((uint32_t)SpiPort == SPI1_BASE)	
		NVIC_EnableIRQ(SPI1_IRQn);
	else if((uint32_t)SpiPort == SPI2_BASE)		
		NVIC_EnableIRQ(SPI2_IRQn);
}	

/**
  * @brief  Disable the SPI interrupt.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
void SPI_DisableInt(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL &= ~SPI_CTL_INTEN;

	if((uint32_t)SpiPort == SPI0_BASE)	
		NVIC_DisableIRQ(SPI0_IRQn);
	else if((uint32_t)SpiPort == SPI1_BASE)	
		NVIC_DisableIRQ(SPI1_IRQn);
	else if((uint32_t)SpiPort == SPI2_BASE)		
		NVIC_DisableIRQ(SPI2_IRQn);
}

/**
  * @brief  Read data from SPI Rx registers and trigger SPI for next transfer.
  * @param  SpiPort: SPI port handler
  *	@param  pu32Data:  Data got from the SPI bus.
  * @retval TRUE:  The data stored in pu32Data is valid.
  *	@retval FALSE: The data stored in pu32Data is invalid.
  */
uint8_t SPI_SingleRead(SPI_TypeDef *SpiPort, uint32_t *pu32Data)
{
	if(SpiPort->CTL & SPI_CTL_GO_BUSY)
		return FALSE;

	*pu32Data = SpiPort->RX0;
	SpiPort->CTL |= SPI_CTL_GO_BUSY;
	return TRUE;
}

/**
  * @brief  Write data to SPI bus and trigger SPI to start transfer.
  * @param  SpiPort: SPI port handler
  *	@param  pu32Data:  Data will be put to SPI bus.
  * @retval TRUE:  The data stored in pu32Data has been transferred.
  *	@retval  FALSE: The SPI is in busy. The data stored in pu32Data has not been transferred.
  */
uint8_t SPI_SingleWrite(SPI_TypeDef *SpiPort, uint32_t *pu32Data)
{
	if(SpiPort->CTL & SPI_CTL_GO_BUSY)
		return FALSE;

	SpiPort->TX0 = *pu32Data;
	SpiPort->CTL |= SPI_CTL_GO_BUSY;
	return TRUE;
}

/**
  * @brief  Read data from Rx registers. This function will not trigger another data transfer.
  * @param  SpiPort: SPI port handler
  *	@param  pu32Buf:  Store the data got from Rx registers.
  *	@param  u32DataCount: The count of data read from Rx registers.
  * @retval The count of data actually read from Rx registers.
  */
uint32_t SPI_DumpRxRegister(SPI_TypeDef *SpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
	assert_param(SPI_CHECK_SET_DATAREG_NUM);
	
	pu32Buf[0] = SpiPort->RX0;
	
	if(u32DataCount == 2)
		pu32Buf[1] = SpiPort->RX1;
	
    return u32DataCount;
}

/**
  * @brief  Write data to Tx registers. This function will not trigger another data transfer.
  * @param  SpiPort: SPI port handler
  *	@param  pu32Buf:  Store the data which will be written to Tx registers.
  *	@param	u32DataCount: The count of data write to Tx registers.
  * @retval The count of data actually written to Tx registers.
  */
uint32_t SPI_SetTxRegister(SPI_TypeDef *SpiPort, uint32_t *pu32Buf, uint32_t u32DataCount)
{
	assert_param(SPI_CHECK_SET_DATAREG_NUM);
	
	SpiPort->TX0 = pu32Buf[0];
	
	if(u32DataCount == 2)
		SpiPort->TX1 = pu32Buf[1];
		
    return u32DataCount;
}

/**
  * @brief  Configure the DMA Tx settings.
  * @param  SpiPort: SPI port handler
  *	@param  bEnable: TRUE/FALSE  -- Enable/Disable DMA
  * @retval None.
  */
void SPI_SetTxPDMA(SPI_TypeDef *SpiPort, uint8_t bEnable)
{
	if(bEnable)
		SpiPort->PDMA |= SPI_PDMA_TX_DMA_EN;
	else
		SpiPort->PDMA &= ~SPI_PDMA_TX_DMA_EN;
}

/**
  * @brief  Configure the DMA Rx settings.
  * @param  SpiPort: SPI port handler
  *	@param  bEnable: TRUE/FALSE  -- Enable/Disable DMA
  * @retval None.
  */
void SPI_SetRxPDMA(SPI_TypeDef *SpiPort, uint8_t bEnable)
{
	if(bEnable)
		SpiPort->PDMA |= SPI_PDMA_RX_DMA_EN;
	else
		SpiPort->PDMA &= ~SPI_PDMA_RX_DMA_EN;
}

/**
  * @brief  Enable the 3-wire SPI start interrupt of SPI and install the callback function.
  * @param  SpiPort: SPI port handler 
  * @retval None.
  */
void SPI_Enable3WireStartInt(SPI_TypeDef *SpiPort)
{ 
    SpiPort->SSR |= SPI_SSR_SSTA_INTEN;

    if((uint32_t)SpiPort == SPI0_BASE)	
	{
		NVIC_EnableIRQ(SPI0_IRQn);
		NVIC_SetPriority(SPI0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	}
	else if((uint32_t)SpiPort == SPI1_BASE)	
	{	
		NVIC_EnableIRQ(SPI1_IRQn);
		NVIC_SetPriority(SPI1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	}
	else if((uint32_t)SpiPort == SPI2_BASE)
	{	
		NVIC_EnableIRQ(SPI2_IRQn);
		NVIC_SetPriority(SPI2_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
	}
}   

/**
  * @brief  Disable the 3-wire SPI start interrupt.
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
void SPI_Disable3WireStartInt(SPI_TypeDef *SpiPort)
{    
    SpiPort->SSR &= ~SPI_SSR_SSTA_INTEN;
    
    if((uint32_t)SpiPort == SPI0_BASE)	
		NVIC_DisableIRQ(SPI0_IRQn);
	else if((uint32_t)SpiPort == SPI1_BASE)	
		NVIC_DisableIRQ(SPI1_IRQn);
	else if((uint32_t)SpiPort == SPI2_BASE)		
		NVIC_DisableIRQ(SPI2_IRQn);   
}

/**
  * @brief  Configure SPI wakeup function.
  * @param  SpiPort: SPI port handler
  *	@param  bEnable: TRUE/FALSE  -- Enable/Disable SPI wakeup function.
  * @retval None.
  */
void SPI_WakeupConfigure(SPI_TypeDef *SpiPort, uint8_t bEnable)
{
	if(bEnable)
		SpiPort->CTL |= SPI_CTL_WKEUP_EN;
	else
		SpiPort->CTL &= ~SPI_CTL_WKEUP_EN;
}

/**
  * @brief  Enable/disable FIFO mode. If the caller enables FIFO mode, configure the setting of suspend cycle.
  * @param  SpiPort: SPI port handler
  * @param  bEnable: Enable (TRUE) / Disable (FALSE)
  * @param  i32Interval: In FIFO mode, it could be 2~15 and 0. 0 indicates the maximum suspend interval; 2
  *                      indicates the minimum suspend interval.                                           
  * @retval None.
  */
void SPI_SetFIFOMode(SPI_TypeDef *SpiPort, uint8_t bEnable, int32_t i32Interval)
{    
    SpiPort->CTL = (SpiPort->CTL & ~SPI_CTL_SP_CYCLE) | (i32Interval << 12);
    if(bEnable)
    	SpiPort->CTL |= SPI_CTL_FIFOM;
    else
    	SpiPort->CTL &= ~SPI_CTL_FIFOM;
}

/**
  * @brief  Use FIFO mode to write 8bit data only (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu8TxBuf: Specify the buffer to send the data using FIFO.
  *	@param  u32WriteLength: Specify the number of data.
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFOWrite8(SPI_TypeDef *SpiPort, uint8_t *pu8TxBuf, uint32_t u32WriteLength)
{
	uint32_t  u32Count, u32delayno;
	 
	for (u32Count=0; u32Count<u32WriteLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* Wait Tx is empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        SpiPort->TX0 = pu8TxBuf[u32Count];
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to write 16bit data only (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu16TxBuf: Specify the buffer to send the data using FIFO.
  *	@param  u32WriteLength: Specify the number of data.
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFOWrite16(SPI_TypeDef *SpiPort, uint16_t *pu16TxBuf, uint32_t u32WriteLength)
{
	uint32_t  u32Count, u32delayno;
	 
	for (u32Count=0; u32Count<u32WriteLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* Wait Tx is empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        SpiPort->TX0 = pu16TxBuf[u32Count];
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to write 32bit data only (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu32TxBuf: Specify the buffer to send the data using FIFO.
  *	@param  u32WriteLength: Specify the number of data.
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFOWrite32(SPI_TypeDef *SpiPort, uint32_t *pu32TxBuf, uint32_t u32WriteLength)
{
	uint32_t  u32Count, u32delayno;
	 
	for (u32Count=0; u32Count<u32WriteLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* Wait Tx is empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        SpiPort->TX0 = pu32TxBuf[u32Count];
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to read 8bit data only (usually for slave).
  * @param  SpiPort: SPI port handler
  * @param  pu8RxBuf: Specify the buffer to receive the data using FIFO.
  *	@param  u32ReadLength: Specify the word number of data to read.
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFORead8(SPI_TypeDef *SpiPort, uint8_t *pu8RxBuf, uint32_t u32ReadLength)
{
	uint32_t  u32Count, u32delayno;
		 	
	for (u32Count=0; u32Count<u32ReadLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_RX_EMPTY)  /* Wait Rx is not empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        pu8RxBuf[u32Count] = SpiPort->RX0;
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to read 16bit data only (usually for slave).
  * @param  SpiPort: SPI port handler
  * @param  pu16RxBuf: Specify the buffer to receive the data using FIFO.
  *	@param  u32ReadLength: Specify the number of data to read
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFORead16(SPI_TypeDef *SpiPort, uint16_t *pu16RxBuf, uint32_t u32ReadLength)
{
	uint32_t  u32Count, u32delayno;
		 	
	for (u32Count=0; u32Count<u32ReadLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_RX_EMPTY)  /* Wait Rx is not empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        pu16RxBuf[u32Count] = SpiPort->RX0;
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to read 32bit data only (usually for slave).
  * @param  SpiPort: SPI port handler
  * @param  pu32RxBuf: Specify the buffer to receive the data using FIFO.
  *	@param  u32ReadLength: Specify the number of data to read.
  * @retval E_SUCCESS: Success.
  * @retval SPI_ERR_TIMEOUT: timeout.
  */
int32_t SPI_FIFORead32(SPI_TypeDef *SpiPort, uint32_t *pu32RxBuf, uint32_t u32ReadLength)
{
	uint32_t  u32Count, u32delayno;
		 	
	for (u32Count=0; u32Count<u32ReadLength; u32Count++)
    {
    	u32delayno = 0;
	   	while (SpiPort->STATUS & SPI_STATUS_RX_EMPTY)  /* Wait Rx is not empty and Time-out manner */
        {
       	    u32delayno++;
       	    if ( u32delayno >= 0x40000000 )     	    
       	       return SPI_ERR_TIMEOUT;   
				   
        }
        pu32RxBuf[u32Count] = SpiPort->RX0;
    }
    
    return E_SUCCESS;
}

/**
  * @brief  Use FIFO mode to read/write 8bit data at the same time  (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu8TxBuf: Specify the buffer to transmit the data using FIFO. 
  *                   NULL: receive data only.
  *					  Otherwise: both send and receive data.
  * @param  pu8RxBuf: Specify the buffer to receive the data using FIFO.    
  *	@param  u32Length: Specify the number of data.
  * @retval None.  
  */  
void SPI_FIFOReadWrite8(SPI_TypeDef *SpiPort, uint8_t *pu8TxBuf, uint8_t *pu8RxBuf, uint32_t u32Length)
{
	uint32_t  u32Count, u32RxCount = 0;
	 
	for (u32Count=0; u32Count<u32Length; u32Count++)
    {    	
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* if Tx is full, process Rx */
        {
       	    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
   	    		pu8RxBuf[u32RxCount++] = SpiPort->RX0;				   
        }
        
        if(pu8TxBuf == NULL)
        	SpiPort->TX0 = 0xFFFFFFFF;
        else
        	SpiPort->TX0 = pu8TxBuf[u32Count];

        if(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))	/* check if rx has something */
    	{
    		while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    			pu8RxBuf[u32RxCount++] = SpiPort->RX0;
    	}
    }
    
    // SPI is still in progress, check Rx if something is in FIFO 
    while(SpiPort->CTL & SPI_CTL_GO_BUSY)
    {
    	while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    		pu8RxBuf[u32RxCount++] = SpiPort->RX0;
    }
    
    // SPI is finisned, but still need to check FIFO status
    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    	pu8RxBuf[u32RxCount++] = SpiPort->RX0;
}

/**
  * @brief  Use FIFO mode to read/write 16bit data at the same time  (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu16TxBuf: Specify the buffer to transmit the data using FIFO. 
  *                   NULL: receive data only.
  *					  Otherwise: both send and receive data.
  * @param  pu16RxBuf: Specify the buffer to receive the data using FIFO.    
  *	@param  u32Length: Specify the number of data.
  * @retval None.  
  */  
void SPI_FIFOReadWrite16(SPI_TypeDef *SpiPort, uint16_t *pu16TxBuf, uint32_t *pu16RxBuf, uint32_t u32Length)
{
	uint32_t  u32Count, u32RxCount = 0;
	 
	for (u32Count=0; u32Count<u32Length; u32Count++)
    {    	
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* if Tx is full, process Rx */
        {
       	    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
   	    		pu16RxBuf[u32RxCount++] = SpiPort->RX0;				   
        }
        
        if(pu16TxBuf == NULL)
        	SpiPort->TX0 = 0xFFFFFFFF;
        else
        	SpiPort->TX0 = pu16TxBuf[u32Count];

        if(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))	/* check if rx has something */
    	{
    		while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    			pu16RxBuf[u32RxCount++] = SpiPort->RX0;
    	}
    }
    
    // SPI is still in progress, check Rx if something is in FIFO 
    while(SpiPort->CTL & SPI_CTL_GO_BUSY)
    {
    	while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    		pu16RxBuf[u32RxCount++] = SpiPort->RX0;
    }
    
    // SPI is finisned, but still need to check FIFO status
    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    	pu16RxBuf[u32RxCount++] = SpiPort->RX0;
}

/**
  * @brief  Use FIFO mode to read/write 32bit data at the same time  (usually for master).
  * @param  SpiPort: SPI port handler
  * @param  pu32TxBuf: Specify the buffer to transmit the data using FIFO. 
  *                   NULL: receive data only.
  *					  Otherwise: both send and receive data.
  * @param  pu32RxBuf: Specify the buffer to receive the data using FIFO.    
  *	@param	u32Length: Specify the number of data.
  * @retval None.  
  */  
void SPI_FIFOReadWrite32(SPI_TypeDef *SpiPort, uint32_t *pu32TxBuf, uint32_t *pu32RxBuf, uint32_t u32Length)
{
	uint32_t  u32Count, u32RxCount = 0;
	 
	for (u32Count=0; u32Count<u32Length; u32Count++)
    {    	
	   	while (SpiPort->STATUS & SPI_STATUS_TX_FULL)  /* if Tx is full, process Rx */
        {
       	    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
   	    		pu32RxBuf[u32RxCount++] = SpiPort->RX0;				   
        }
        
        if(pu32TxBuf == NULL)
        	SpiPort->TX0 = 0xFFFFFFFF;
        else
        	SpiPort->TX0 = pu32TxBuf[u32Count];

        if(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))	/* check if rx has something */
    	{
    		while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    			pu32RxBuf[u32RxCount++] = SpiPort->RX0;
    	}
    }
    
    // SPI is still in progress, check Rx if something is in FIFO 
    while(SpiPort->CTL & SPI_CTL_GO_BUSY)
    {
    	while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    		pu32RxBuf[u32RxCount++] = SpiPort->RX0;
    }
    
    // SPI is finisned, but still need to check FIFO status
    while(!(SpiPort->STATUS & SPI_STATUS_RX_EMPTY))
    	pu32RxBuf[u32RxCount++] = SpiPort->RX0;
}

/**
  * @brief  This function is used to get SPI source clock frequency.
  * @param  SpiPort	Select the SPI port to get source clock frequency
  * @retval SPI source clock frequency (Hz).
  */
uint32_t SPI_GetSourceClockFreq(SPI_TypeDef *SpiPort)
{	
	//	Clock Source is form:
	//  (PLL, HXT, HIRC, LXT, LIRC) --> [HCLK_Div] ---> HCLK --->
	//    |                                                      [SPI_Div] ====> SPI clock
	//    +--------------------------------------------PLL------>
	//
	uint32_t u32Freq = 12000000, u32ClkSrcSel;
	
	if((uint32_t)SpiPort == SPI0_BASE)
		u32ClkSrcSel = CLK->CLKSEL2 & CLK_CLKSEL2_SPI0_MASK;
	else if((uint32_t)SpiPort == SPI1_BASE)
		u32ClkSrcSel = CLK->CLKSEL2 & CLK_CLKSEL2_SPI1_MASK;
	else 
		u32ClkSrcSel = CLK->CLKSEL2 & CLK_CLKSEL2_SPI2_MASK;
		
		
	if((u32ClkSrcSel == CLK_CLKSEL2_SPI0_PLL)  ||
		(u32ClkSrcSel == CLK_CLKSEL2_SPI1_PLL) ||
		(u32ClkSrcSel == CLK_CLKSEL2_SPI2_PLL))
	{
		return SYS_GetPLLClockFreq();
	}
	
	if((u32ClkSrcSel == CLK_CLKSEL2_SPI0_HCLK)  ||
		(u32ClkSrcSel == CLK_CLKSEL2_SPI1_HCLK) ||
		(u32ClkSrcSel == CLK_CLKSEL2_SPI2_HCLK))
	{
		return SYS_GetHCLKFreq();
	}
	
	return u32Freq;
}

/*@}*/ /* end of group NANO1xx_SPI_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_SPI_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
