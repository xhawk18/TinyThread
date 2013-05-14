/******************************************************************************
 * @file     nano1xx_pwm.c
 * @brief    NANO1xx PWM driver source file
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "nano1xx_pwm.h"
#include "nano1xx_sys.h"
#include "nano1xx_assert.h"


/** @addtogroup NANO1xx_Device_Driver NANO1xx Device Driver
  @{
*/

/** @addtogroup NANO1xx_PWM_Driver PWM Driver
  @{
*/

/// @cond
// two channels share a single clock en bit. need to keep track of enabled channel, so we know when to disable clock
static uint8_t pwm_en = 0;

/// @endcond

/** @addtogroup NANO1xx_PWM_EXPORTED_FUNCTIONS PWM Exported Functions
  @{
*/


/**
  * @brief  Set PWM counter
  * @param  u8Ch  Specify the PWM channel
  * @param  u32Cn Counter value
  * @param  u16Cm Comperator value
  * @return None
  */
void PWM_SetCounter(uint8_t u8Ch, uint32_t u32Cn, uint16_t u16Cm)
{
	assert_param(u8Ch < PWM_CH_NUM);
        if (u8Ch < 4) {
                *(__IO uint32_t *) (&PWM0->DUTY0 + 0x3 * u8Ch) = ((u16Cm << 16) | u32Cn);
        } else {
                *(__IO uint32_t *) (&PWM1->DUTY0 + 0x3 * (u8Ch - 4)) = ((u16Cm << 16) | u32Cn);
        }
}


/**
  * @brief  Enable PWM interrupt
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_EnableInt(uint8_t u8Ch)
{
        PWM_TypeDef *base;
        uint8_t ch;

	assert_param(u8Ch < PWM_CH_NUM);

        base = (u8Ch < 4) ? PWM0 : PWM1;
        ch = u8Ch & 0x3;


        base->INTSTS = 1 << ch;
        base->INTEN |= 1 << ch;


        if (u8Ch < 0x04)
                NVIC_EnableIRQ(PWM0_IRQn);
        else
                NVIC_EnableIRQ(PWM1_IRQn);
}

/**
  * @brief  Disable PWM interrupt
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_DisableInt(uint8_t u8Ch)
{
        PWM_TypeDef *base;
        uint8_t ch;

	assert_param(u8Ch < PWM_CH_NUM);

        base = (u8Ch < 4) ? PWM0 : PWM1;
        ch = u8Ch & 0x3;


        base->INTEN &= ~(1 << ch);
        base->INTSTS = 1 << ch;

        if((base->INTEN & 0xF == 0) && (base->CAPINTEN & 0x03030303) == 0)
		NVIC_DisableIRQ(u8Ch < 0x04 ? PWM0_IRQn : PWM1_IRQn);


}

/**
  * @brief  Enable PWM capture interrupt
  * @param  u8Ch  Specify the PWM channel
  * @param  u8Int Capture interrupt to be enabled  
  * @return None
  */
void PWM_EnableCaptureInt(uint8_t u8Ch, uint8_t u8Int)
{
        PWM_TypeDef *base;
        uint8_t ch;

	assert_param(u8Ch < PWM_CH_NUM);

        base = (u8Ch < 4) ? PWM0 : PWM1;
        ch = u8Ch & 0x3;


        base->CAPINTSTS = 0x7 << (ch * 8);
        base->CAPINTEN &= ~(0x3 << (ch * 8));
        base->CAPINTEN |= ((u8Int) << (ch * 8));


        if (u8Ch < 0x04)
                NVIC_EnableIRQ(PWM0_IRQn);
        else
                NVIC_EnableIRQ(PWM1_IRQn);
}

/**
  * @brief  Disable PWM capture interrupt
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_DisableCaptureInt(uint8_t u8Ch)
{
        PWM_TypeDef *base;
        uint8_t ch;

	assert_param(u8Ch < PWM_CH_NUM);

        base = (u8Ch < 4) ? PWM0 : PWM1;
        ch = u8Ch & 0x3;

        base->CAPINTEN &= ~(0x3 << (ch * 8));
        base->CAPINTSTS = 0x7 << (ch * 8);

        if((base->INTEN & 0xF == 0) && (base->CAPINTEN & 0x03030303) == 0)
		NVIC_DisableIRQ(u8Ch < 0x04 ? PWM0_IRQn : PWM1_IRQn);
}

/**
  * @brief  Clear PWM interrupt
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_ClearInt(uint8_t u8Ch)
{

	assert_param(u8Ch < PWM_CH_NUM);
	if(u8Ch < 4)
		PWM0->INTSTS = 1 << u8Ch;
	else
		PWM1->INTSTS = 1 << (u8Ch & 0x3);
}

/**
  * @brief  Clear PWM capture interrupt
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_ClearCaptureInt(uint8_t u8Ch)
{

	assert_param(u8Ch < PWM_CH_NUM);
	if(u8Ch < 4)
		PWM0->CAPINTSTS = 0x7 << (u8Ch * 8);
	else
		PWM1->CAPINTSTS = 0x7 << ((u8Ch & 0x3) * 8);
}

/**
  * @brief  Get PWM interrupt flag
  * @param  u8Ch  Specify the PWM channel
  * @return Interrupt flag of specified channel
  */
