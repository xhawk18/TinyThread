/******************************************************************************
 * @file     nano1xx_spi.h
 * @brief    NANO1xx SPI driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_SPI_H
#define __NANO1XX_SPI_H

#include "nano1xx.h"
#include "nano1xx_assert.h"

/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_SPI_Driver SPI Driver
  @{
*/

/** @addtogroup NANO1xx_SPI_EXPORTED_CONSTANTS SPI Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Define Error Code                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/*
	SPI_ERR_BUSY                   SPI is busy
	SPI_ERR_TIMEOUT				   SPI FIFO timeout
*/
#define SPI_ERR_BUSY                 	0
#define SPI_ERR_TIMEOUT               	(-1)

#define SPI_CHECK_BIT_LENGTH_0				(sParam->i32BitLength >=8) && (sParam->i32BitLength <= 32) 	
#define SPI_CHECK_BIT_LENGTH_1				(i32BitLength >=8) && (i32BitLength <= 32) 	
#define SPI_CHECK_BYTE_SUS					(i32Interval >= 2) && (i32Interval <= 17)	
#define SPI_CHECK_SET_DATAREG_NUM			u32DataCount <= 2								
#define SPI_CHECK_BYTE_REORDER(port)		(!(SpiPort->CTL & SPI_CTL_TX_BIT_LEN_16BIT) && !(SpiPort->CTL & SPI_CTL_TX_BIT_LEN_24BIT) && !(SpiPort->CTL & SPI_CTL_TX_BIT_LEN_32BIT))


/*  SPI Mode  */
#define SPI_MODE_MASTER		0
#define SPI_MODE_SLAVE		SPI_CTL_SLAVE

/*  SPI tx/rx data order  */
#define SPI_MSB				0
#define SPI_LSB				SPI_CTL_LSB

/*---------------------------------------------------------------------------------------------------------*/
/* SPI Setting                                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
typedef enum {
	SPI_TYPE0=0x004,
	SPI_TYPE1=0x002,
	SPI_TYPE2=0x804,
	SPI_TYPE3=0x802,
} SPI_TRANS_TYPE;

typedef enum {
	SPI_NONE,
	SPI_SS0,
	SPI_SS1,
	SPI_SS0_SS1
} SPI_SLAVE_SEL;

typedef enum {
	SPI_DUALIO_DIR_IN=0,
	SPI_DUALIO_DIR_OUT=1
} SPI_DUALIO_DIR;

/*@}*/ /* end of group NANO1xx_SPI_EXPORTED_CONSTANTS */

/** @addtogroup NANO1xx_SPI_EXPORTED_STRUCTS SPI Exported Structs
  @{
*/

/*---------------------------------------------------------------------------------------------------------*/
/*  SPI Data Structure 								                                          	           */
/*---------------------------------------------------------------------------------------------------------*/
typedef struct
{	
	uint32_t u32Mode;
	uint32_t u32Type;
	int32_t i32BitLength;
}SPI_DATA_T;

/*@}*/ /* end of group NANO1xx_SPI_EXPORTED_STRUCTS */



/** @addtogroup NANO1xx_SPI_EXPORTED_FUNCTIONS SPI Exported Functions
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/* inline functions                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
/**
  * @brief  Set 2-bit serial data I/O mode.
  *			When enable 2-bit serial data I/O mode, the Tx_NUM must be configure as 0x00 (one transaction in one transfer.)
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_Enable2BitTransferMode(SPI_TypeDef *SpiPort)
{	
	SpiPort->CTL |= SPI_CTL_TWOB;
}

/**
  * @brief  Disable 2-bit serial data I/O mode.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_Disable2BitTransferMode(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL &= ~SPI_CTL_TWOB;
}

/**
  * @brief  Set to transfer data with LSB first
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_SetLSBEndian(SPI_TypeDef *SpiPort)
{	
	SpiPort->CTL |= SPI_CTL_LSB;
}

/**
  * @brief  Set to transfer data with MSB first
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_SetMSBEndian(SPI_TypeDef *SpiPort)
{	
	SpiPort->CTL &= ~SPI_CTL_LSB;
}

/**
  * @brief  Enable Byte Reorder function.
  *			The Byte Reorder function is supported only in 16-, 24- and 32-bit transaction mode.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
  
static __INLINE void SPI_EableByteReorder(SPI_TypeDef *SpiPort)
{	
    assert_param(SPI_CHECK_BYTE_REORDER(SpiPort));
    SpiPort->CTL |= SPI_CTL_REORDER;    
}

/**
  * @brief  Disable Byte Reorder function.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
  
static __INLINE void SPI_SetByteReorder(SPI_TypeDef *SpiPort)
{	
	SpiPort->CTL &= ~SPI_CTL_REORDER;
}

/**
  * @brief  Set the trigger mode of slave select pin to level trigger.  
  * @param  SpiPort: SPI port handler
  * @retval None.  
  */
