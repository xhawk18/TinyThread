
/******************************************************************************
 * @file     nano1xx_tk.h
 * @brief    NANO1xx Touch Key driver header file
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

#ifndef _NANO1XX_TK_H_
#define _NANO1XX_TK_H_


#ifdef  __cplusplus
extern "C"
{
#endif


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_TouchKey_Driver Touch Key Driver
  @{
*/


/** @addtogroup NANO1xx_TK_EXPORTED_CONSTANTS Touch Key Exported Constants
  @{
*/

/** Max valid channel number. NANO1xx supports 16 touch key channels, 0~15 */
#define TK_CH_NUM      16
#define TK_MAX_CURRENT_LEVEL	15
#define TK_MIN_CURRENT_LEVEL	1

#define TK_MAX_CLOCK_DIVIDER	15
#define TK_MIN_CLOCK_DIVIDER	0

#define TK_INT_ALL	(TK_INTEN_SCAN_THC | TK_INTEN_SCAN_COMPLETE | TK_INTEN_SEN_FAIL)

#define TK_ENABLE_START_BY_TIMER	do{TK->CTL1 |= TK_CTL1_START_BY_TMR;}while(0) 	/*!< Enable timer trigger TK */
#define TK_DISABLE_START_BY_TIMER	do{TK->CTL1 &= ~TK_CTL1_START_BY_TMR;}while(0)	/*!< Disable timer trigger TK */

#define TK_CLEAR_INTERRUPT		do{TK->STAT = TK->STAT;}while(0)

/*@}*/ /* end of group NANO1xx_TK_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_TK_EXPORTED_STRUCTS Touch Key Exported Structs
  @{
*/
/**
 * Stores TK channel settings
 */
typedef struct {
	uint8_t u8Level;	 	///< Change current level
	uint8_t u8Div;          	///< TK engine clock divider
	uint16_t	u16HighThld;	///< High threshold
	uint16_t	u16LowThld;	///< Low threahold
}S_TK_CH_CFG;

/*@}*/ /* end of group NANO1xx_TK_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_TK_EXPORTED_FUNCTIONS Touch Key Exported Functions
  @{
*/
extern void TK_Init(uint32_t u32Width);
extern void TK_DeInit(void);
extern void TK_Reset(void);
extern void TK_Start(uint16_t u16Mask);
extern void TK_ConfigChannel(uint8_t u8Ch, S_TK_CH_CFG *cfg);
extern void TK_EnableInterrupt(uint32_t u32Mask);
extern void TK_DisableInterrupt(uint32_t u32Mask);
extern int16_t TK_ReadData(uint8_t u8Ch);
extern void TK_ReadStatus(uint8_t *pu8Level, uint8_t *pu8Fail, uint8_t *pu8Th);
extern void TK_CheckBusyComplete(uint8_t *pu8Busy, uint8_t *pu8Complete);

/*@}*/ /* end of group NANO1xx_TK_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_TouchKey_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

#ifdef  __cplusplus
}
#endif

#endif //#ifndef _NANO1XX_TK_H_

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
