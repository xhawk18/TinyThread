/******************************************************************************
 * @file     nano1xx_dac.h
 * @brief    NANO1xx DAC driver header file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nano1xx.h"
#include "nano1xx_assert.h"
#ifndef _NANO1XX_DAC_H_
#define _NANO1XX_DAC_H_

#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_DAC_Driver DAC Driver
  @{
*/

/** @addtogroup NANO1xx_DAC_EXPORTED_CONSTANTS DAC Exported Constants
  @{
*/

/** Max valid channel number. NANO100 supports 2 DAC channels, 0~1 */
#define DAC_CH_NUM      2

#define DAC_CH0_POWER_ON	do{DAC->CTL0 |= DAC_CTL_EN;}while(0)	/*!< Turn on DAC0 analog power */
#define DAC_CH1_POWER_ON	do{DAC->CTL1 |= DAC_CTL_EN;}while(0)	/*!< Turn on DAC1 analog power */
#define DAC_CH0_POWER_OFF	do{DAC->CTL0 &= ~DAC_CTL_EN;}while(0)	/*!< Turn off DAC0 analog power */
#define DAC_CH1_POWER_OFF	do{DAC->CTL1 &= ~DAC_CTL_EN;}while(0)	/*!< Turn off DAC1 analog power */

#define DAC_CLEAR_CH0_INTERRUPT	do{DAC->STS0 = DAC_STS_IFG;}while(0)	/*!< Clear DAC0 interrupt status */
#define DAC_CLEAR_CH1_INTERRUPT	do{DAC->STS1 = DAC_STS_IFG;}while(0)	/*!< Clear DAC1 interrupt status */

#define DAC_ENABLE_GROUP_MODE	do{DAC->COMCTL |= DAC_COMCTL_GRP;}while(0)	/*!< Enable DAC group mode */
#define DAC_DISABLE_GROUP_MODE	do{DAC->COMCTL &= ~DAC_COMCTL_GRP;}while(0)	/*!< Disable DAC group mode */

/** Indicates DAC channel's status */
typedef enum {
	DAC_IDLE,          ///< DAC channel is idle, can output new data
	DAC_BUSY,          ///< DAC channel is converting data
	DAC_INIT           ///< DAC is initializing
}E_DAC_STATE;

/*@}*/ /* end of group NANO1xx_DAC_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_DAC_EXPORTED_FUNCTIONS DAC Exported Functions
  @{
*/

/**
  * @brief  Enables DAC engine clock
  * @param  None.
  * @return None.
  */
static __INLINE void DAC_Init(void)
{
        CLK->APBCLK |= CLK_APBCLK_DAC_EN;

}

/**
  * @brief  Disables DAC engine clock and analog power
  * @param  None
  * @return None
  */
static __INLINE void DAC_DeInit(void)
{
	DAC_CH0_POWER_OFF;
	DAC_CH1_POWER_OFF;
        CLK->APBCLK &= ~CLK_APBCLK_DAC_EN;

}

/**
  * @brief  Set DAC reference voltage
  * @param  u32RefVol Assign the reference voltage source, could be DAC_COMCTL_REFSEL_AVDD, DAC_COMCTL_REFSEL_INT_VREF, or DAC_COMCTL_REFSEL_VREF
  * @return None
  */
static __INLINE void DAC_SetRefVol(uint32_t u32RefVol)
{
	assert_param((u32RefVol == DAC_COMCTL_REFSEL_AVDD) ||
		     (u32RefVol == DAC_COMCTL_REFSEL_INT_VREF) ||
		     (u32RefVol == DAC_COMCTL_REFSEL_VREF));

        DAC->COMCTL = (DAC->COMCTL & ~DAC_COMCTL_REFSEL_MASK) | u32RefVol;

}

extern void DAC_WriteData(uint8_t u8Ch, uint16_t u16Data);
extern void DAC_SetDelayTime(uint8_t u8Ch, uint16_t u16PUPDelay, uint8_t u8ConvDelay);
extern void DAC_SetRefVol(uint32_t u32RefVol);
extern void DAC_SetLoadSel(uint8_t u8Ch, uint32_t u32LoadSel);
extern void DAC_EnableInterrupt(uint8_t u8Ch);
extern void DAC_DisableInterrupt(uint8_t u8Ch);
extern E_DAC_STATE DAC_GetState(uint8_t u8Ch);

/*@}*/ /* end of group NANO1xx_DAC_EXPORTED_FUNCTIONS */


/*@}*/ /* end of group NANO1xx_DAC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


#ifdef  __cplusplus
}
#endif


#endif //#ifndef _NANO1XX_DAC_H_



/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/

