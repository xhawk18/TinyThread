/******************************************************************************
 * @file     nano1xx_pdma.c
 * @brief    NANO1xx PDMA driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>


/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "nano1xx_pdma.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_PDMA_Driver PDMA Driver
  @{
*/



/** @addtogroup NANO1xx_PDMA_EXPORTED_FUNCTIONS PDMA Exported Functions
  @{
*/

/************************************************************/
/**   
  * @brief  PDMA_Close, Disable all channel clocks.
  * @param  None.
  * @retval None.
  */
void PDMA_Close(void)
{
   	/* Disable All PDMA clock */
	PDMAGCR->CSR = 0;

}

/**   
  * @brief  PDMA_Open, configures PDMA setting.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  sParam : PDMA configure structure.
  * @retval None.
  */
void PDMA_Open(PDMA_TypeDef *ch, STR_PDMA_T *sParam)
{
	/*-----------------------------------------------------------------------------------------------------*/
    /* Set PDMA settings                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
	ch->SAR = sParam->SrcAddr;			/* Source Start Address */
	ch->DAR = sParam->DestAddr;			/* Destination Start Address */
	ch->BCR = sParam->ByteCnt;			/* Byte Count */
	ch->CSR = (uint32_t)sParam->SrcCtrl |sParam->DestCtrl |sParam->TransWidth |sParam->Mode |
			  sParam->IsTimeOutEnable |PDMA_CSR_CEN;
}

/**   
  * @brief  PDMA_OpenVDMA, configures VDMA setting.
  * @param  sParam : PDMA configure structure.
  * @retval None.
  */
void PDMA_OpenVDMA(STR_VDMA_T *sParam)
{
	/*-----------------------------------------------------------------------------------------------------*/
    /* Set VDMA settings                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
	PDMAGCR->CSR |= (1<<8);					/* Enable Channel Clock */

	VDMA->SAR    = sParam->SrcAddr;			/* Set Source Address */
	VDMA->DAR    = sParam->DestAddr;		/* Set Destination Address */
	VDMA->BCR    = sParam->ByteCnt;			/* Set Byte Count Register */
	VDMA->DASOCR = sParam->u16Dastobl;
	VDMA->SASOCR = (sParam->u16Stbc << 16) | sParam->u16Sastobl;

	VDMA->CSR = sParam->Dir | sParam->IsStrideEnable | VDMA_CSR_CEN;
}

/**   
  * @brief  PDMA_EnableInt, Enable Interrupt for channel.
  * @param  ch: PDMA Source channel, PDMA1 ~ PDMA6.
  * @param  IntSource: Intterrupt source.
  * @retval None.
  */
void PDMA_EnableInt(PDMA_TypeDef *ch, uint32_t IntSource)
{
	ch->IER |= IntSource;

    /* Enable Specified Interrupt */
	NVIC_EnableIRQ(PDMA_IRQn);
}


/**   
  * @brief  PDMA_OpenCRC, configures CRC setting.
  * @param  sParam : PDMA configure structure.
  * @retval None.
  */
void PDMA_OpenCRC(STR_CRC_T *sParam)
{
	/*-----------------------------------------------------------------------------------------------------*/
    /* Set CRC settings                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
	PDMAGCR->CSR |= (1<<24);					/* Enable CRC Clock */

	PDMACRC->DMASAR = sParam->u32DMASrcAddr;		/* Set Source Address */
	PDMACRC->DMABCR = sParam->u16DMAByteCnt;		/* Set Byte Count Register */

	PDMACRC->CTL = sParam->u32Mode | sParam->u32CPUWdlen | 
			(sParam->bIsChecksumComplement ? PDMACRC_CTL_CHECKSUM_COM : 0) |
			(sParam->bIsWriteDataComplement ? PDMACRC_CTL_WDATA_COM : 0) |
			(sParam->bIsChecksumReverse ? PDMACRC_CTL_CHECKSUM_RVS : 0) |
			(sParam->bIsWriteDataReverse ? PDMACRC_CTL_WDATA_RVS : 0) |
			PDMACRC_CTL_CEN;
}


/**   
  * @brief  PDMA_CRCEnableInt, Enable CRC Interrupt.
  * @param  IntSource: Intterrupt source.
  * @retval None.
  */
void PDMA_CRCEnableInt(uint32_t IntSource)
{
	PDMACRC->DMAIER |= IntSource;

    /* Enable Specified Interrupt */
	NVIC_EnableIRQ(PDMA_IRQn);
}

/*@}*/ /* end of group NANO1xx_PDMA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_PDMA_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
