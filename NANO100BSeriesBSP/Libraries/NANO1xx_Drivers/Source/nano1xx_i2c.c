/******************************************************************************
 * @file     nano1xx_i2c.c
 * @brief    NANO1xx I2C driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
//* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "nano1xx.h"
#include "nano1xx_i2c.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_I2C_Driver I2C Driver
  @{
*/

/** @addtogroup NANO1xx_I2C_EXPORTED_FUNCTIONS I2C Exported Functions
  @{
*/

/**
  * @brief  Set I2C control bit.
  * @param  I2cPort: I2C port handler
  * @param  start: Set I2C START bit.
  *	@param  stop: Set I2C STOP bit.
  *	@param  ack: set I2C ACK bit.
  * @retval None.
  */
void I2C_Ctrl(I2C_TypeDef *I2cPort, uint8_t start, uint8_t stop, uint8_t ack)
{	
	uint32_t Reg = 0;
	
	if (start)
		Reg |= I2C_CON_START;
	if (stop)
	    Reg |= I2C_CON_STOP;
	if (ack)
		Reg |= I2C_CON_ACK;
	
	I2cPort->CON = (I2cPort->CON & ~0x1E) | Reg;
}


/**
  * @brief  Enable I2C function and set clock divider. I2C clock = PCLK / (4x(divider+1)).
  * @param  I2cPort: I2C port handler
  *	@param  u32BusClock: I2C bus clock frequency (Hz)  
  * @retval None.
  */
void I2C_Open(I2C_TypeDef *I2cPort, uint32_t u32BusClock)
{
    uint32_t divider;     
 	 	
	divider = (uint32_t) (((SYS_GetHCLKFreq()*10)/(u32BusClock * 4) + 5) / 10 - 1);	/* Compute proper divider for I2C clock */
	
    if((uint32_t)I2cPort == I2C1_BASE)
	{		
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_I2C1;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_I2C1;		
	}
	else
	{		
		GCR->IPRST_CTL2 |= GCR_IPRSTCTL2_I2C0;
		GCR->IPRST_CTL2 &= ~GCR_IPRSTCTL2_I2C0;	
	}
	
	I2cPort->DIV = divider;
	I2cPort->CON = I2C_CON_INTEN | I2C_CON_IPEN;
}

/**
  * @brief  Disable I2C function and clock source.
  * @param  I2cPort: I2C port handler  
  * @retval 0: None
  */
void I2C_Close(I2C_TypeDef *I2cPort)
{
    I2cPort->CON &= ~(I2C_CON_INTEN | I2C_CON_IPEN); 
}

/**
  * @brief  Get I2C bit rate.
  * @param  I2cPort: I2C port handler   
  * @retval I2C Bus Clock Frequency
  */
uint32_t I2C_GetClockFreq(I2C_TypeDef *I2cPort)
{
    uint32_t divider;     
 
	divider = I2cPort->DIV;
	return ( SYS_GetHCLKFreq() / ((divider+1)<<2) );
}

/**
  * @brief  Enable I2C interrupt and NVIC corresponding to I2C.
  * @param  I2cPort: I2C port handler  
  * @retval 0: None.
  */
void I2C_EnableInt(I2C_TypeDef *I2cPort)
{	
	I2cPort->CON |= I2C_CON_INTEN;
		
	if((uint32_t)I2cPort == I2C0_BASE)	
		NVIC_EnableIRQ(I2C0_IRQn);
	else if((uint32_t)I2cPort == I2C1_BASE)	
		NVIC_EnableIRQ(I2C1_IRQn);	
}

/**
  * @brief  Disable I2C interrupt and NVIC corresponding to I2C.
  * @param  I2cPort: I2C port handler  
  * @retval 0: None.
  */
void I2C_DisableInt(I2C_TypeDef *I2cPort)
{
	I2cPort->CON &= ~I2C_CON_INTEN;
	
	if((uint32_t)I2cPort == I2C0_BASE)	
		NVIC_DisableIRQ(I2C0_IRQn);
	else if((uint32_t)I2cPort == I2C1_BASE)	
		NVIC_DisableIRQ(I2C1_IRQn);   
}

/**
  * @brief  Set 2 7-bit slave addresses and enable General Call function.
  * @param  I2cPort: I2C port handler
  *	@param  slaveNo: There are 2 slave addrss. The slaveNo is 0/1.
  *	@param  slave_addr: 7-bit data
  *	@param  GC_Flag: Enable/Disable GC function
  * @retval 0: Success.
  *	@retval  <0: Failed.
  */
int32_t I2C_SetAddress(I2C_TypeDef *I2cPort, uint8_t slaveNo, uint8_t slave_addr, uint8_t GC_Flag)
{	
	switch (slaveNo)
	{
		case 0:
			I2cPort->SADDR0 = (slave_addr << 1) | GC_Flag;
			break;
		case 1:
			I2cPort->SADDR1 = (slave_addr << 1) | GC_Flag;
			break;			
		default:
			return -1;
	}

	return 0;
}

/**
  * @brief  Set 2 7-bit slave address mask. The corresponding address bit is "Don't Care".
  * @param  I2cPort: I2C port handler
  *	@param  slaveNo: There are 4 slave addrss. The slaveNo is 0/1.
  *	@param  slaveAddrMask: 7-bit slave address mask 
  * @retval 0: Success.
  *	@retval  <0: Failed.
  */
int32_t I2C_SetAddressMask(I2C_TypeDef *I2cPort, uint8_t slaveNo, uint8_t slaveAddrMask)
{
	switch (slaveNo)
	{
		case 0:
			I2cPort->SAMASK0 = slaveAddrMask << 1;
			break;
		case 1:
			I2cPort->SAMASK1 = slaveAddrMask << 1;
			break;			
		default:
			return -1;
	}
	
	return 0;
}

/**
  * @brief  Enable I2C engine clock.
  * @param  I2cPort: I2C port handler  
  * @retval None.
  */
void I2C_Init(I2C_TypeDef *I2cPort)
{
	if((uint32_t)I2cPort == I2C1_BASE)
		CLK->APBCLK |= CLK_APBCLK_I2C1_EN;
	else
		CLK->APBCLK |= CLK_APBCLK_I2C0_EN;
}

/**
  * @brief  Disable I2C engine clock.
  * @param  I2cPort: I2C port handler  
  * @retval None.
  */
void I2C_DeInit(I2C_TypeDef *I2cPort)
{
	if((uint32_t)I2cPort == I2C1_BASE)
		CLK->APBCLK &= ~CLK_APBCLK_I2C1_EN;
	else
		CLK->APBCLK &= ~CLK_APBCLK_I2C0_EN;
}

/*@}*/ /* end of group NANO1xx_I2C_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_I2C_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