uint32_t PWM_GetIntFlag(uint8_t u8Ch)
{

	assert_param(u8Ch < PWM_CH_NUM);
	if(u8Ch < 4)
		return((PWM0->INTSTS >> u8Ch) & 1);
	else
		return((PWM1->INTSTS >> (u8Ch & 3)) & 1);
}

/**
  * @brief  Get PWM capture interrupt flag
  * @param  u8Ch  Specify the PWM channel
  * @return Interrupt flag of specified channel
  */
uint32_t PWM_GetCaptureIntFlag(uint8_t u8Ch)
{

	assert_param(u8Ch < PWM_CH_NUM);
	if(u8Ch < 4)
		return(PWM0->CAPINTSTS >> ((u8Ch * 8) & 7));
	else
		return((PWM0->CAPINTSTS >> ((u8Ch & 3) * 8)) & 7);
}


/**
  * @brief  Get PWM rising counter
  * @param  u8Ch  Specify the PWM channel
  * @return Rising counter of specified channel
  */
uint32_t PWM_GetRisingCounter(uint8_t u8Ch)
{
	assert_param(u8Ch < PWM_CH_NUM);
        if (u8Ch < 4) {
        	return(*(uint32_t *)(&PWM0->CRL0 + 0x2 * u8Ch));
        } else {
                return(*(uint32_t *)(&PWM1->CRL0 + 0x2 * (u8Ch & 0x3)));
        }
}

/**
  * @brief  Get PWM falling counter
  * @param  u8Ch  Specify the PWM channel
  * @return Falling counter of specified channel
  */
uint32_t PWM_GetFallingCounter(uint8_t u8Ch)
{
	assert_param(u8Ch < PWM_CH_NUM);
        if (u8Ch < 4) {
        	return(*(uint32_t *)(&PWM0->CFL0 + 0x2 * (u8Ch & 0x3)));
        } else {
                return(*(uint32_t *)(&PWM1->CFL0 + 0x2 * (u8Ch & 0x3)));
        }
}

/**
  * @brief  Init PWM channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_Init(uint8_t u8Ch)
{
	assert_param(u8Ch < PWM_CH_NUM);
	if(u8Ch < 2)
		CLK->APBCLK |= CLK_APBCLK_PWM0_CH01_EN;
	else if(u8Ch < 4)
		CLK->APBCLK |= CLK_APBCLK_PWM0_CH23_EN;
	else if(u8Ch < 6)
		CLK->APBCLK |= CLK_APBCLK_PWM1_CH01_EN;
	else
		CLK->APBCLK |= CLK_APBCLK_PWM1_CH23_EN;
	pwm_en |= (1 << u8Ch);
}

/**
  * @brief  DeInit PWM channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_DeInit(uint8_t u8Ch)
{
        PWM_TypeDef *base;
	uint8_t ch;
	assert_param(u8Ch < PWM_CH_NUM);


        ch = u8Ch & 0x3;
        base = (u8Ch < 4) ? PWM0 : PWM1;

	base->CTL &= ~(1 << (ch * 8));
        base->OE &= ~(1 << ch);
        base->CAPCTL &= ~(0x6 << (8 * ch));
        base->CAPINTSTS = 0x7 << (ch * 8);
        base->INTSTS = 1 << ch;

        if((base->INTEN & 0xF == 0) && (base->CAPINTEN & 0x03030303) == 0)
		NVIC_DisableIRQ(u8Ch < 0x04 ? PWM0_IRQn : PWM1_IRQn);

	pwm_en &= ~(1 << u8Ch);
	if(u8Ch < 2 && ((pwm_en & 0x3) == 0))
		CLK->APBCLK &= ~CLK_APBCLK_PWM0_CH01_EN;
	else if(u8Ch < 4 && ((pwm_en & 0xC) == 0))
		CLK->APBCLK &= ~CLK_APBCLK_PWM0_CH23_EN;
	else if(u8Ch < 6 && ((pwm_en & 0x30) == 0))
		CLK->APBCLK &= ~CLK_APBCLK_PWM1_CH01_EN;
	else if(((pwm_en & 0xC0) == 0))
		CLK->APBCLK &= ~CLK_APBCLK_PWM1_CH23_EN;

}

/**
  * @brief  Enable PWM output dead zone and set duration
  * @param  u8Ch  Specify the PWM channel
  * @param  u8Length Duration
  * @return None
  */