static __INLINE void SPI_SetLevelTriggerMode(SPI_TypeDef *SpiPort)
{    
    SpiPort->SSR |= SPI_SSR_SS_LTRIG;
}

/**
  * @brief  Set the trigger mode of slave select pin to edge trigger.  
  * @param  SpiPort: SPI port handler
  * @retval None.  
  */
static __INLINE void SPI_SetEdgeTriggerMode(SPI_TypeDef *SpiPort)
{    
    SpiPort->SSR &= ~SPI_SSR_SS_LTRIG;
}

/**
  * @brief  Set the trigger mode of slave select pin to active high.  
  * @param  SpiPort: SPI port handler
  * @retval None.  
  */
static __INLINE void SPI_SetSlaveSelectActiveHigh(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR |= SPI_SSR_SS_LVL;
}

/**
  * @brief  Set the trigger mode of slave select pin to active low.  
  * @param  SpiPort: SPI port handler
  * @retval None.  
  */
static __INLINE void SPI_SetSlaveSelectActiveLow(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR &= ~SPI_SSR_SS_LVL;
}

/**
  * @brief  Enable the automatic slave select function and set the specified slave select pin.
  * @param  SpiPort: SPI port handler
  *	@param  eSlaveSel: Select the slave select pins which will be used. 
  * @retval None.
  */
static __INLINE void SPI_EnableAutoSS(SPI_TypeDef *SpiPort, SPI_SLAVE_SEL eSlaveSel)
{
    SpiPort->SSR |= SPI_SSR_AUTOSS;
    SpiPort->SSR =  (SpiPort->SSR & ~SPI_SSR_SSR_MASK) | eSlaveSel;
}

/**
  * @brief  Disable the Automatic Slave Slect function and deselect slave select pins.
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
static __INLINE void SPI_DisableAutoSS(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR &= ~SPI_SSR_AUTOSS;
}

/**
  * @brief  Configure the slave select pins.
  * @param  SpiPort: SPI port handler
  *	@param  eSlaveSel: In automatic slave select operation, to use this parameter to select the slave select pins
  *					   which will be used. 
  *					   In manual slave select operation, the specified slave select pins will be set to active state.
  *					   It could be SPI_NONE, SPI_SS0, SPI_SS1 or SPI_SS0_SS1.
  * @retval None.
  */
static __INLINE void SPI_SetSS(SPI_TypeDef *SpiPort, SPI_SLAVE_SEL eSlaveSel)
{
    SpiPort->SSR = (SpiPort->SSR & ~SPI_SSR_SSR_MASK) | eSlaveSel;
}

/**
  * @brief  Set the specified slave select pins to inactive state.
  * @param  SpiPort: SPI port handler  
  *	@param  eSlaveSel: Specify slave select pins.
  *						It could be SPI_NONE, SPI_SS0, SPI_SS1 or SPI_SS0_SS1.
  * @retval None.
  */
static __INLINE void SPI_ClrSS(SPI_TypeDef *SpiPort, SPI_SLAVE_SEL eSlaveSel)
{	
	SpiPort->SSR &= ~eSlaveSel;	
}

/**
  * @brief  Check the busy status of the specified SPI port.
  * @param  SpiPort: SPI port handler  
  * @retval TRUE:  The SPI port is in busy.
  *			FALSE: The SPI port is not in busy.
  */
static __INLINE uint8_t SPI_IsBusy(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->CTL & SPI_CTL_GO_BUSY)?TRUE:FALSE);
}

/**
  * @brief  Enalbe variable clock pattern.
  * @param  SpiPort: SPI port handler
  * @param  u32Pattern: Specify the variable clock pattern
  * @retval None.
  */
static __INLINE void SPI_EnableVariableClockFunction(SPI_TypeDef *SpiPort, uint32_t u32Pattern)
{
	SpiPort->CTL |= SPI_CTL_VARCLK_EN;
	SpiPort->VARCLK = u32Pattern;	
}

/**
  * @brief  Disable variable clock pattern.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_DisableVariableClockFunction(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL &= ~SPI_CTL_VARCLK_EN;	
}

/**
  * @brief  Return the SPI interrupt flag
  * @param  SpiPort: SPI port handler
  * @retval 0: SPI interrupt doesn't occur
  *         1: SPI interrupt occurs  
  */
