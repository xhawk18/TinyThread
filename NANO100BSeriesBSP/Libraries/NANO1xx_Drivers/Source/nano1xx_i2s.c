/******************************************************************************
 * @file     nano1xx_i2s.c
 * @brief    NANO1xx I2S driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
  
//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_i2s.h"
#include "nano1xx_sys.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_I2S_Driver I2S Driver
  @{
*/

/** @addtogroup NANO1xx_I2S_EXPORTED_FUNCTIONS I2S Exported Functions
  @{
*/

/**
  * @brief  This function is used to configure the I2S mode/format/FIFO threshold function/BCLK.
  * @param  sParam: Data structure to configure
  *			u32SampleRate: Sample rate
  *			u8WordWidth: Data length - 	I2S_DATABIT_8  :  8-bit Data
  *										I2S_DATABIT_16 : 16-bit Data
  *										I2S_DATABIT_24 : 24-bit Data
  *										I2S_DATABIT_32 : 32-bit Data
  *          u8AudioFormat: Audio format- I2S_MONO: Mono / I2S_STEREO: Stereo
  *          u8DataFormat: Data format - I2S_FORMAT_I2S : I2S format
  *	               					     I2S_FORMAT_MSB : MSB justified format
  *          u8Mode: I2S operation mode - I2S_MODE_MASTER: master mode
  *                                       I2S_MODE_SLAVE : slave mode
  *          u8TxFIFOThreshold: Tx FIFO Threshold Level - I2S_FIFO_TX_LEVEL_WORD_1 : 1 Word 
  *             					                         I2S_FIFO_TX_LEVEL_WORD_2 : 2 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_3 : 3 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_4 : 4 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_5 : 5 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_6 : 6 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_7 : 7 Word
  *             						                     I2S_FIFO_TX_LEVEL_WORD_8 : 8 Word
  *          u8TxFIFOThreshold: Rx FIFO Threshold Level - I2S_FIFO_RX_LEVEL_WORD_1 : 1 Word
  *              							              I2S_FIFO_RX_LEVEL_WORD_2 : 2 Word
  *														  I2S_FIFO_RX_LEVEL_WORD_3 : 3 Word
  *               							              I2S_FIFO_RX_LEVEL_WORD_4 : 4 Word
  *               							              I2S_FIFO_RX_LEVEL_WORD_5 : 5 Word
  *               							              I2S_FIFO_RX_LEVEL_WORD_6 : 6 Word
  *               							              I2S_FIFO_RX_LEVEL_WORD_7 : 7 Word
  *               							              I2S_FIFO_RX_LEVEL_WORD_8 : 8 Word
  * @retval 0: Success.
  */
int32_t I2S_Open(I2S_DATA_T *sParam)
{
	uint8_t u8Divider;
	uint32_t u32BitRate, u32SrcClk;
	
	GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_I2S;
	GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_I2S;
				
	I2S->CTRL = (sParam->u32WordWidth | sParam->u32AudioFormat | sParam->u32DataFormat | sParam->u32Mode |sParam->u32TxFIFOThreshold | sParam->u32RxFIFOThreshold) ;

	u32SrcClk = I2S_GetSourceClockFreq();	
	
	u32BitRate = sParam->u32SampleRate * (sParam->u32WordWidth + 1) * 16;

	u8Divider = ((u32SrcClk/u32BitRate) >> 1) - 1;
	
	I2S->CLKDIV = (I2S->CLKDIV & ~I2S_CLKDIV_BCLK_DIV_MASK) | (u8Divider << 8);	
	
	I2S->CTRL |= I2S_CON_I2SEN;	
	
	NVIC_EnableIRQ(I2S_IRQn);
	
	return E_SUCCESS;
}

/**
  * @brief  Disable I2S function and I2S clock.
  * @param  None.
  * @retval None.
  */
void I2S_Close(void)
{
	I2S->CTRL &= ~I2S_CON_I2SEN;
	
	NVIC_DisableIRQ(I2S_IRQn);	
}

/**
  * @brief  Get I2S BCLK frequency.
  * @param  None.
  * @retval I2S BCLK frequency.
  */
uint32_t I2S_GetBCLKFreq(void)
{
	uint32_t u32Reg, u32SrcClk;
	
	u32SrcClk = I2S_GetSourceClockFreq(); 			
	u32Reg = (I2S->CLKDIV & I2S_CLKDIV_BCLK_DIV_MASK) >> 8;
	
	return ((u32SrcClk >> 1) / (u32Reg + 1));
}

/**
  * @brief  Set bit clock divider if I2S operates in master mode. BCLK = I2S_CLK / (2x(divider+1)).
  * @param  u32Bclk: I2S bit clock ftequency (Hz).
  * @retval None.
  */
void I2S_SetBCLKFreq(uint32_t u32Bclk)
{
	uint8_t u8Divider;
	uint32_t u32SrcClk;

	u32SrcClk = I2S_GetSourceClockFreq(); 	
	u8Divider = ((u32SrcClk/u32Bclk) >> 1) - 1;

	I2S->CLKDIV |= ((u8Divider << 8) & I2S_CLKDIV_BCLK_DIV_MASK);
}

/**
  * @brief  Get I2S MCLK frequency.
  * @param  None.
  * @retval I2S MCLK frequency.
  */
uint32_t I2S_GetMCLKFreq(void)
{
	uint32_t u32Reg, u32SrcClk;

	u32Reg = I2S->CLKDIV & I2S_CLKDIV_MCLK_DIV_MASK;

	u32SrcClk = I2S_GetSourceClockFreq(); 
	
	if (u32Reg == 0)
		return u32SrcClk;
	else
		return ((u32SrcClk >> 1) / u32Reg);
}

/**
  * @brief  Set master clock divider. MCLK = I2S_CLK / (2xdivider).
  * @param  u32Mclk: I2S master clock output ftequency (Hz).
  * @retval None.
  */
void I2S_SetMCLKFreq(uint32_t u32Mclk)
{
	uint8_t u8Divider;
	uint32_t u32SrcClk;

	u32SrcClk = I2S_GetSourceClockFreq(); 
	if (u32Mclk == u32SrcClk)
		u8Divider = 0;
	else
		u8Divider = (u32SrcClk/u32Mclk) >> 1;
	
	I2S->CLKDIV |= (u8Divider & I2S_CLKDIV_MCLK_DIV_MASK);
}

/**
  * @brief  This function is used to get I2S source clock frequency.
  * @param  None
  * @retval I2S source clock frequency (Hz).
  */
uint32_t I2S_GetSourceClockFreq(void)
{	
	uint32_t u32Freq, u32Div, u32ClkSrcSel;;
	
	// get I2S selection clock source
	u32ClkSrcSel = CLK->CLKSEL2 & CLK_CLKSEL2_I2S_MASK;
	
	switch (u32ClkSrcSel)
	{
		case CLK_CLKSEL2_I2S_HXT:
			u32Freq = SYS_GetExtClockFreq();
			break;
		
		case CLK_CLKSEL2_I2S_PLL:
			u32Freq = SYS_GetPLLClockFreq();					
			break;
		
		case CLK_CLKSEL2_I2S_HIRC:
			u32Freq = __IRC12M;
			break;	
		
		default: 
			u32Freq = __IRC12M;
			break;	
	}
	
	u32Div = (CLK->CLKDIV0 & CLK_CLKDIV0_I2S_MASK) >> 12;
	u32Div++;
	u32Freq /= u32Div;	

	return u32Freq;
}


/*@}*/ /* end of group NANO1xx_I2S_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_I2S_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
