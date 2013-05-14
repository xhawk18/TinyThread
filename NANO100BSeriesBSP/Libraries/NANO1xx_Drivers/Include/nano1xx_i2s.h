/******************************************************************************
 * @file     nano1xx_i2s.h
 * @brief    NANO1xx I2S driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "nano1xx.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_I2S_Driver I2S Driver
  @{
*/

/** @addtogroup NANO1xx_I2S_EXPORTED_CONSTANTS I2S Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Define parameter                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
#define I2S_DATABIT_8			I2S_CON_WORDWIDTH_8BIT
#define I2S_DATABIT_16			I2S_CON_WORDWIDTH_16BIT
#define I2S_DATABIT_24			I2S_CON_WORDWIDTH_24BIT
#define I2S_DATABIT_32			I2S_CON_WORDWIDTH_32BIT

/* Audio Format	*/
#define I2S_MONO				I2S_CON_MONO
#define I2S_STEREO				0

/* I2S Data Format */
#define I2S_FORMAT_MSB 			I2S_CON_FORMAT
#define I2S_FORMAT_I2S			0

/* I2S Operation mode */
#define I2S_MODE_SLAVE			I2S_CON_SLAVE
#define I2S_MODE_MASTER			0

/* I2S FIFO Threshold */
#define I2S_FIFO_TX_LEVEL_WORD_1	I2S_CON_TXTH_1_WORD
#define I2S_FIFO_TX_LEVEL_WORD_2	I2S_CON_TXTH_2_WORD
#define I2S_FIFO_TX_LEVEL_WORD_3	I2S_CON_TXTH_3_WORD
#define I2S_FIFO_TX_LEVEL_WORD_4	I2S_CON_TXTH_4_WORD
#define I2S_FIFO_TX_LEVEL_WORD_5	I2S_CON_TXTH_5_WORD
#define I2S_FIFO_TX_LEVEL_WORD_6	I2S_CON_TXTH_6_WORD
#define I2S_FIFO_TX_LEVEL_WORD_7	I2S_CON_TXTH_7_WORD
#define I2S_FIFO_TX_LEVEL_WORD_8	I2S_CON_TXTH_8_WORD

#define I2S_FIFO_RX_LEVEL_WORD_1	I2S_CON_RXTH_1_WORD
#define I2S_FIFO_RX_LEVEL_WORD_2	I2S_CON_RXTH_2_WORD
#define I2S_FIFO_RX_LEVEL_WORD_3	I2S_CON_RXTH_3_WORD
#define I2S_FIFO_RX_LEVEL_WORD_4	I2S_CON_RXTH_4_WORD
#define I2S_FIFO_RX_LEVEL_WORD_5	I2S_CON_RXTH_5_WORD
#define I2S_FIFO_RX_LEVEL_WORD_6	I2S_CON_RXTH_6_WORD
#define I2S_FIFO_RX_LEVEL_WORD_7	I2S_CON_RXTH_7_WORD
#define I2S_FIFO_RX_LEVEL_WORD_8	I2S_CON_RXTH_8_WORD

#define I2S_MONO_RIGHT 			0
#define I2S_MONO_LEFT 			1

/*@}*/ /* end of group NANO1xx_I2S_EXPORTED_CONSTANTS */



/** @addtogroup NANO1xx_I2S_EXPORTED_STRUCTS I2S Exported Structs
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  I2S Data Structure 								                                          	           */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{
	uint32_t	u32SampleRate;
	uint32_t  	u32WordWidth;
	uint32_t  	u32AudioFormat;
	uint32_t  	u32DataFormat;
	uint32_t  	u32Mode;
	uint32_t  	u32TxFIFOThreshold;
	uint32_t  	u32RxFIFOThreshold;
}I2S_DATA_T;

