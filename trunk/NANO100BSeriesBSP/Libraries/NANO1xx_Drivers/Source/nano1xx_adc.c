/******************************************************************************
 * @file     nano1xx_adc.c
 * @brief	 NANO1xx ADC driver source file
 * @version	 1.0.1
 * @date	 04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "nano1xx.h"
#include "nano1xx_adc.h"
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"

/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_ADC_Driver ADC Driver
  @{
*/


/** @addtogroup NANO1xx_ADC_EXPORTED_FUNCTIONS ADC Exported Functions
  @{
*/

/**
  * @brief  Enable and configure ADC function, and set resolution to 12 bit.
  * @param  u32InputMode Specify the operation mode, valid values are ADC_CR_DIFF and ADC_CR_SINGLE_END
  * @param  u32OpMode Specify the input mode, valid values are ADC_CR_ADMD_SINGEL, ADC_CR_ADMD_S_CYCLE, and ADC_CR_ADMD_CONTINUOUS
  * @param  u32Mask Specify the input channel mask, each bit represents one channel starting from LSB.
  * @param  u32Ref Specify reference voltage source, valid values are ADC_CR_REFSEL_AVDD, ADC_CR_REFSEL_INT_REF, and ADC_CR_REFSEL_VREF
  * @return None
  * @note   Don't forget to configure the corresponding pins of the specified channels as analog input pins.
  *         This function does not turn on ADC power.
  */
void ADC_Init(uint32_t u32InputMode, uint32_t u32OpMode, uint32_t u32Mask, uint32_t u32Ref)
{
        assert_param(u32Mask < (1 << ADC_CH_NUM));
        assert_param((u32InputMode == ADC_CR_DIFF) || (u32InputMode == ADC_CR_SINGLE_END));
	assert_param((u32OpMode == ADC_CR_ADMD_SINGEL) ||
                     (u32OpMode == ADC_CR_ADMD_S_CYCLE) ||
                     (u32OpMode == ADC_CR_ADMD_CONTINUOUS));
        assert_param((u32Ref == ADC_CR_REFSEL_AVDD) ||
		(u32Ref == ADC_CR_REFSEL_INT_REF) ||
		(u32Ref == ADC_CR_REFSEL_VREF));

        /* ADC engine clock enable */
        CLK->APBCLK |= CLK_APBCLK_ADC_EN;
	ADC->CR = u32InputMode | u32OpMode | u32Ref | ADC_CR_RESSEL_12BIT;
        ADC->CHER = u32Mask;

}

/**
  * @brief  Disable ADC. This function truns off analog power, ADC clock, and disable ADC interrupt
  * @param  None
  * @return None
  */
void ADC_DeInit(void)
{
        ADC->CHER = 0;
        ADC->CR = 0;
        ADC->CMPR0 = 0;
        ADC->CMPR1 = 0;
        CLK->APBCLK &= ~CLK_APBCLK_ADC_EN;         /* ADC engine clock disable */

        NVIC_DisableIRQ(ADC_IRQn);
}




/**
  * @brief  Enable ADC interrupt
  * @param  None
  * @return None
  */
