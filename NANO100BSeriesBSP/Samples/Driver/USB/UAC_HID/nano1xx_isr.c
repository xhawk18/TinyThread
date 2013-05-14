/******************************************************************************
 * @file     nano1xx_isr.c
 * @brief    This file contains interrupt service routines
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_usb.h"
#include "nano1xx_i2c.h"
#include "nano1xx_i2s.h"
#include "USB_Class.h"


/**   
  * @brief  HIRC_IRQHandler, HIRC trim interrupt handler.
  * @param  None.
  * @retval None.
  */
void HIRC_IRQHandler(void)
{
	__IO uint32_t reg = GCR->IRCTRIMISR;

	if ((reg & GCR_IRCTRIMINT_FAIL_INT) || (reg & GCR_IRCTRIMINT_32KERR_ENNT))
	{
		GCR->IRCTRIMISR = reg;
		GCR->IRCTRIMCTL = (GCR_IRCTRIMCTL_TRIM_12M | GCR_IRCTRIMCTL_LOOP_32CLK | GCR_IRCTRIMCTL_RETRY_512);
	}
}


/**   
  * @brief  USBD_IRQHandler, USB interrupt handler.
  * @param  None.
  * @retval None.
  */
void USBD_IRQHandler(void)
{
	uint32_t u32INTSTS = USBD->INTSTS;

	if (u32INTSTS & USB_INTSTS_FLD_STS)
	{
	    /* Handle the USB attached/detached event */
		USB_FloatDetect();
	}
	else if(u32INTSTS & USB_INTSTS_BUS_STS)
	{
	    /* Handle the USB bus event: Reset, Suspend, and Resume */
		USB_BusEvent();
	}
	else if(u32INTSTS & USB_INTSTS_USB_STS)
	{
	    /* Handle the USB Protocol/Clase event */
		USB_UsbEvent(u32INTSTS);
    }
	else if (u32INTSTS & USB_INTSTS_WKEUP_STS)
	{
		/* Clear wakeup event. write one clear */
		USBD->INTSTS |= USB_INTSTS_WKEUP_STS;
	}
}

extern uint8_t Device_Addr0;
extern uint8_t Tx_Data0[2];
extern uint8_t DataCnt0;
extern uint32_t __IO EndFlag0;

void I2C0_IRQHandler(void)
{
    uint32_t status;
	
	// clear interrupt flag
	I2C0->INTSTS |= I2C_INTSTS_INTSTS;	
  	
  	status  = I2C0->STATUS; 
	
	if (status == 0x08)						/* START has been transmitted */
	{
		I2C_WriteData(I2C0, Device_Addr0<<1);
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
		while(1);
		//printf("Status 0x%x is NOT processed\n", status);
	}
}

/* Recoder Buffer and its pointer */
extern uint16_t PcmRecBuff[BUFF_LEN];
extern uint32_t u32RecPos_Out;
extern uint32_t u32RecPos_In;

/* Player Buffer and its pointer */
extern uint32_t PcmPlayBuff[BUFF_LEN];
extern uint32_t u32PlayPos_Out;
extern uint32_t u32PlayPos_In;

void I2S_IRQHandler(void)
{
    uint32_t u32Reg;
	uint32_t i;
	uint32_t *pBuffTx; 
	uint16_t *pBuffRx;
		
	u32Reg = I2S->STATUS;

	if (u32Reg & I2S_STATUS_I2STXINT)
	{	
		if ((u32PlayPos_Out < u32PlayPos_In - 4) || (u32PlayPos_Out > u32PlayPos_In))
		{
			pBuffTx = &PcmPlayBuff[u32PlayPos_Out];
	
			for	( i = 0; i < 4; i++)
			{
				I2S_WriteTxFIFO(pBuffTx[i]);
			}
	
			u32PlayPos_Out += 4;
	
			if (u32PlayPos_Out >= BUFF_LEN)
			{
				u32PlayPos_Out = 0;
			}
		}
		else
		{
			for	( i = 0; i < 4; i++)
			{
				I2S_WriteTxFIFO(0x00);
			}
		}
	}
	else if (u32Reg & I2S_STATUS_I2SRXINT)
	{
		pBuffRx = &PcmRecBuff[u32RecPos_In];

		for ( i = 0; i < 4; i++ )
		{
			pBuffRx[i] = (uint16_t)(I2S_ReadRxFIFO() & 0xFFFF);
		}
	
		u32RecPos_In += 4;
	
		if (u32RecPos_In >= BUFF_LEN)
		{
			u32RecPos_In = 0;
		}
	}
}


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