/*@}*/ /* end of group NANO1xx_I2S_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_I2S_EXPORTED_FUNCTIONS I2S Exported Functions
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
  * @brief  This function is used to enable the I2S RX underflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_RxUnderFlow(void)
{
	I2S->INTEN |= I2S_IE_RXUDFIE;
}

/**
  * @brief  This function is used to enable the I2S RX overflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_RxOverFlow(void)
{
	I2S->INTEN |= I2S_IE_RXOVFIE;
}

/**
  * @brief  This function is used to enable the I2S RX threshold interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_RxThreshold(void)
{
	I2S->INTEN |= I2S_IE_RXTHIE;
}

/**
  * @brief  This function is used to enable the I2S TX underflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_TxUnderFlow(void)
{
	I2S->INTEN |= I2S_IE_TXUDFIE;
}

/**
  * @brief  This function is used to enable the I2S TX overflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_TxOverFlow(void)
{
	I2S->INTEN |= I2S_IE_TXOVFIE;
}

/**
  * @brief  This function is used to enable the I2S TX threshold interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableInt_TxThreshold(void)
{
	I2S->INTEN |= I2S_IE_TXTHIE;
}


/**
  * @brief  This function is used to disable the I2S RX underflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_RxUnderFlow(void)
{
	I2S->INTEN &= ~I2S_IE_RXUDFIE;
}

/**
  * @brief  This function is used to disable the I2S RX overflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_RxOverFlow(void)
{
	I2S->INTEN &= ~I2S_IE_RXOVFIE;
}

/**
  * @brief  This function is used to disable the I2S RX threshold interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_RxThreshold(void)
{
	I2S->INTEN &= ~I2S_IE_RXTHIE;
}

/**
  * @brief  This function is used to disable the I2S TX underflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_TxUnderFlow(void)
{
	I2S->INTEN &= ~I2S_IE_TXUDFIE;
}

/**
  * @brief  This function is used to disable the I2S TX overflow interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_TxOverFlow(void)
{
	I2S->INTEN &= ~I2S_IE_TXOVFIE;
}

/**
  * @brief  This function is used to disable the I2S TX threshold interrupt.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableInt_TxThreshold(void)
{
	I2S->INTEN &= ~I2S_IE_TXTHIE;
}

/**
  * @brief  Enable left channel zero cross detect function.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableLeftZeroCrossDetect(void)
{
	I2S->CTRL |= I2S_CON_LCHZCEN;
}

/**
  * @brief  Enable right channel zero cross detect function.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableRightZeroCrossDetect(void)
{
	I2S->CTRL |= I2S_CON_RCHZCEN;
}

/**
  * @brief  Disable left channel zero cross detect function.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableLeftZeroCrossDetect(void)
{
	I2S->CTRL &= ~I2S_CON_LCHZCEN;
}

/**
  * @brief  Disable right channel zero cross detect function.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableRightZeroCrossDetect(void)
{
	I2S->CTRL &= ~I2S_CON_RCHZCEN;
}


/**
  * @brief  Enable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableTxDMA(void)
{	
	I2S->CTRL |= I2S_CON_TXDMA;
}

/**
  * @brief  Disable I2S Tx DMA function. I2S requests DMA to transfer data to Tx FIFO.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableTxDMA(void)
{	
	I2S->CTRL &= ~I2S_CON_TXDMA;
}

/**
  * @brief  Enable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableRxDMA(void)
{
	I2S->CTRL |= I2S_CON_RXDMA;
}

/**
  * @brief  Disable I2S Rx DMA function. I2S requests DMA to transfer data from Rx FIFO.
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableRxDMA(void)
{
	I2S->CTRL &= ~I2S_CON_RXDMA;		
}

/**
  * @brief  Enable I2S Tx function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableTx(void)
{
	I2S->CTRL |= I2S_CON_TXEN;
}

/**
  * @brief  Disable I2S Tx function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableTx(void)
{
	I2S->CTRL &= ~I2S_CON_TXEN;
}

/**
  * @brief  Enable I2S Rx function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableRx(void)
{
	I2S->CTRL |= I2S_CON_RXEN;	
}

/**
  * @brief  Disable I2S Rx function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableRx(void)
{
	I2S->CTRL &= ~I2S_CON_RXEN;	
}

/**
  * @brief  Enable Tx Mute function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableTxMute(void)
{
	I2S->CTRL |= I2S_CON_MUTE;
}

/**
  * @brief  Disable Tx Mute function .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableTxMute(void)
{
	I2S->CTRL &= ~I2S_CON_MUTE;		
}

/**
  * @brief  Enable MCLK .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_EnableMCLK(void)
{	
	I2S->CTRL |= I2S_CON_MCLKEN;	
}

/**
  * @brief  Disable MCLK .
  * @param  None.
  * @retval None.
  */