void ADC_EnableInt(void)
{

        ADC->SR = ADC_SR_ADF;  // clear interrupt flag
        ADC->CR |= ADC_CR_ADIE;
        NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief  Disable ADC interrupt
  * @param  None
  * @return None
  */
void ADC_DisableInt(void)
{
        ADC->CR &= ~ADC_CR_ADIE;
        if(!(ADC->CMPR0 & ADC_CMPR_CMPIE))
            if(!(ADC->CMPR1 & ADC_CMPR_CMPIE))
		NVIC_DisableIRQ(ADC_IRQn);
}


/**
  * @brief  Enable comperator 0 interrupt
  * @param  None
  * @return None
  */
void ADC_EnableComp0Int(void)
{

        ADC->SR = ADC_SR_CMPF0;        /* clear the ADC comparator 1 interrupt flag for safe */
        ADC->CMPR0 |= ADC_CMPR_CMPIE;
        NVIC_EnableIRQ(ADC_IRQn);
}


/**
  * @brief  Disable comperator 0 interrupt
  * @param  None
  * @return None
  */
void ADC_DisableComp0Int(void)
{
        ADC->CMPR0 &= ~ADC_CMPR_CMPIE;

        if(!(ADC->CR & ADC_CR_ADIE))
            if(!(ADC->CMPR1 & ADC_CMPR_CMPIE))
		NVIC_DisableIRQ(ADC_IRQn);
}

/**
  * @brief  Enable comperator 1 interrupt
  * @param  None
  * @return None
  */
void ADC_EnableComp1Int(void)
{

        ADC->SR = ADC_SR_CMPF1;        /* clear the ADC comparator 1 interrupt flag for safe */
        ADC->CMPR1 |= ADC_CMPR_CMPIE;
        NVIC_EnableIRQ(ADC_IRQn);
}


/**
  * @brief  Disable comperator 1 interrupt
  * @param  None
  * @return None
  */
void ADC_DisableComp1Int(void)
{
        ADC->CMPR1 &= ~ADC_CMPR_CMPIE;

        if(!(ADC->CR & ADC_CR_ADIE))
            if(!(ADC->CMPR0 & ADC_CMPR_CMPIE))
		NVIC_DisableIRQ(ADC_IRQn);

}


/**
  * @brief  Enable and configure ADC external trigger
  * @param  u32Condition Trigger condition, valid values are ADC_CR_TRGCOND_L_LEV, ADC_CR_TRGCOND_H_LEV, ADC_CR_TRGCOND_F_EDGE, and ADC_CR_TRGCOND_R_EDGE
  * @return None
  */
void ADC_EnableExtTrigger(uint32_t u32Condition)
{
	uint32_t reg;

	assert_param((u32Condition == ADC_CR_TRGCOND_L_LEV) ||
		(u32Condition == ADC_CR_TRGCOND_H_LEV) ||
		(u32Condition == ADC_CR_TRGCOND_F_EDGE) ||
		(u32Condition == ADC_CR_TRGCOND_R_EDGE));

	reg = ADC->CR & ~(ADC_CR_TRGCOND_MASK);
	ADC->CR = reg | u32Condition | ADC_CR_TRGEN;

}




/**
  * @brief  Get the latest conversion result of specified channel
  * @param  u8Ch Specify the ADC channel
  * @return Conversion result
  */
int32_t ADC_GetConversionData(uint8_t u8Ch)
{
        return(*(__IO uint32_t *)(ADC_BASE + (u8Ch * 4)) & ADC_RESULT_RSLT_MASK);
}

/**
  * @brief  Check if data is valid on specified channel or not
  * @param  u8Ch Specify the ADC channel to check
  * @return Valid or not
  * @retval 1 Valid
  * @retval 0 Not valid
  */
uint32_t ADC_IsDataValid(uint8_t u8Ch)
{
        assert_param(u8Ch < ADC_CH_NUM);

        return((*(__IO uint32_t *)(ADC_BASE + (u8Ch * 4)) & ADC_RESULT_VALID) ? 1 : 0);
}

/**
  * @brief  Check if data over run occured on specified channel or not
  * @param  u8Ch Specify the ADC channel to check
  * @return Overrun or not
  * @retval 1 Overrun
  * @retval 0 Not overrun
  */
uint32_t ADC_IsDataOverrun(uint8_t u8Ch)
{
        assert_param(u8Ch < ADC_CH_NUM);

        return((*(__IO uint32_t *)(ADC_BASE + (u8Ch * 4)) & ADC_RESULT_OVERRUN) ? 1 : 0);
}

/**
  * @brief  Enable and congire ADC comparator 0
  * @param  u8ch Specify the channel number to compare
  * @param  u32Condition Specify the compare condition, could be ADC_CMPR_CMPCOND_GE or ADC_CMPR_CMPCOND_LT
  * @param  u16Data Specify the compare data
  * @param  u8Count Specify the compare match count, valid values are between 1~16
  * @return This API always success and has no return value.
  */
void ADC_EnableComp0(uint8_t u8ch,
                             uint32_t u32Condition,
                             uint16_t u16Data,
                             uint8_t u8Count)
{
        assert_param(u8ch < ADC_CH_NUM);
        assert_param(u16Data <= ADC_RESULT_RSLT_MASK);
        assert_param(u8Count <= (ADC_CMPR_CMPMATCNT_MASK + 1));
	assert_param((u32Condition == ADC_CMPR_CMPCOND_GE) || (u32Condition == ADC_CMPR_CMPCOND_LT));

        ADC->CMPR0 = (u16Data << 16) | ((u8Count - 1) << 8) | (u8ch << 3) | u32Condition | ADC_CMPR_CMPEN;

}


/**
  * @brief  Enable and congire ADC comparator 1
  * @param  u8ch Specify the channel number to compare
  * @param  u32Condition Specify the compare condition, could be ADC_CMPR_CMPCOND_GE or ADC_CMPR_CMPCOND_LT
  * @param  u16Data Specify the compare data
  * @param  u8Count Specify the compare match count, valid values are between 1~16
  * @return This API always success and has no return value.
  */
void ADC_EnableComp1(uint8_t u8ch,
                             uint32_t u32Condition,
                             uint16_t u16Data,
                             uint8_t u8Count)
{
        assert_param(u8ch < ADC_CH_NUM);
        assert_param(u16Data <= ADC_RESULT_RSLT_MASK);
        assert_param(u8Count <= (ADC_CMPR_CMPMATCNT_MASK + 1));
	assert_param((u32Condition == ADC_CMPR_CMPCOND_GE) || (u32Condition == ADC_CMPR_CMPCOND_LT));

        ADC->CMPR1 = (u16Data << 16) | ((u8Count - 1) << 8) | (u8ch << 3) | u32Condition | ADC_CMPR_CMPEN;

}


/**
  * @brief  Enable ADC time out trigger, and set timer trigger attributes
  * @param  cfg Time out trigger configuration.	Including transfer count and timer source.
  * @return None
  * @note   After timer trigger enabled, neither external trigger nor software
  *         trigger can start ADC conversion, and this API will close ADC analog power.
  */
void ADC_EnableTimerTrigger(S_ADC_TIMER_TRIGGER_CFG *cfg)
{
	uint32_t reg;

        assert_param(cfg != NULL);
        assert_param(cfg->u8Cnt != 0);
        assert_param((cfg->u32Src == ADC_CR_TMSEL_TMR0_CH0) ||
		(cfg->u32Src == ADC_CR_TMSEL_TMR0_CH1) ||
		(cfg->u32Src == ADC_CR_TMSEL_TMR1_CH0) ||
		(cfg->u32Src == ADC_CR_TMSEL_TMR1_CH1));

	reg = ADC->CR & ~(ADC_CR_TMSEL_MASK | ADC_CR_TMPDMACNT_MASK | ADC_CR_ADEN);
	ADC->CR = reg | (cfg->u32Src) | (cfg->u8Cnt) | ADC_CR_TMTRGMOD;

}




/**
  * @brief  Config calibration function
  * @param  u8Enable Enable or disable calibration block
  * @param  u32Mode Calibration mode. Could be ADC_CALCTL_CALSEL or ADC_CALCTL_LOAD
  * @return None
  */
void ADC_ConfigCal(uint8_t u8Enable, uint32_t u32Mode)
{

	uint32_t reg;

	assert_param((u32Mode == ADC_CALCTL_CALSEL) || (u32Mode == ADC_CALCTL_LOAD));

	reg = ADC->CALCTL & ~(ADC_CALCTL_CALEN | ADC_CALCTL_CALSEL);
	ADC->CALCTL = reg | u32Mode | (u8Enable == 0 ? 0 : 1);

}





/**
  * @brief  Set power down mode
  * @param  u32PdMode Power down mode
  * @param  u32CalEn Recalibrate after power up or not
  * @return None
  */
void ADC_SetPDMode(uint32_t u32PdMode, uint32_t u32CalEn)
{
	uint32_t reg;

	assert_param((u32PdMode == ADC_PWRCTL_PWDMOD_PD) || (u32PdMode == ADC_PWRCTL_PWDMOD_STBY));
	assert_param((u32CalEn == ADC_PWRCTL_PWDCALEN) || (u32CalEn == ADC_PWRCTL_PWDCALDIS));

	reg = ADC->PWRCTL & ~(ADC_PWRCTL_PWDCALEN | ADC_PWRCTL_PWDMOD_STBY);
	ADC->PWRCTL = reg | u32PdMode | u32CalEn;

}

/**
  * @brief  Set ADC resolution
  * @param  u32Res Resolution, could be ADC_CR_RESSEL_6BIT, ADC_CR_RESSEL_8BIT, ADC_CR_RESSEL_10BIT, ADC_CR_RESSEL_12BIT
  * @return None
  */
void ADC_SetResolution(uint32_t u32Res)
{
        assert_param((u32Res == ADC_CR_RESSEL_6BIT) ||
                     (u32Res == ADC_CR_RESSEL_8BIT) ||
                     (u32Res == ADC_CR_RESSEL_10BIT) ||
                     (u32Res == ADC_CR_RESSEL_12BIT));
        ADC->CR = (ADC->CR & ~ADC_CR_RESSEL_MASK) | u32Res;

}

/**
  * @brief  Set ADC sample time of specified channel
  * @param  u8Ch Channel number
  * @param  u32Time Sample time
  * @return None
  * @note   All internal channels shares a single setting
  */
void ADC_SetSampleTime(uint8_t u8Ch, uint32_t u32Time)
{
        uint32_t reg;

        assert_param(u8Ch < ADC_CH_NUM);
        assert_param(u32Time < ADC_SMPLCNT_Mask);

        if(u8Ch< 8) {
                reg = ADC->SMPLCNT0 & ~(0xF << (4 *u8Ch));
                ADC->SMPLCNT0 = reg | (u32Time << (4 * u8Ch));
        } else if(u8Ch < 12) {
                reg = ADC->SMPLCNT1 & ~(0xF << (4 * (u8Ch - 8)));
                ADC->SMPLCNT1 = reg | (u32Time << (4 * (u8Ch - 8)));
        } else {  // internal channel
                reg = ADC->SMPLCNT1 & ~(0xF << (4 * (13 - 8)));
                ADC->SMPLCNT1 = reg | (u32Time << (4 * (13 - 8)));
        }

}

/*@}*/ /* end of group NANO1xx_ADC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_ADC_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */

/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
