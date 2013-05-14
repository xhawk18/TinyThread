/******************************************************************************
 * @file     nano1xx_i2c.h
 * @brief    NANO1xx I2C driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "nano1xx.h"
#include "nano1xx_sys.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_I2C_Driver I2C Driver
  @{
*/


/** @addtogroup NANO1xx_I2C_EXPORTED_FUNCTIONS I2C Exported Functions
  @{
*/
		   
/*---------------------------------------------------------------------------------------------------------*/
/* inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
  * @brief  Active I2C operation.
  * @param  I2cPort: I2C port handler
  * @retval None.
  */
static __INLINE void I2C_Trig(I2C_TypeDef *I2cPort)
{
	I2cPort->CON |= I2C_CON_I2C_STS;
}

/**
  * @brief  Clear interrupt flag.
  * @param  I2cPort: I2C port handler
  * @retval None.
  */
static __INLINE void I2C_ClearIntFlag(I2C_TypeDef *I2cPort)
{
	I2cPort->INTSTS |= I2C_INTSTS_INTSTS;
}

/**
  * @brief  Write Data into I2C Data register.
  * @param  I2cPort: I2C port handler
  *	@param  u8data: I2C data.
  * @retval None.
  */
static __INLINE void I2C_WriteData(I2C_TypeDef *I2cPort, uint8_t u8data)
{
	I2cPort->DATA = u8data;
}

/**
  * @brief  Read Data from I2C Data register.
  * @param  I2cPort: I2C port handler  
  * @retval I2C data.
  */
static __INLINE uint8_t I2C_ReadData(I2C_TypeDef *I2cPort)
{
	return I2cPort->DATA;
}

/**
  * @brief  Get I2C interrupt flag.
  * @param  I2cPort: I2C port handler  
  * @retval I2C interrupt flag.
  */
static __INLINE uint8_t I2C_GetIntFlag(I2C_TypeDef *I2cPort)
{
	return (I2cPort->INTSTS & I2C_INTSTS_INTSTS);
}

/**
  * @brief  Clear I2C time out flag.
  * @param  I2cPort: I2C port handler  
  * @retval None.
  */
static __INLINE void I2C_ClearTimeoutFlag(I2C_TypeDef *I2cPort)
{
	I2cPort->INTSTS |= I2C_INTSTS_TIF;
}

/**
  * @brief  Get I2C status.
  * @param  I2cPort: I2C port handler  
  * @retval I2C status.
  */
static __INLINE uint32_t I2C_GetStatus(I2C_TypeDef *I2cPort)
{
	return I2cPort->INTSTS;
}

/**
  * @brief  Enable I2C 14-bit timeout counter and set div4 bit of timeout counter.
  * @param  I2cPort: I2C port handler
  *	@param  i32enable: Enable timeout counter
  *	@param  u8div4:	Enable timeout counter input clock is didider by 4
  * @retval 0: None.
  */
static __INLINE void I2C_SetTimeoutCounter(I2C_TypeDef *I2cPort, int32_t i32enable, uint8_t u8div4)
{
	I2cPort->TOUT = (u8div4 << 1) | i32enable;
}

/**
  * @brief  Configure Wakeup function.
  * @param  I2cPort: I2C port handler
  * @param  bEnable: Enable (TRUE) / Disable (FALSE)
  * @retval None.
  */
static __INLINE void I2C_WakeupConfigure(I2C_TypeDef *I2cPort, uint8_t bEnable)
{
	I2cPort->WKUPCON = bEnable;
}

/**
  * @brief  Set clock divider. I2C clock = PCLK / (4x(divider+1)).
  * @param  I2cPort: I2C port handler 
  *	@param  u32BusClock: I2C bus clock frequency (Hz)    
  * @retval 0: None.
  */
static __INLINE void I2C_SetClockFreq(I2C_TypeDef *I2cPort, uint32_t u32BusClock)
{ 	    
    I2cPort->DIV = (uint32_t) (((SYS_GetHCLKFreq()*10)/(u32BusClock * 4) + 5) / 10 - 1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Define I2C functions prototype                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_Ctrl(I2C_TypeDef *I2cPort, uint8_t start, uint8_t stop, uint8_t ack);
void I2C_Open(I2C_TypeDef *I2cPort, uint32_t u32BusClock);
void I2C_Close(I2C_TypeDef *I2cPort);
uint32_t I2C_GetClockFreq(I2C_TypeDef *I2cPort);
void I2C_EnableInt(I2C_TypeDef *I2cPort);
void I2C_DisableInt(I2C_TypeDef *I2cPort);
int32_t I2C_SetAddress(I2C_TypeDef *I2cPort, uint8_t slaveNo, uint8_t slave_addr, uint8_t GC_Flag);
int32_t I2C_SetAddressMask(I2C_TypeDef *I2cPort, uint8_t slaveNo, uint8_t slaveAddrMask);
void I2C_Init(I2C_TypeDef *I2cPort);
void I2C_DeInit(I2C_TypeDef *I2cPort);

/*@}*/ /* end of group NANO1xx_I2C_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_I2C_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