static __INLINE void I2S_DisableMCLK(void)
{	
	I2S->CTRL &= ~I2S_CON_MCLKEN;	
}

/**
  * @brief  Clear Tx FIFO. Internal pointer is reset to FIFO start point.
  * @param  None
  * @retval None.
  */
static __INLINE void I2S_ClearTxFIFO(void)
{
	I2S->CTRL |= I2S_CON_CLR_TXFIFO;
}

/**
  * @brief  Clear Rx FIFO. Internal pointer is reset to FIFO start point.
  * @param  None
  * @retval None.
  */
static __INLINE void I2S_ClearRxFIFO(void)
{
	I2S->CTRL |= I2S_CON_CLR_RXFIFO;
}

/**
  * @brief  Decide when MONO mode is used, which channel the data is stored.
  * @param  u8Ch: I2S_MONO_LEFT / I2S_MONO_RIGHT
  * @retval None
  */
static __INLINE void I2S_SetMonoSrcChannel(uint8_t u8Ch)
{
	u8Ch == I2S_MONO_LEFT ?
		(I2S->CTRL |= I2S_CON_RXLCH) :
		(I2S->CTRL &= ~I2S_CON_RXLCH);
		
}

/**
  * @brief  Write data to I2S Tx FIFO.
  * @param  u32Data: The data written to FIFO.
  * @retval None
  */
static __INLINE void I2S_WriteTxFIFO(uint32_t u32Data)
{
	I2S->TXFIFO = u32Data;
}

/**
  * @brief  Read Rx FIFO.
  * @param  None.
  * @retval Data in Rx FIFO.
  */
static __INLINE uint32_t I2S_ReadRxFIFO(void)
{
	return I2S->RXFIFO;
}

/**
  * @brief  Read word data number in Tx FIFO.
  * @param  None.
  * @retval Word number in Tx FIFO.
  */
static __INLINE uint32_t I2S_ReadTxFIFOLevel(void)
{
	return (I2S->STATUS & I2S_STATUS_TX_LEVEL_MASK) >> 28;
}

/**
  * @brief  Read word data number in Rx FIFO.
  * @param  None.
  * @retval Word number in Rx FIFO.
  */
static __INLINE uint32_t I2S_ReadRxFIFOLevel(void)
{
	return (I2S->STATUS & I2S_STATUS_RX_LEVEL_MASK) >> 24;
}

/**
  * @brief  Enable I2S engine clock.
  * @param  None.  
  * @retval None.
  */
static __INLINE void I2S_Init(void)
{
	CLK->APBCLK |= CLK_APBCLK_I2S_EN;	
}

/**
  * @brief  Disable I2S engine clock.
  * @param  None.  
  * @retval None.
  */
static __INLINE void I2S_DeInit(void)
{
	CLK->APBCLK &= ~CLK_APBCLK_I2S_EN;	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Define I2S functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/

int32_t I2S_Open(I2S_DATA_T *sParam);
void I2S_Close(void);
uint32_t I2S_GetBCLKFreq(void);
void I2S_SetBCLKFreq(uint32_t u32Bclk);
uint32_t I2S_GetMCLKFreq(void);
void I2S_SetMCLKFreq(uint32_t u32Mclk);
uint32_t I2S_GetSourceClockFreq(void);

/*@}*/ /* end of group NANO1xx_I2S_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_I2S_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