void PWM_EnableDeadZone(uint8_t u8Ch, uint8_t u8Length)
{
	assert_param(u8Ch < PWM_CH_NUM);


	if(u8Ch < 2) {
		PWM0->PRES = (PWM0->PRES & ~PWM_PRES_DZ01_MASK) | u8Length;
		PWM0->CTL |= PWM_CTL_DZEN01;
	} else if(u8Ch < 4) {
		PWM0->PRES = (PWM0->PRES & ~PWM_PRES_DZ23_MASK) | u8Length;
		PWM0->CTL |= PWM_CTL_DZEN23;
	} else if(u8Ch < 6) {
		PWM1->PRES = (PWM0->PRES & ~PWM_PRES_DZ01_MASK) | u8Length;
		PWM1->CTL |= PWM_CTL_DZEN01;
	} else {
		PWM1->PRES = (PWM0->PRES & ~PWM_PRES_DZ23_MASK) | u8Length;
		PWM1->CTL |= PWM_CTL_DZEN23;
        }

}

/**
  * @brief  Disable PWM output dead zone
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_DisableDeadZone(uint8_t u8Ch)
{
	assert_param(u8Ch < PWM_CH_NUM);

	if(u8Ch < 2) {
		PWM0->CTL &= ~PWM_CTL_DZEN01;
	} else if(u8Ch < 4) {
		PWM0->CTL &= ~PWM_CTL_DZEN23;
	} else if(u8Ch < 6) {
		PWM1->CTL &= ~PWM_CTL_DZEN01;
	} else {
		PWM1->CTL &= ~PWM_CTL_DZEN23;
        }
}

/**
  * @brief  Enable PWM channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_Enable(uint8_t u8Ch)
{
        PWM_TypeDef *base = (u8Ch< 4) ? PWM0 : PWM1;
	assert_param(u8Ch < PWM_CH_NUM);

        base->CTL |= (1 << ((u8Ch & 0x3) * 8));

}

/**
  * @brief  Enable PWM capture channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_CaptureEnable(uint8_t u8Ch)
{
        PWM_TypeDef *base = (u8Ch< 4) ? PWM0 : PWM1;
	assert_param(u8Ch < PWM_CH_NUM);


	base->CTL |= (0x9 << ((u8Ch & 0x3) * 8));
	base->CAPCTL |= (2 << ((u8Ch & 0x3) * 8));


}

/**
  * @brief  Disable PWM channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_Disable(uint8_t u8Ch)
{
        PWM_TypeDef *base = (u8Ch< 4) ? PWM0 : PWM1;
	assert_param(u8Ch < PWM_CH_NUM);

        base->CTL &= ~(1 << ((u8Ch & 0x3) * 8));

}

/**
  * @brief  Disable PWM capture channel
  * @param  u8Ch  Specify the PWM channel
  * @return None
  */
void PWM_CaptureDisable(uint8_t u8Ch)
{
        PWM_TypeDef *base = (u8Ch< 4) ? PWM0 : PWM1;
	assert_param(u8Ch < PWM_CH_NUM);


	base->CTL &= ~(0x9 << ((u8Ch & 0x3) * 8));
	base->CAPCTL &= ~(2 << ((u8Ch & 0x3) * 8));


}

/**
  * @brief  Configure PWM clock parameters
  * @param  u8Ch  Specify the PWM channel
  * @param  cfg Structure pointer points to timing parameters
  * @return None
  */
void PWM_SetClk(uint8_t u8Ch, S_DRVPWM_TIME_DATA_T *cfg)
{
	PWM_TypeDef *base = (u8Ch< 4) ? PWM0 : PWM1;
	uint8_t ch = u8Ch & 3;
	uint32_t reg;
	assert_param(u8Ch < PWM_CH_NUM);


	reg = base->CTL & ~(0xC << (ch * 8));
	if(cfg->u8Inv)
		reg |= (0x4 << (ch * 8));
	base->CTL = reg | (cfg->u8Mode << (ch * 8));

	reg = base->PRES & ~(0xFF << ((ch >> 1) * 8));
	base->PRES = reg | (cfg->u8PreScale << ((ch >> 1) * 8));

	reg = base->CLKSEL & ~(0x7 << (ch * 4));
	base->CLKSEL = reg | (cfg->u8Div << (ch * 4));

	*(__IO uint32_t *) (&base->DUTY0 + 0x3 * ch) = ((cfg->u16Cm << 16) | cfg->u32Cn);


}

/**
  * @brief  Enable/Disable PWM output
  * @param  u8Ch  Specify the PWM channel
  * @param  u32Enable Enable/Disable
  * @return None
  */