static __INLINE uint32_t SPI_GetIntFlag(SPI_TypeDef *SpiPort)
{
	return ((SpiPort->STATUS & SPI_STATUS_INTSTS)? TRUE:FALSE);
}

/**
  * @brief  Clear the SPI interrupt flag.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_ClrIntFlag(SPI_TypeDef *SpiPort)
{
	SpiPort->STATUS |= SPI_STATUS_INTSTS;
}

/**
  * @brief  Set the GO_BUSY bit to trigger a SPI data trasfer.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_SetGo(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL |= SPI_CTL_GO_BUSY;
}

/**
  * @brief  Clear the GO_BUSY bit.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_ClrGo(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL &= ~SPI_CTL_GO_BUSY;
}

/**
  * @brief  Enable the SPI 3-wire function. In master mode, executing this function is unmeaningful.
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
static __INLINE void SPI_Enable3Wire(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR |= SPI_SSR_NOSLVSEL;
}

/**
  * @brief  Disable the SPI 3-wire function. 
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
static __INLINE void SPI_Disable3Wire(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR &= ~SPI_SSR_NOSLVSEL;
}

/**
  * @brief  Abort transfer when using 3-wire SPI.                                                           
  *         If using 3-wire SPI as slave, when slave start interrupt status is set but transfer done flag        
  *         doesn't be set over a reasonable time, use this function to abort this transfer.                      
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
static __INLINE void SPI_3WireAbort(SPI_TypeDef *SpiPort)
{
    SpiPort->SSR |= SPI_SSR_SLV_ABORT;
}

/**
  * @brief  Return the 3-wire SPI start interrupt status.
  * @param  SpiPort: SPI port handler  
  * @retval 0: SPI start interrupt doesn't occur 
  * @retval 1: SPI start interrupt occurs
  */
static __INLINE uint32_t SPI_Get3WireStartIntFlag(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->STATUS & SPI_STATUS_SLV_START_INTSTS)?TRUE:FALSE);
}

/**
  * @brief  Clear the 3-wire SPI start interrupt status. 
  * @param  SpiPort: SPI port handler  
  * @retval None.
  */
static __INLINE void SPI_Clr3WireStartIntFlag(SPI_TypeDef *SpiPort)
{
    SpiPort->STATUS |= SPI_STATUS_SLV_START_INTSTS;
}

/**
  * @brief  Check the status of the Rx buffer of the specified SPI port.
  * @param  SpiPort: SPI port handler
  * @retval TRUE:  Rx buffer is empty.
  * @retval FALSE: Rx buffer is not empty.
  */
static __INLINE uint8_t SPI_IsRxEmpty(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->STATUS & SPI_STATUS_RX_EMPTY)?TRUE:FALSE);
}

/**
  * @brief  Check the status of the Rx buffer of the specified SPI port.
  * @param  SpiPort: SPI port handler
  * @retval TRUE:  Rx buffer is full.
  * @retval FALSE: Rx buffer is not full.
  */
static __INLINE uint8_t SPI_IsRxFull(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->STATUS & SPI_STATUS_RX_FULL)?TRUE:FALSE);
}

/**
  * @brief  Check the status of the Tx buffer of the specified SPI port.
  * @param  SpiPort: SPI port handler
  * @retval TRUE:  Tx buffer is empty.
  * @retval FALSE: Tx buffer is not empty.
  */
static __INLINE uint8_t SPI_IsTxEmpty(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->STATUS & SPI_STATUS_TX_EMPTY)?TRUE:FALSE);
}

/**
  * @brief  Check the status of the Tx buffer of the specified SPI port.
  * @param  SpiPort: SPI port handler
  * @retval TRUE:  Tx buffer is full.
  * @retval FALSE: Tx buffer is not full.
  */
static __INLINE uint8_t SPI_IsTxFull(SPI_TypeDef *SpiPort)
{
    return ((SpiPort->STATUS & SPI_STATUS_TX_FULL)?TRUE:FALSE);
}

/**
  * @brief  Clear TX FIFO.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_ClearTxFIFO(SPI_TypeDef *SpiPort)
{
	SpiPort->FF_CTL |= SPI_FFCTL_TX_CLR;
}

/**
  * @brief  Clear RX FIFO.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_ClearRxFIFO(SPI_TypeDef *SpiPort)
{
	SpiPort->FF_CTL |= SPI_FFCTL_RX_CLR;
}

/**
  * @brief  Enable Dual IO mode.
  * @param  SpiPort: SPI port handler
  *	@param  u8Dir: Set the direction to in/out. (SPI_DUALIO_DIR_IN / SPI_DUALIO_DIR_OUT)
  * @retval None.
  */
