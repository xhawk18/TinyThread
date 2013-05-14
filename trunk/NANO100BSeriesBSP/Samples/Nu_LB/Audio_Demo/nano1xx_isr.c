
/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "nano1xx.h"
#include "nano1xx_i2c.h"
#include "nano1xx_i2s.h"

#include "config.h"

extern volatile uint32_t EndFlag0;
extern uint8_t Tx_Data0[2];
extern uint8_t DataCnt0;

#define NAU8822_ADDR	0x1A	 			/* NAU8822 Device ID */

#ifdef I2S_USE_FIFO
static uint32_t PcmBuff[32] = {0};
extern uint32_t volatile u32BuffPos;
#endif

void I2C0_IRQHandler(void)
{
    uint32_t status;
	
	// clear interrupt flag
	I2C0->INTSTS |= I2C_INTSTS_INTSTS;
	
  	status  = I2C0->STATUS;
  	
  	if (status == 0x08)						/* START has been transmitted */
	{
		I2C_WriteData(I2C0, NAU8822_ADDR<<1);
		I2C_Trig(I2C0);
	}	
	else if (status == 0x18)				/* SLA+W has been transmitted and ACK has been received */
	{
		I2C_WriteData(I2C0, Tx_Data0[DataCnt0++]);
		I2C_Trig(I2C0);
	}
	else if (status == 0x20)				/* SLA+W has been transmitted and NACK has been received */
	{		
		I2C_Ctrl(I2C0, 1, 1, 0);
		I2C_Trig(I2C0);
	}	
	else if (status == 0x28)				/* DATA has been transmitted and ACK has been received */
	{
		if (DataCnt0 != 2)
		{
			I2C_WriteData(I2C0, Tx_Data0[DataCnt0++]);
			I2C_Trig(I2C0);
		}
		else
		{
			I2C_Ctrl(I2C0, 0, 1, 0);
			I2C_Trig(I2C0);
			EndFlag0 = 1;
		}		
	}
	else
	{
		printf("Status 0x%x is NOT processed\n", status);
	}		
}

#ifdef I2S_USE_FIFO
void I2S_IRQHandler(void)
{
    uint32_t u32Reg;
	uint32_t u32Len, i;
	uint32_t *pBuffTx, *pBuffRx;
		
	u32Reg = I2S->STATUS;

	if (u32Reg & I2S_STATUS_I2STXINT)
	{	
		pBuffTx = &PcmBuff[0];
	
		/* Read Tx FIFO free size */
		u32Len = 8 - I2S_ReadTxFIFOLevel();
		
		if (u32BuffPos >= 8)
		{
			for	(i = 0; i < u32Len; i++)
			{											   
		   		I2S_WriteTxFIFO(pBuffTx[i]);
			}
	
			for (i = 0; i < BUFF_LEN - u32Len; i++)
			{
				pBuffTx[i] = pBuffTx[i + u32Len];	
			}
	
			u32BuffPos -= u32Len;
		}
		else
		{
			for	(i = 0; i < u32Len; i++)
			{
		   		I2S_WriteTxFIFO(0x00);	   
			}			
		}
	}
	else if (u32Reg & I2S_STATUS_I2SRXINT)
	{
		if (u32BuffPos < (BUFF_LEN-8))
		{
			pBuffRx = &PcmBuff[u32BuffPos];
	
			/* Read Rx FIFO Level */
			u32Len = I2S_ReadRxFIFOLevel();
		
			for ( i = 0; i < u32Len; i++ )
			{
				pBuffRx[i] = I2S_ReadRxFIFO();
			}
		
			u32BuffPos += u32Len;
		
			if (u32BuffPos >= BUFF_LEN)
			{
				u32BuffPos =	0;
			}						 	
		}
	}
}
#endif

#if 1
#ifdef I2S_USE_PDMA
extern uint32_t volatile PdmaTxBuff[BUFF_LEN];
extern uint32_t volatile PdmaRxBuff[BUFF_LEN];
extern uint32_t volatile Buff_Tx_Idx;
void PDMA_IRQHandler(void)
{
	uint32_t u32ISR;

	u32ISR = PDMAGCR->ISR;
	if (u32ISR & 0x2)
	{		
		u32ISR = PDMA1->ISR;
	 	if(u32ISR & 0x04)
		{							/* Wrap around Transfer complete and clear */
			PDMA1->ISR = 0x04;
			
			/* Last half of buffer can be copied from Rx buffer */
			Buff_Tx_Idx = BUFF_HALF_LEN;
		}
		else if(u32ISR & 0x10)
		{							/* Wrap around Transfer Half and clear */
			PDMA1->ISR = 0x10;
			
			/* First half of buffer can be copied from Rx buffer */
			Buff_Tx_Idx = 0;
		}
	}
	else if(u32ISR & 0x4)
	{
		u32ISR = PDMA2->ISR;
	 	if(u32ISR & 0x04)
		{							/* Wrap around Transfer complete and clear */
			PDMA2->ISR = 0x04;
			
			/* Copy data form Rx buffer to Tx buffer */
			memcpy((void *)&PdmaTxBuff[Buff_Tx_Idx], (void *)&PdmaRxBuff[BUFF_HALF_LEN], BUFF_HALF_LEN*sizeof(uint32_t));
		}
		else if(u32ISR & 0x10)
		{							/* Wrap around Transfer Half and clear */
			PDMA2->ISR = 0x10;
			
			/* Copy data form Rx buffer to Tx buffer */
			memcpy((void *)&PdmaTxBuff[Buff_Tx_Idx], (void *)&PdmaRxBuff[0], BUFF_HALF_LEN*sizeof(uint32_t));
		}
	}
}
#endif
#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