void PWM_ConfigOutput(uint8_t u8Ch, uint32_t u32Enable)
{
        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;

	assert_param(u8Ch < PWM_CH_NUM);
	if(u32Enable)
		base->OE |= (1 << ch);
	else
		base->OE &= ~(1 << ch);
}

/**
  * @brief  Enable/Disable PWM capture input
  * @param  u8Ch  Specify the PWM channel
  * @param  u32Enable Enable/Disable
  * @return None
  */
void PWM_ConfigCaptureIntput(uint8_t u8Ch, uint32_t u32Enable)
{

        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;

	assert_param(u8Ch < PWM_CH_NUM);
	if(u32Enable)
		base->CAPCTL |= 0x4 << (8 * ch);
	else
		base->CAPCTL &= ~(0x4 << (8 * ch));
}



/**
  * @brief  Enable/Disable PWM capture in cascade mode
  * @param  u8Ch Specity the channel to be configured
  * @param  u32Enable Enable or Disable cascade mode
  * @return None
  */
void PWM_ConfigCascade(uint8_t u8Ch, uint32_t u32Enable)
{
        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;

	assert_param(u8Ch < PWM_CH_NUM);
	assert_param((u8Ch & 1) == 0);

	if(u32Enable)
		base->CAPCTL |= (0x2000 << (8 * ch));
	else
		base->CAPCTL &= ~(0x2000 << (8 * ch));
}


/**
  * @brief  Enable PDMA transfer of a capture channel
  * @param  u8Ch Specify the PWM channel
  * @param  u32Order Specify PDMA capture order
  * @param  u32Mode Specify PDMA capture mode
  * @return None
  */
void PWM_EnablePdma(uint8_t u8Ch, uint32_t u32Order, uint32_t u32Mode)
{
        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;
	uint32_t reg;

	assert_param(u8Ch < PWM_CH_NUM);
	assert_param((u8Ch & 1) == 0);
	assert_param((u32Order & ~PWM_CAPCTL_CH0RFORDER) == 0);
	assert_param((u32Mode & ~PWM_CAPCTL_PDMACAPMOD_MASK) == 0);

	reg = base->CAPCTL;
	reg = (reg & ~PWM_CAPCTL_CH0RFORDER << (ch * 8)) | (u32Order << (ch * 8));
	reg = (reg & ~PWM_CAPCTL_PDMACAPMOD_MASK << (ch * 8)) | (u32Mode << (ch * 8));
        base->CAPCTL = reg | (PWM_CAPCTL_CH0PDMAEN << (ch * 8));

}

/**
  * @brief  Disable PDMA transfer of a capture channel
  * @param  u8Ch Specify the PWM channel
  * @return None
  */
void PWM_DisablePdma(uint8_t u8Ch)
{
        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;

	assert_param(u8Ch < PWM_CH_NUM);
	assert_param((u8Ch & 1) == 0);

        base->CAPCTL &= ~(PWM_CAPCTL_CH0PDMAEN << (ch * 8));

}

/**
  * @brief  Set attribute of capture channel
  * @param  u8Ch  Specify the PWM channel
  * @param  u8Inv Enable/Disable inverse
  * @param  u8Cas Enable/Disable cascade
  * @param  u32Reload Configure when to reload counter
  * @return None
  */
void PWM_SetCaptureAttribute(uint8_t u8Ch, uint8_t u8Inv, uint8_t u8Cas, uint32_t u32Reload)
{

        PWM_TypeDef *base = (u8Ch < 4) ? PWM0 : PWM1;
        uint8_t ch = u8Ch & 0x3;
        uint32_t reg;

	assert_param(u8Ch < PWM_CH_NUM);
        assert_param(!((ch % 2) && (u8Cas))) // cascade mode support channel 0 & 2 only
	assert_param((u32Reload & PWM_CAPCTL_CAPRELOADREN) || (u32Reload & PWM_CAPCTL_CAPRELOADFEN));

	reg = base->CAPCTL;

        if(u8Inv)
                reg |= 1 << (8 * ch);
        else
                reg &= ~(1 << (8 * ch));

        if(u8Cas)
                reg |= 0x2000 << (8 * ch);
        else  if((ch % 2) == 0)
                reg &= ~(0x2000 << (8 * ch));

        reg = (reg & ~(PWM_CAPCTL_CAPRELOAD_MASK << (8 * ch))) | (u32Reload  << (8 * ch));
	base->CAPCTL = reg;

}

/*@}*/ /* end of group NANO1xx_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO1xx_PWM_Driver */

/*@}*/ /* end of group NANO1xx_Device_Driver */


/*** (C) COPYRIGHT 2012 Nuvoton Technology Corp. ***/
