
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
#include <stdint.h>
#include "nano1xx.h"
#include "nano1xx_i2c.h"

extern uint8_t Device_Addr0;
extern uint8_t Tx_Data0[3];
extern uint8_t Rx_Data0;
extern uint8_t DataLen0;
extern uint8_t DataLen1;
extern uint8_t volatile EndFlag0;
extern uint8_t volatile TimeOutFlag0; 
extern uint8_t volatile TimeOutFlag1;
extern uint8_t volatile MasterTxGo;
extern uint8_t volatile MasterRxGo;
extern uint8_t Addr1[3];
extern uint8_t  Slave_Buff1[32];
extern uint16_t Slave_Buff_Addr1;

/* ToDo: add interrupt service routines. */
void I2C0_IRQHandler(void)
{
    uint32_t status;
	
	// clear interrupt flag
	I2C0->INTSTS |= I2C_INTSTS_INTSTS;	
  	
  	status  = I2C0->STATUS; 
  	
  	if (I2C0->INTSTS & I2C_INTSTS_TIF)
	{
		I2C0->INTSTS |= I2C_INTSTS_TIF;	/* Clear TIF */
       	TimeOutFlag0 = 1;
		printf("Warring: I2C0 Time Out !!\n");	
		return;	
	}
	 	
  	if(MasterTxGo)
  	{
  		if (status == 0x08)						/* START has been transmitted */
		{
			I2C_WriteData(I2C0, Device_Addr0<<1);
			I2C_Trig(I2C0);
		}	
		else if (status == 0x18)				/* SLA+W has been transmitted and ACK has been received */
		{
			I2C_WriteData(I2C0, Tx_Data0[DataLen0++]);
			I2C_Trig(I2C0);
		}
		else if (status == 0x20)				/* SLA+W has been transmitted and NACK has been received */
		{
			I2C_Ctrl(I2C0, 1, 1, 0);
			I2C_Trig(I2C0);
		}	
		else if (status == 0x28)				/* DATA has been transmitted and ACK has been received */
		{
			if (DataLen0 != 3)
			{
				I2C_WriteData(I2C0, Tx_Data0[DataLen0++]);
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
  	
  	if(MasterRxGo)
  	{
  		if (status == 0x08)					   	/* START has been transmitted and prepare SLA+W */
		{
			I2C_WriteData(I2C0, Device_Addr0<<1);
			I2C_Trig(I2C0);
		}	
		else if (status == 0x18)				/* SLA+W has been transmitted and ACK has been received */
		{
			I2C_WriteData(I2C0, Tx_Data0[DataLen0++]);
			I2C_Trig(I2C0);
		}
		else if (status == 0x20)				/* SLA+W has been transmitted and NACK has been received */
		{
			I2C_Ctrl(I2C0, 1, 1, 0);
			I2C_Trig(I2C0);
		}
		else if (status == 0x28)				/* DATA has been transmitted and ACK has been received */
		{
			if (DataLen0 != 2)
			{
				I2C_WriteData(I2C0, Tx_Data0[DataLen0++]);
				I2C_Trig(I2C0);
			}
			else
			{
				I2C_Ctrl(I2C0, 1, 0, 0);
				I2C_Trig(I2C0);
			}		
		}
		else if (status == 0x10)				/* Repeat START has been transmitted and prepare SLA+R */
		{
			I2C_WriteData(I2C0, Device_Addr0<<1 | 0x01);
			I2C_Trig(I2C0);
		}
		else if (status == 0x40)				/* SLA+R has been transmitted and ACK has been received */
		{
			I2C_Trig(I2C0);
		}		
		else if (status == 0x58)				/* DATA has been received and NACK has been returned */
		{
			Rx_Data0 = I2C_ReadData(I2C0);
			I2C_Ctrl(I2C0, 0, 1, 0);
			I2C_Trig(I2C0);
			EndFlag0 = 1;
		}
		else if (status == 0xf8)				/* Stop */
		{
			I2C_Ctrl(I2C0, 0, 1, 0);
		}
		else
		{
			printf("Status 0x%x is NOT processed\n", status);
		}		
  	}
}


void I2C1_IRQHandler(void)
{
    uint32_t status;
	
	// clear interrupt flag
	I2C1->INTSTS |= I2C_INTSTS_INTSTS;
	
  	status  = I2C1->STATUS;
  	
  	if (I2C1->INTSTS & I2C_INTSTS_TIF)
	{
		I2C1->INTSTS |= I2C_INTSTS_TIF;	/* Clear TIF */
       	TimeOutFlag1 = 1;
		printf("Warring: I2C1 Time Out !!\n");		
		return;
	}
	
  	if ((status == 0x60) || (status == 0x68))	  	/* SLA+W has been received and ACK has been returned */
	{
		DataLen1 = 0; 
		I2C_Ctrl(I2C1, 0, 0, 1);
		I2C_Trig(I2C1);
	}	
	else if (status == 0x80)					  	/* DATA has been received and ACK has been returned */
	{
       	Addr1[DataLen1++] = I2C_ReadData(I2C1);
  		
  		if (DataLen1 == 2)
  		{
  			Slave_Buff_Addr1 = (Addr1[0] << 8) + Addr1[1];
  		}

  		if ((DataLen1 == 3) && (Slave_Buff_Addr1 < 32))		
  		{
  			Slave_Buff1[Slave_Buff_Addr1] = Addr1[2];
			DataLen1 = 0;  		
  		}

  		I2C_Ctrl(I2C1, 0, 0, 1);
  		I2C_Trig(I2C1);
	}	
	else if ((status == 0xB0) || (status == 0xA8))  /* SLA+R has been received and ACK has been returned */
	{
		I2C_WriteData(I2C1, Slave_Buff1[Slave_Buff_Addr1++]);
		if (Slave_Buff_Addr1 >= 32)
			Slave_Buff_Addr1 = 0;
  		I2C_Ctrl(I2C1, 0, 0, 1);
  		I2C_Trig(I2C1);
	}												
	else if (status == 0xC0)						/* DATA has been transmitted and NACK has been received */
	{
  		I2C_Ctrl(I2C1, 0, 0, 1);
  		I2C_Trig(I2C1);
	}
	else if (status == 0xA0)						/* STOP or Repeat START has been received */
	{
		DataLen1 = 0;
		I2C_Ctrl(I2C1, 0, 0, 1);
		I2C_Trig(I2C1);
	}
	else
	{
		printf("Status 0x%x is NOT processed\n", status);
	}
}

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
