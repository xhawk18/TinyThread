/******************************************************************************
 * @file     nano1xx_adc.h
 * @brief	 NANO1xx ADC driver header file
 * @version	 1.0.1
 * @date	 04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __NANO1XX_ADC_H
#define __NANO1XX_ADC_H
#include "nano1xx.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_ADC_Driver ADC Driver
  @{
*/

/** @addtogroup NANO1xx_ADC_EXPORTED_STRUCTS ADC Exported Structs
  @{
*/

/** Stores timer trigger relate settings  */
typedef struct {
        uint32_t u32Src;    	  ///< Indicates timer trigger source
        uint8_t u8Cnt;   ///< Conversion count for each iteration before go back to sleep
} S_ADC_TIMER_TRIGGER_CFG;

/*@}*/ /* end of group NANO1xx_ADC_EXPORTED_STRUCTS */


/** @addtogroup NANO1xx_ADC_EXPORTED_CONSTANTS ADC Exported Constants
  @{
*/

/** There are 18 ADC channels, 0~17 */
#define ADC_CH_NUM          18

#define ADC_START_CONV 			do{ADC->CR |= ADC_CR_ADST;}while(0)	///< Start ADC conversion
#define ADC_STOP_CONV 			do{ADC->CR &= ~ADC_CR_ADST;}while(0)	///< Stop ADC conversion

#define ADC_CLEAR_ALL_INTERRUPT		do{ADC->SR = ADC->SR;}while(0)		///< Clear all ADC interrupt flags

#define ADC_DISABLE_EXT_TRIGGER		do{ADC->CR &= ~ADC_CR_TRGEN;}while(0)	///< Disable ADC external trigger
#define ADC_DISABLE_TIMER_TRIGGER	do{ADC->CR &= ~ADC_CR_TMTRGMOD;}while(0)///< Disable ADC external trigger

#define ADC_DISABLE_COMP0	do{ADC->CMPR0 &= ~ADC_CMPR_CMPEN;}while(0)	///< Disable ADC comperator 0
#define ADC_DISABLE_COMP1	do{ADC->CMPR1 &= ~ADC_CMPR_CMPEN;}while(0)	///< Disable ADC comperator 1

#define ADC_ENABLE_PDMA		do{ADC->CR |= ADC_CR_PTEN;}while(0)		///< Enable PDMA data transfer
#define ADC_DISABLE_PDMA	do{ADC->CR &= ~ADC_CR_PTEN;}while(0)		///< Disable PDMA data transfer

#define ADC_START_CALIBRATION	do{ADC->CALCTL |= ADC_CALCTL_CALSTART;}while(0)	///< Start ADC calibration

#define ADC_POWER_ON		do{ADC->CR |= ADC_CR_ADEN; \
				   while(((ADC->PWRCTL & ADC_PWRCTL_PUPRDY)== 0) || ((ADC->SR & ADC_SR_INITRDY) == 0));\
				}while(0)		///< Enable ADC power
#define ADC_POWER_DOWN		do{ADC->CR &= ~ADC_CR_ADEN;}while(0)		///< Disable ADC power

/*@}*/ /* end of group NANO1xx_ADC_EXPORTED_CONSTANTS */


/** @addtogroup NANO1xx_ADC_EXPORTED_FUNCTIONS ADC Exported Functions
  @{
*/

/**
  * @brief  Check if ADC conversion is complete
  * @param  None
  * @return ADC status
  * @retval 0 ADC is still busy
  * @retval 1 ADC conversion complete
  */
static __INLINE uint32_t ADC_IsConversionDone(void)
{
        return((ADC->SR & ADC_SR_ADF) ? 1 : 0);
}

/**
  * @brief  Check if calibration complete or not
  * @param  none
  * @return Complete or not
  * @retval 1 Complete
  * @retval 0 Not complete
  */
static __INLINE int8_t ADC_IsCalDone(void)
{
        return((ADC->CALCTL & ADC_CALCTL_CALDONE) ? 1 : 0);

}

/**
  * @brief  Set calibration word
  * @param  data A 7-bit data to set as calibration word
  * @return None
  */
static __INLINE void ADC_SetCalWord(uint8_t data)
{

        assert_param(data <= ADC_CALWORD_WORD_MASK);

        ADC->CALWORD = data;

}

/**
  * @brief  Get calibration word
  * @param  None
  * @return Current calibratiobn word
  */
static __INLINE uint8_t ADC_GetCalWord(void)
{
        return((uint8_t)(ADC->CALWORD & ADC_CALWORD_WORD_MASK));
}

/**
  * @brief  Set ADC input channel mask
  * @param  u32Mask ADC channel mask specified the enabled channel(s)
  * @return This API always success and has no return value.
  */
static __INLINE void ADC_SetChannelMask(uint32_t u32Mask)
{
        assert_param(u32Mask < (1 << ADC_CH_NUM));
        ADC->CHER = u32Mask;
}

void ADC_Init(uint32_t u32InputMode, uint32_t u32OpMode, uint32_t u32Mask, uint32_t u32Ref);
void ADC_DeInit(void);


void ADC_EnableExtTrigger(uint32_t u32Condition);

void ADC_EnableInt(void);
void ADC_DisableInt(void);
void ADC_EnableComp0Int(void);
void ADC_DisableComp0Int(void);
void ADC_EnableComp1Int(void);
void ADC_DisableComp1Int(void);


int32_t ADC_GetConversionData(uint8_t u8Ch);
uint32_t ADC_IsDataValid(uint8_t u8Ch);
uint32_t ADC_IsDataOverrun(uint8_t u8Ch);

void ADC_EnableComp0(uint8_t u8ch, uint32_t u32Condition, uint16_t u16Data, uint8_t u8Count);
void ADC_EnableComp1(uint8_t u8ch, uint32_t u32Condition, uint16_t u16Data, uint8_t u8Count);

void ADC_EnableTimerTrigger(S_ADC_TIMER_TRIGGER_CFG *cfg);

void ADC_ConfigCal(uint8_t u8Enable, uint32_t u32Mode);

void ADC_SetPDMode(uint32_t u32PdMode, uint32_t u32CalEn);
void  ADC_SetResolution(uint32_t u32Res);
void  ADC_SetSampleTime(uint8_t u8Ch, uint32_t u32Time);


/*@}*/ /* end of group NANO1xx_ADC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_ADC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


#endif //__NANO1XX_ADC_H

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