static __INLINE void SPI_EnableDualIOMode(SPI_TypeDef *SpiPort, uint8_t u8Dir)
{
	SpiPort->CTL |= SPI_CTL_DUAL_IO_EN;
	SpiPort->CTL = SpiPort->CTL & ~SPI_CTL_DUAL_IO_DIR | u8Dir;
}

/**
  * @brief  Disable Dual IO mode.
  * @param  SpiPort: SPI port handler
  * @retval None.
  */
static __INLINE void SPI_DisableDualIOMode(SPI_TypeDef *SpiPort)
{
	SpiPort->CTL &= ~SPI_CTL_DUAL_IO_EN;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void SPI_Init(SPI_TypeDef *SpiPort);
void SPI_DeInit(SPI_TypeDef *SpiPort);
int32_t SPI_Open(SPI_TypeDef *SpiPort, SPI_DATA_T *sParam);
void SPI_Close(SPI_TypeDef *SpiPort);
void SPI_SetBitLength(SPI_TypeDef *SpiPort, int32_t i32BitLength);
void SPI_SetSuspendCycle(SPI_TypeDef *SpiPort, int32_t i32Interval);
uint32_t SPI_SetClockFreq(SPI_TypeDef *SpiPort, uint32_t u32Clock1, uint32_t u32Clock2);
uint32_t SPI_GetClock1Freq(SPI_TypeDef *SpiPort);
uint32_t SPI_GetClock2Freq(SPI_TypeDef *SpiPort);
void SPI_EnableInt(SPI_TypeDef *SpiPort);
void SPI_DisableInt(SPI_TypeDef *SpiPort);
uint8_t SPI_SingleRead(SPI_TypeDef *SpiPort, uint32_t *pu32Data);
uint8_t SPI_SingleWrite(SPI_TypeDef *SpiPort, uint32_t *pu32Data);
uint32_t SPI_DumpRxRegister(SPI_TypeDef *SpiPort, uint32_t *pu32Buf, uint32_t u32DataCount);
uint32_t SPI_SetTxRegister(SPI_TypeDef *SpiPort, uint32_t *pu32Buf, uint32_t u32DataCount);
void SPI_SetTxPDMA(SPI_TypeDef *SpiPort, uint8_t bEnable);
void SPI_SetRxPDMA(SPI_TypeDef *SpiPort, uint8_t bEnable);
void SPI_Enable3WireStartInt(SPI_TypeDef *SpiPort);
void SPI_Disable3WireStartInt(SPI_TypeDef *SpiPort);
void SPI_WakeupConfigure(SPI_TypeDef *SpiPort, uint8_t bEnable);
void SPI_SetFIFOMode(SPI_TypeDef *SpiPort, uint8_t bEnable, int32_t i32Interval);
int32_t SPI_FIFOWrite8(SPI_TypeDef *SpiPort, uint8_t *pu8TxBuf, uint32_t u32WriteLength);
int32_t SPI_FIFOWrite16(SPI_TypeDef *SpiPort, uint16_t *pu16TxBuf, uint32_t u32WriteLength);
int32_t SPI_FIFOWrite32(SPI_TypeDef *SpiPort, uint32_t *pu32TxBuf, uint32_t u32WriteLength);
int32_t SPI_FIFORead8(SPI_TypeDef *SpiPort, uint8_t *pu8RxBuf, uint32_t u32ReadLength);
int32_t SPI_FIFORead16(SPI_TypeDef *SpiPort, uint16_t *pu16RxBuf, uint32_t u32ReadLength);
int32_t SPI_FIFORead32(SPI_TypeDef *SpiPort, uint32_t *pu32RxBuf, uint32_t u32ReadLength);
void SPI_FIFOReadWrite8(SPI_TypeDef *SpiPort, uint8_t *pu8TxBuf, uint8_t *pu8RxBuf, uint32_t u32Length);
void SPI_FIFOReadWrite16(SPI_TypeDef *SpiPort, uint16_t *pu16TxBuf, uint32_t *pu16RxBuf, uint32_t u32Length);
void SPI_FIFOReadWrite32(SPI_TypeDef *SpiPort, uint32_t *pu32TxBuf, uint32_t *pu32RxBuf, uint32_t u32Length);
uint32_t SPI_GetSourceClockFreq(SPI_TypeDef *SpiPort);

/*@}*/ /* end of group NANO1xx_SPI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_SPI_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#endif

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

